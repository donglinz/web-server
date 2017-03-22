//
// Created by ubuntu on 17-3-1.
//

#ifndef WEB_SERVER_LOGGER_H
#define WEB_SERVER_LOGGER_H

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <map>
#include <iostream>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <vector>
#include <atomic>
#include <thread>

namespace expr = boost::log::expressions;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;

class Logger {
public:
    static void init(std::string logPath);
    static void LogNormal(std::string msg) __wur __attribute_deprecated__;
    static void LogNotification(std::string msg);
    static void LogWarning(std::string msg);
    static void LogError(std::string msg);
    static void LogCritical(std::string msg);

private:
    enum severity_level {
        normal,
        notification,
        warning,
        error,
        critical
    };

    static void setLogPath(std::string logPath);
    static void setFilter();
    // Now our logs will be written both to the console and to the file.
    // Let's do a quick test and output something. We have to create a logger for this.
    static src::logger lg;

    static src::severity_logger< severity_level > slg;
    static std::atomic_flag atomic_bool;
};


#endif //WEB_SERVER_LOGGER_H
