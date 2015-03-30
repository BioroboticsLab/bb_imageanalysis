/*
 * Logger.h
 *
 *  Created on: 29.03.2015
 *      Author: mareikeziese
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iomanip>
#include <cstddef>
#include <string>
#include <ostream>
#include <fstream>
#include <iomanip>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

enum severity_level {
	normal, notification, warning, error, critical
};

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(duration, "Duration", attrs::timer::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", attrs::named_scope::value_type)


class Logger {
public:
	static bool _init;
	static logging::attribute_set::iterator tag_addr;
	static logging::attribute_set::iterator dur_addr;
	/**
	 * setup the log file for the current process
	 */
	static void init(std::string directoryName = "") {

		std::string logFile;
		if(directoryName.length() > 0){
			logFile = directoryName+ "info.log";
		}else{
			logFile =  "decoder_info.log";
		}



	    boost::shared_ptr< logging::core > core = logging::core::get();

	    // Create a backend and attach a couple of streams to it
	    boost::shared_ptr< sinks::text_ostream_backend > backend =
	        boost::make_shared< sinks::text_ostream_backend >();
	    backend->add_stream(
	        boost::shared_ptr< std::ostream >(new std::ofstream(logFile)));

	    // Enable auto-flushing after each log record written
	    backend->auto_flush(true);

	    // Wrap it into the frontend and register in the core.
	    // The backend requires synchronization in the frontend.
	    typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;
	    boost::shared_ptr< sink_t > sink(new sink_t(backend));



		// Add attributes
		logging::add_common_attributes();



		//define format
		sink->set_formatter(
				expr::stream << std::hex << std::setw(8) << std::setfill('0')
						 << std::dec << expr::attr< boost::posix_time::ptime >("TimeStamp")<<": <"
						<< severity << ">"<< expr::if_(expr::has_attr(tag_attr))[expr::stream << "["<< tag_attr << "] "]
						<< expr::if_(expr::has_attr(duration))[expr::stream << "["<< duration << "] "] << expr::smessage);

		logging::core::get()->add_sink(sink);


		Logger::_init = true;


	}
	/**
	 *
	 * @param message
	 * @param level
	 */
	static void log(std::string message, severity_level level) {
		if(!Logger::_init){
			Logger::init();
		}
		src::severity_logger<severity_level> slg;

		BOOST_LOG_SEV(slg, level)<< message;
	}

	struct stringbuilder
	{
	   std::stringstream ss;
	   template<typename T>
	   stringbuilder & operator << (const T &data)
	   {
	        ss << data;
	        return *this;
	   }
	   operator std::string() { return ss.str(); }
	};

	static void log(std::ostream &message, severity_level level) {
			if(!Logger::_init){
				Logger::init();
			}
			src::severity_logger<severity_level> slg;
			std::string str =stringbuilder() << message.rdbuf();
#ifdef DEBUG_PROGRAM
			std::cout << str << std::endl;
#endif
			BOOST_LOG_SEV(slg, level)<< str;
		}
	/**
	 *
	 * @param message
	 * @param level
	 * @param tag_name
	 */
	static void log_tag(std::string message, severity_level level,
			std::string tag_name) {
		if(!Logger::_init){
				Logger::init();
			}
		src::severity_logger<severity_level> slg;

		slg.add_attribute("Tag", attrs::constant<std::string>(tag_name));
		BOOST_LOG_SEV(slg, level)<< message;
	}


	/**
	 *
	 * @param message
	 * @param level
	 * @param scope
	 */
	/*static void log_scope(std::string message, severity_level level,
			std::string scope) {
		if(!_init){
				init();
			}
		BOOST_LOG_NAMED_SCOPE(scope);
		src::severity_logger<severity_level> slg;
		BOOST_LOG_SEV(slg, normal)<< message;
	}*/

	/**
	 * starts a timer which will be stopped on next logging call
	 *
	 * @param message
	 * @param level
	 */
	static void log_start_timer(std::string message, severity_level level) {
		if(!Logger::_init){
			Logger::init();
			}
		src::severity_logger<severity_level> slg;
		 slg.add_attribute("Duration", attrs::timer());
		BOOST_LOG_SEV(slg, level)<<message;
	}


};

bool Logger::_init = false;

// The operator puts a human-friendly representation of the severity level to the stream
std::ostream& operator<<(std::ostream& strm, severity_level level) {
	static const char* strings[] = { "normal", "notification", "warning",
			"error", "critical" };

	if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings))
		strm << strings[level];
	else
		strm << static_cast<int>(level);

	return strm;
}


#endif /* LOGGER_H_ */
