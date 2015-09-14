#include "DecodingProcess.h"

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <iostream>

#include <unistd.h>
#include <dirent.h>
#include <sys/resource.h>

#include <mpi.h>

#include <boost/filesystem.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <opencv2/opencv.hpp>

#include "utility/Logger.h"
#include "utility/Export.h"

//#define DEBUG_PROGRAM

/* MPI macros cause old-style-cast warnings. Therefore, if compiling with MPI,
 * we have to disable this specific warning for this compilation unit. */
#if defined(CRAY) && (defined(__GNUC__) || defined(__clang__))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

using namespace boost::filesystem;
using namespace std;

namespace bt = boost::posix_time;

namespace {
class MeasureTimeRAII {
public:
    MeasureTimeRAII(std::string const& what) :
        _start(std::chrono::steady_clock::now()), _what(what) {
    }
    ~MeasureTimeRAII() {
        std::chrono::steady_clock::time_point end =
                std::chrono::steady_clock::now();
        std::cout << _what << " took "

                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                         end - _start).count() << "ms.\n";
        Logger::log(
                    std::stringstream().flush() << _what << " took "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(
                        end - _start).count() << "ms.",
                    severity_level::normal);
    }
private:
    std::chrono::steady_clock::time_point _start;
    std::string _what;
};

const std::locale formats[] = {
    std::locale(std::locale::classic(),new bt::time_input_facet("%Y%m%d%H%M%S"))
};

const size_t formats_n = sizeof(formats)/sizeof(formats[0]);
}

void DecodingProcess::loadMetaInfos(const std::string &filename) {

    this->_meta_infos.camera_id = boost::lexical_cast<unsigned int>(
                filename.substr(4, 1));
    std::cout << "camera id " << this->_meta_infos.camera_id << std::endl;

    Logger::log(
                std::stringstream().flush() << "identificate camera id = "
                << this->_meta_infos.camera_id,severity_level::normal);
    std::string timestring = filename.substr(6, 14);
    std::cout << "camera id " << timestring << std::endl;
}

DecodingProcess::DecodingProcess(const std::string &settingsPath)
{
    pipeline::settings::preprocessor_settings_t preprocessor_settings;
    pipeline::settings::localizer_settings_t localizer_settings;
    pipeline::settings::ellipsefitter_settings_t ellipsefitter_settings;
    pipeline::settings::gridfitter_settings_t gridfitter_settings;

    for (pipeline::settings::settings_abs* settings :
         std::array<pipeline::settings::settings_abs*, 4>({&preprocessor_settings,
                                                          &localizer_settings,
                                                          &ellipsefitter_settings,
                                                          &gridfitter_settings}))
    {
        settings->loadFromJson(settingsPath);
    }

#ifdef USE_DEEPLOCALIZER
    static const boost::filesystem::path deeplocalizer_model_path(BOOST_PP_STRINGIZE(MODEL_PATH));

    boost::filesystem::path model_path(localizer_settings.get_deeplocalizer_model_file());
    model_path = deeplocalizer_model_path / model_path.filename();

    boost::filesystem::path param_path(localizer_settings.get_deeplocalizer_param_file());
    param_path = deeplocalizer_model_path / param_path.filename();

    localizer_settings.setValue(pipeline::settings::Localizer::Params::DEEPLOCALIZER_MODEL_FILE,
                                model_path.string());
    localizer_settings.setValue(pipeline::settings::Localizer::Params::DEEPLOCALIZER_PARAM_FILE,
                                param_path.string());
#endif

    _preprocessor.loadSettings(preprocessor_settings);
    _localizer.loadSettings(localizer_settings);
    _ellipseFitter.loadSettings(ellipsefitter_settings);
    _gridFitter.loadSettings(gridfitter_settings);
}

