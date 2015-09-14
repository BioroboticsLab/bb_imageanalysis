#ifndef DECODINGPROCESS_H_
#define DECODINGPROCESS_H_

#include <string>

#include <boost/date_time.hpp>

std::time_t pt_to_time_t(const boost::posix_time::ptime& pt);

time_t seconds_from_epoch(const std::string& s);

class DecodingProcess {
public:
    DecodingProcess() {}
    virtual ~DecodingProcess() {}

    void process(std::string const& filename) const;

private:
    struct image_meta_t{
        /**
     * @var detected from imagename
     */
        unsigned int camera_id;
        /**
     * @var detected from imagename
     */
        time_t timestamp;
    };

    std::string _settingsSection;
    image_meta_t _meta_infos;

    void loadMetaInfos(const std::string &filename);
};

int main(int argc, char** argv);

#endif /* DECODINGPROCESS_H_ */

