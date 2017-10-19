//
// Created by jfelip on 10/18/17.
//
#ifndef JFELIP_CODEBASE_STRINGHELPERS_H
#define JFELIP_CODEBASE_STRINGHELPERS_H

#include <string>
#include <algorithm>

namespace StringHelpers
{
    // trim from start (in place)
    static inline void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string &s)
    {
        ltrim(s);
        rtrim(s);
    }

    // trim from start (copying)
    static inline std::string ltrimCopy(const std::string& cs)
    {
        std::string s = cs;
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    static inline std::string rtrimCopy(const std::string& cs)
    {
        std::string s = cs;
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    static inline std::string trimCopy(const std::string& cs) {
        std::string s = cs;
        trim(s);
        return s;
    }

}

#endif //JFELIP_CODEBASE_STRINGHELPERS_H
