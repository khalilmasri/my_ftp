#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string.h>
#include <map>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define DBG_PRINT_LOGGER(format) \
    std::cout << "[LOG]|" << __FILENAME__ << "|" << format << std::endl;

#define DBG_PRINT_ERR(format) \
    std::cout << "[LOG]|" << __FILENAME__ << "|" << format << std::endl;

// enum logLevel_e {
//     logError    = 0,
//     logWarning  = 1,
//     logInfo     = 2,
//     logDebug    = 3
// };


// class Logger {

//     public:
//         Logger();
//         Logger(const logLevel_e&);

//         ~Logger();

//         friend void operator << (logLevel_e&, const std::string&);

//     private:
//         std::string buffer;
//         logLevel_e logLevel;
//         std::map < logLevel_e , std::string > log_table;
// };

#endif