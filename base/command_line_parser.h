#ifndef BASE_COMMAND_LINE_PARSER_H
#define BASE_COMMAND_LINE_PARSER_H

#include <string>
#include <unordered_map>

// Singleton class
// Takes a command line arguments and parse it to parse (flag, value)
// Flags starts with --
// Assumes command line has the following format:
// program_name --flag1 value1 --flag2 value2 etc.
namespace base {
    
    class CmdParser {
    public:
        static CmdParser* instance();
        
        // returns true if parsing was successful, false otherwise
        bool Parse(int argc, char* argv[]);
        
        // returns true if there is a value for given flag
        bool HaveArgument(const std::string& flag);
        
        // returns argument for given flag, or empty string if flag isn't set
        std::string GetArgument(const std::string& flag);
        
        std::unordered_map<std::string, std::string>* arguments() { return &_arguments; }
        
    private:
        CmdParser() {}
        
        static CmdParser* _instance;
        
        std::unordered_map<std::string, std::string> _arguments;
    };
    
}  // namespace base

#endif  // BASE_COMMAND_LINE_PARSER_H