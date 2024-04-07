//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_TRIGGER_HPP
#define CPP_TRIGGER_HPP

#include <string>
#include <utility>
#include <condition_variable>
#include "json.hpp"

using namespace std;

class Trigger
{
protected:
    string alias;

    bool has_triggered;
    condition_variable *cv_mother;
    mutex *cv_m_mother;

public:
    explicit Trigger(string alias);

    virtual void Run(condition_variable *cv_mother, mutex *cv_m_mother) = 0;
    virtual void Stop() = 0;
    bool HasTriggered();
    string GetAlias();
    virtual string Print() = 0;

    virtual void IO(string attribute, string value);
};

#endif // CPP_TRIGGER_HPP
