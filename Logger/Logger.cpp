//
// Created by ubuntu on 17-3-1.
//

#include "Logger.h"

src::logger Logger::lg;
src::severity_logger< Logger::severity_level > Logger::slg;
std::mutex Logger::mutex;
using boost::shared_ptr;

template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
        std::basic_ostream< CharT, TraitsT >& strm, Logger::severity_level lvl)
{
    static const char* const str[] =
            {
                    "normal",
                    "notification",
                    "warning",
                    "error",
                    "critical"
            };
    if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
        strm << str[lvl];
    else
        strm << static_cast< int >(lvl);
    return strm;
}

void Logger::init(std::string logPath) {
    //setFilter();
    setLogPath(logPath);
    logging::add_common_attributes();

    //logging::add_console_log(std::clog, keywords::format = "%TimeStamp%: %Message%");


    // One can also use lambda expressions to setup filters and formatters


    logging::add_common_attributes();
    logging::core::get()->add_thread_attribute("Scope", attrs::named_scope());

    BOOST_LOG_FUNCTION();

    // Let's pretend we also want to profile our code, so add a special timer attribute.
    slg.add_attribute("Uptime", attrs::timer());

}

void Logger::setLogPath(std::string logPath) {
   // if(logPath.back() != '/') logPath += '/';

//    logging::add_file_log(
//            /* file name pattern */
//            keywords::file_name = /*logPath + */"log_%N.log",
//            /* rotate files every 10 MB */
//            keywords::rotation_size = 10 * 1024 * 1024,
//            /* rotate files at midnight */
//            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
//            /* log record format */
//            keywords::format = "[%TimeStamp%]: %Message%"
//    );
    if(logPath.back() != '/') logPath += '/';
    logging::add_file_log
            (
                    /* file name pattern */
            //keywords::file_name = logPath + "server_log_%N.log",
                    /* rotate files every 10 MB */
            keywords::rotation_size = 10 * 1024 * 1024,
                    /* rotate files at midnight */
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
                    /* log filter */
            //keywords::filter = expr::attr< severity_level >("Severity") >= notification,
                    /* log formatter */
            keywords::format = expr::stream
                    << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
                    << " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
                    << "]" /*[" << expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
                    << "]*/ << " <" << expr::attr< severity_level >("Severity")
                    << "> " << expr::message
/*
        keywords::format = expr::format("%1% [%2%] [%3%] <%4%> %5%")
            % expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
            % expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
            % expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
            % expr::attr< severity_level >("Severity")
            % expr::message
*/
    );
}

/* set filter level */
void Logger::setFilter() {
    logging::core::get()->set_filter(
            logging::trivial::severity >= logging::trivial::info
    );
}

/* 默认情况下normal级别的日志不会被记录
 * 若要修改过滤器级别 在add_file_log函数处修改并重新编译程序 */
void Logger::LogNormal(std::string msg) {
    std::unique_lock<std::mutex> lck(mutex);
    BOOST_LOG_SEV(slg, normal) << msg;
}

void Logger::LogNotification(std::string msg) {
    std::unique_lock<std::mutex> lck(mutex);
    BOOST_LOG_SEV(slg, notification) << msg;
}

void Logger::LogWarning(std::string msg) {
    std::unique_lock<std::mutex> lck(mutex);
    BOOST_LOG_SEV(slg, warning) << msg;
}

void Logger::LogError(std::string msg) {
    std::unique_lock<std::mutex> lck(mutex);
    BOOST_LOG_SEV(slg, error) << msg;
}

void Logger::LogCritical(std::string msg) {
    std::unique_lock<std::mutex> lck(mutex);
    BOOST_LOG_SEV(slg, critical) << msg;
}

