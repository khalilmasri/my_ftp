#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <iostream>
#include <limits>

class ArgsException : public std::exception{

    public:
        virtual const char* what() const throw(){
            return("Usage: ./server port path");
        }
};

class PrecisionException : public std::exception{

    public:
        std::string message;
        PrecisionException(int const&pre){
            message = ("Incorrect precision for type: max ") + std::to_string(pre) + (" decimal places, Terminating.");
        }
        virtual const char* what() const throw(){
            return (message.c_str());
        }
};

#endif
