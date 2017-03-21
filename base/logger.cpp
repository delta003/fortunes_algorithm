#include "logger.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

namespace base {
    
    const::string Logger::RESET = "\033[0m";
    const::string Logger::BLACK = "\033[30m";
    const::string Logger::RED = "\033[31m";
    const::string Logger::GREEN = "\033[32m";
    const::string Logger::YELLOW = "\033[33m";
    const::string Logger::BLUE = "\033[34m";
    const::string Logger::MAGENTA = "\033[35m";
    const::string Logger::CYAN = "\033[36m";
    const::string Logger::WHITE = "\033[37m";
    const::string Logger::BOLDBLACK = "\033[1m\033[30m";
    const::string Logger::BOLDRED = "\033[1m\033[31m";
    const::string Logger::BOLDGREEN = "\033[1m\033[32m";
    const::string Logger::BOLDYELLOW = "\033[1m\033[33m";
    const::string Logger::BOLDBLUE = "\033[1m\033[34m";
    const::string Logger::BOLDMAGENTA = "\033[1m\033[35m";
    const::string Logger::BOLDCYAN = "\033[1m\033[36m";
    const::string Logger::BOLDWHITE = "\033[1m\033[37m";
    
    Logger* Logger::_instance = nullptr;
    
    Logger::~Logger() {
        if (logging_to_file) {
            file.close();
        }
    }
    
    Logger* Logger::instance() {
        if (_instance == nullptr) {
            _instance = new Logger();
        }
        return _instance;
    }
    
    void Logger::SetLogFile(string file_name) {
        if (logging_to_file) {
            file.close();
        }
        file.open(file_name, std::ios::out);
        logging_to_file = true;
    }
    
    void Logger::SetLogConsole() {
        if (logging_to_file) {
            file.close();
        }
        logging_to_file = false;
    }
    
    void Logger::OutputInfo(string message) {
        if (logging_to_file) {
            file << "INFO: " << message << endl;
        } else {
            cout << message << endl;
        }
    }
    
    void Logger::OutputWarning(string message) {
        if (logging_to_file) {
            file << "WARNING: " << message << endl;
        } else {
            cout << YELLOW << message << RESET << endl;
        }
    }
    
    void Logger::OutputError(string message) {
        if (logging_to_file) {
            file << "ERROR: " << message << endl;
        } else {
            cout << RED << message << RESET << endl;
        }
    }
    
    void Logger::OutputSuccess(string message) {
        if (logging_to_file) {
            file << "SUCCESS: " << message << endl;
        } else {
            cout << GREEN << message << RESET << endl;
        }
    }
    
}  // namespace base
