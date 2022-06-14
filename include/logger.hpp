#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <mutex>
#include <cstdio>
#include <cstring>

#define LOG_TRACE(format...)    Logger::Trace(__FILE__, format);
#define LOG_INFO(format...)     Logger::Info(__FILE__, format);
#define LOG_DEBUG(format...)    Logger::Debug(__FILE__, format);
#define LOG_WARN(format...)     Logger::warning(__FILE__, format);
#define LOG_ERR(format...)      Logger::Error(__FILE__, format);

#define __FILENAME__(file) (std::strrchr(file, '/') ? std::strrchr(file, '/') + 1 : file)

enum logPriority{
    trace   = 0, 
    debug   = 1, 
    info    = 2, 
    warning = 3, 
    error   = 4
};

class Logger
{
    private:
        std::mutex log_mutex;
        logPriority priority = info;

        template<typename... Args>
        static void log(const char* msg_prio_str, logPriority msg_prio, const char* file, const char* msg, Args... args){
                        
            if(msg_prio <= getInstance().priority){
                {
                    std::unique_lock<std::mutex> lock(getInstance().log_mutex);
                    std::cout << msg_prio_str;
                    std::cout << __FILENAME__(file) << "||";
                    std::printf(msg, args...);
                    std::cout << std::endl;
                }
            }
        }

        static Logger& getInstance(){
            static Logger logger;
            return logger;
        }

    public:
        
        static void setPriority(logPriority new_priority){
            getInstance().priority = new_priority;
        }

        template<typename... Args>
        static void Trace(const char* file, const char* msg, Args... args){
            log("[Trace]||", trace, file, msg, args...);
        }

        template<typename... Args>
        static void Debug(const char* file, const char* msg, Args... args){
            log("[Debug]||", debug, file, msg, args...);
        }

        template<typename... Args>
        static void Info(const char* file, const char* msg, Args... args){
            log("[Info]||", info, file, msg, args...);
        }

        template<typename... Args>
        static void Warning(const char* file, const char* msg, Args... args){
            log("[Warning]||", warning, file, msg, args...);
        }

        template<typename... Args>
        static void Error(const char* file, const char* msg, Args... args){
            log("[Error]||", error, file, msg, args...);
        }

};

#endif