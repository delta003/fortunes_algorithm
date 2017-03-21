#include "command_line_parser.h"

using std::string;

namespace base {
    
    CmdParser* CmdParser::_instance = nullptr;

    CmdParser* CmdParser::instance() {
        if (_instance == nullptr) {
            _instance = new CmdParser();
        }
        return _instance;
    }

    bool CmdParser::Parse(int argc, char* argv[]) {
        // invalid format
        if ((argc - 1) % 2 != 0) {
            return false;
        }
        for (int i = 1; i < argc; i += 2 ) {
            string flag = string(argv[i] + 2);
            string value = string(argv[i + 1]);
            _arguments[flag] = value;
        }
        return true;
    }

    bool CmdParser::HaveArgument(const string& flag) {
        return (_arguments.find(flag) != _arguments.end());
    }

    string CmdParser::GetArgument(const string& flag) {
        if (HaveArgument(flag)) {
            return _arguments[flag];
        }
        return "";
    }
    
}  // namespace base