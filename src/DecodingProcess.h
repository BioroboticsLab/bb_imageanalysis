/*
 * DecodingProcess.h
 *
 *  Created on: 21.08.2014
 *      Author: mareikeziese
 */

#ifndef DECODINGPROCESS_H_
#define DECODINGPROCESS_H_

#include <string>
#include <boost/date_time.hpp>
#include "utility/Logger.h"
#include "utility/Export.h"

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

namespace bt = boost::posix_time;
const std::locale formats[] = {
std::locale(std::locale::classic(),new bt::time_input_facet("%Y%m%d%H%M%S"))};

const size_t formats_n = sizeof(formats)/sizeof(formats[0]);

std::time_t pt_to_time_t(const bt::ptime& pt)
{
    bt::ptime timet_start(boost::gregorian::date(1970,1,1));
    bt::time_duration diff = pt - timet_start;
    return diff.ticks()/bt::time_duration::rep_type::ticks_per_second;


}
time_t seconds_from_epoch(const std::string& s)
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

class DecodingProcess {
private:
	std::string _settingsSection;
	image_meta_t meta_infos;

	/**
	 *
	 * @param filename
	 */
	void _loadMetaInfos(const std::string &filename);
	/**
	 * @TODO implement
	 */
	void _loadSettingSection();
public:
    DecodingProcess() {}
    virtual ~DecodingProcess() {}

    void process(std::string const& filename) const;

};

#endif /* DECODINGPROCESS_H_ */

