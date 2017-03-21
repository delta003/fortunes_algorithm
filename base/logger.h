#ifndef BASE_LOGGER_H
#define BASE_LOGGER_H

#include <fstream>
#include <string>

namespace base {
    class Logger {
    public:
        // if logging to file, close file
        ~Logger();
        
        static Logger* instance();
        
        // starts logging to file
        void SetLogFile(std::string file_name);
        // starts logging to console (default)
        void SetLogConsole();
        
        void OutputInfo(std::string message);
        void OutputWarning(std::string message);
        void OutputError(std::string message);
        void OutputSuccess(std::string message);
        
    private:
        Logger() { logging_to_file = false; }
        
        bool logging_to_file;
        std::fstream file;
        static Logger* _instance;
        
        //the following are LINUX terminal color codes
        static const std::string RESET;
        static const std::string BLACK;
        static const std::string RED;
        static const std::string GREEN;
        static const std::string YELLOW;
        static const std::string BLUE;
        static const std::string MAGENTA;
        static const std::string CYAN;
        static const std::string WHITE;
        static const std::string BOLDBLACK;
        static const std::string BOLDRED;
        static const std::string BOLDGREEN;
        static const std::string BOLDYELLOW;
        static const std::string BOLDBLUE;
        static const std::string BOLDMAGENTA;
        static const std::string BOLDCYAN;
        static const std::string BOLDWHITE;
    };

}  // namespace base

#endif  // BASE_LOGGER_H