void DecodingProcess::process(std::string const& filename) {
    cv::Mat img = cv::imread(filename, CV_LOAD_IMAGE_GRAYSCALE);

    cv::Mat preprocessedImg;
    std::vector<pipeline::Tag> taglist;

#ifdef DEBUG_PROGRAM
    std::vector<pipeline::Tag> taglist_reduced;
#endif

    std::stringstream ss;
    {
        try {
            MeasureTimeRAII measure("Preprocessor");
            preprocessedImg = _preprocessor.process(img);

        } catch (const std::exception & e) {
            ss << "Error on Preprocessor: " << e.what() << " stop processing.";
            std::cout << ss.str();
            Logger::log(ss,severity_level::critical);
            return;
        }
    }
    {
        try {
            MeasureTimeRAII measure("Localizer");
            taglist = _localizer.process(std::move(img), std::move(preprocessedImg));

#ifdef DEBUG_PROGRAM
            // shorten result for faster debugging
            size_t i = 0;
            while(i < 10 && i < taglist.size()){
                taglist_reduced.push_back(taglist[i]);
                i++;
            }
            taglist = taglist_reduced;
#endif

        } catch (const std::exception & e) {
            ss << "Error on Localizer: " << e.what() << " stop processing.";
            std::cout << ss.str();
            Logger::log(ss,severity_level::critical);
            return;
        }
    }
    {
        try {
            MeasureTimeRAII measure("EllipseFitter");
            taglist = _ellipseFitter.process(std::move(taglist));
        } catch (const std::exception & e) {
            ss << "Error on EllipseFitter: " << e.what() << " stop processing.";
            std::cout << ss.str();
            Logger::log(ss,severity_level::critical);
            return;
        }
    }
    {
        try {
            MeasureTimeRAII measure("GridFitter");
            taglist = _gridFitter.process(std::move(taglist));
        } catch (const std::exception & e) {
            ss << "Error on GridFitter: " << e.what() << " stop processing.";
            std::cout << ss.str();
            Logger::log(ss,severity_level::critical);
            return;
        }
    }
    {
        try {
            MeasureTimeRAII measure("Decoder");
            taglist = _decoder.process(std::move(taglist));
        } catch (const std::exception & e) {

            ss << "Error on Decoder: " << e.what() << " stop processing.";
            std::cout << ss.str();
            Logger::log(ss,severity_level::critical);
            return;
        }
    }

    using namespace pipeline;
    path p (filename);
    Export ex = Export();
    ex.writeCSV(taglist, p.parent_path().string()+"/"+p.stem().string()+".csv");
    ex.writeSerializedObjects(taglist, p.parent_path().string()+"/"+p.stem().string()+".dat");
}



bool checkValidFilename(const string& filename) {
    //check if filename is given
    if (filename.size() == 0) {
        return false;
    }

    path p(filename);
    //check if file has the right extension
    if (p.extension().string() == ".png" || p.extension().string() == ".jpeg") {
        return true;
    }
    return false;
}

void listImagesCray( const char *directoryName, vector< std::string > &imageFiles, int world_rank ) {
    std::string path = std::string( directoryName ) + "/" + "proc_" + to_string(static_cast<long>(world_rank)) + "/";
    DIR *directory = opendir( path.c_str( ));
    struct dirent *file;
    while( ( file = readdir( directory )) ) {
        if (( file->d_type & DT_DIR )) { // directories are uninteresting
            continue;
        } // here be file..

        std::string filename = path + file->d_name;
        if (checkValidFilename(filename)) {
            imageFiles.push_back(filename);
        } else {
            Logger::log(
                        std::stringstream().flush() << "ignoring file " << filename,
                        severity_level::warning);
        }
    }
    closedir( directory );
}

void listImagesDebug(const char *directoryName,	vector<std::string> &imageFiles) {
    std::string path = std::string(directoryName)+ "/" + "proc_0" + "/";
    DIR *directory = opendir(path.c_str());
    struct dirent *file;
    while ((file = readdir(directory))) {
        if ((file->d_type & DT_DIR)) { // directories are uninteresting
            continue;
        } // here be file..

        std::string filename = path + file->d_name;
        if (checkValidFilename(filename)) {
            imageFiles.push_back(filename);
        } else {
            Logger::log(
                        std::stringstream().flush() << "ignoring file " << filename,
                        severity_level::warning);
        }
    }
    closedir(directory);
}

