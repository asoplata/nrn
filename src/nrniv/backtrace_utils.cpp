#include <iostream>
#include <cxxabi.h>
#include <cstring>
#include <regex>

/*
 * Small utility to bring C++ symbol demangling to C
 */

extern "C" {

int parse_bt_symbol(char* backtrace_line, char* symbol, char* offset) {
    std::regex btline("(\\d+)\\s+([\\w\\.]+)\\s+(0x[\\da-f]+)\\s+(\\w+)\\s+\\+\\s+(\\d+)");
    std::cmatch backtrace_match;
    if (std::regex_search(backtrace_line, backtrace_match, btline)) {
        std::strcpy(symbol, backtrace_match[4].str().c_str());
        std::strcpy(offset, backtrace_match[5].str().c_str());
        backtrace_line[backtrace_match.position(4)-1] = '\0';
        return 1;
    }
    return 0;
}

int cxx_demangle(char* symbol, char** funcname, size_t* funcname_sz) {

    int status = 0;
    char* ret = abi::__cxa_demangle(symbol, *funcname, funcname_sz, &status);
    *funcname = ret;
    return status;
}


}
