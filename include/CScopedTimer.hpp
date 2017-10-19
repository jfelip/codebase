//
// Created by jfelip on 10/18/17.
//

#ifndef JFELIP_CODEBASE_CSCOPEDTIMER_H
#define JFELIP_CODEBASE_CSCOPEDTIMER_H

#include <GenericMessages.hpp>
#include <chrono>

#define _SCOPED_TIMER_(pbn) CScopedTimer _pfinstance(pbn)

class CScopedTimer
{
public:
    std::string name;
    std::chrono::high_resolution_clock::time_point p;
    CScopedTimer(std::string const &n) : name(n), p(std::chrono::high_resolution_clock::now()) { }

    ~CScopedTimer()
    {
        using dura = std::chrono::duration<double>;
        auto d = std::chrono::high_resolution_clock::now() - p;
        _GENERIC_MESSAGE_( name + ": " + std::to_string(std::chrono::duration_cast<dura>(d).count()));
    }
};



#endif //JFELIP_CODEBASE_CSCOPEDTIMER_H