int main(int argc, char** argv) {
#ifndef MODEL_PATH
    static_assert(false, "Path to deeplocalizer model not defined!");
#endif
    // enable core dumps on crash
    static const __rlim_t infinity =  static_cast<__rlim_t>(-1);
    rlimit core_limit = { infinity, infinity };
    setrlimit( RLIMIT_CORE, &core_limit );

    // The MPI stuff to identify each process
#ifdef CRAY
    MPI_Init( NULL, NULL );
    int world_size;
    int world_rank;

    // Get the number of processes
    MPI_Comm_size( MPI_COMM_WORLD, &world_size );
    // Get the rank of the process
    MPI_Comm_rank( MPI_COMM_WORLD, &world_rank );
    if ( world_rank == 0 ) {
        cout << "There are " + to_string( (long long) world_size ) + " processes working." << endl;
    }
#endif

    // create a list of the files this process has to analyse
    vector<std::string> imageFiles;
    if (argc <= 1) {
        std::cerr << "directory and settings path are missing" << std::endl;
        return EXIT_FAILURE;
    } else if (argc <= 2) {
        std::cerr << "settings path is missing" << std::endl;
        return EXIT_FAILURE;
    }

    char* directoryName = argv[1];
    Logger::init(directoryName);
    Logger::log(std::stringstream().flush() << "start decoder process on folder "<< directoryName,severity_level::notification);
#ifdef CRAY
    listImagesCray( directoryName, imageFiles, world_rank );
#else	
    listImagesDebug(directoryName, imageFiles);
#endif

    unsigned int imageCount = imageFiles.size();
    if (imageCount == 0) {
        cout << "no work found. shutdown" << endl;
        Logger::log(
                    std::stringstream().flush() << "directory " << directoryName
                    << " empty. shutdown process",
                    severity_level::notification);
        return EXIT_SUCCESS;
    }

    Logger::log(
                std::stringstream().flush() << "start processing " << imageCount
                << "images ..",severity_level::notification);
    cout << "I have to work on " << imageCount << " images." << endl;

    std::string settingsFilePath(argv[2]);

    if (!boost::filesystem::is_regular_file(settingsFilePath)) {
        std::cerr << "settings file does not exist" << std::endl;
        return EXIT_FAILURE;
    }

    DecodingProcess dprocess(settingsFilePath);

    for (unsigned int imageNumber = 0; imageNumber < imageCount;
         ++imageNumber) {
        cout << "process works on image " << imageFiles[imageNumber] << endl;
        Logger::log(
                    std::stringstream().flush() << "process works on image "
                    << imageFiles[imageNumber],
                    severity_level::notification);
        dprocess.process(imageFiles[imageNumber]);
        // After processing the image it is deleted to signalize that the processing was completed.
        // If the image is not deleted, it will be analyzed again.
        if (::remove(std::string(imageFiles[imageNumber]).c_str()) != 0)
            perror("Error deleting the image ");
    }
#ifdef CRAY
    // make sure all processes finished
    MPI_Finalize( );
    if ( world_rank == 0 ) {
        cout << "Overall job time: " << endl;
    }
#endif
    return EXIT_SUCCESS;
}

time_t seconds_from_epoch(const string &s)
{
    bt::ptime pt;
    for(size_t i=0; i<formats_n; ++i)
    {
        std::istringstream is(s);
        is.imbue(formats[i]);
        is >> pt;
        if(pt != bt::ptime()) break;
    }
    std::cout << " ptime is " << pt << '\n';
    std::cout << " seconds from epoch are " << pt_to_time_t(pt) << '\n';
    return  pt_to_time_t(pt) ;
}

time_t pt_to_time_t(const boost::posix_time::ptime &pt)
{
    bt::ptime timet_start(boost::gregorian::date(1970,1,1));
    bt::time_duration diff = pt - timet_start;
    return diff.ticks()/bt::time_duration::rep_type::ticks_per_second;
}

#if defined(CRAY) && (defined(__GNUC__) || defined(__clang__))
#pragma GCC diagnostic pop
#endif
