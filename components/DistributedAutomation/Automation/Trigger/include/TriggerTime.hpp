//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_TRIGGERTIMEPATTERN_HPP
#define CPP_TRIGGERTIMEPATTERN_HPP

#include "Trigger.hpp"
#include "croncpp.h"
#include <iostream>
#include "json.hpp"

class TriggerTime : public Trigger
{
protected:
    cron::cronexpr pattern;

    condition_variable cv;
    mutex cv_m;

    bool running;
    bool one_time;

public:
    TriggerTime(string alias, const string &pattern, bool one_time = false);

    void Run(condition_variable *cv_mother, mutex *cv_m_mother) override;
    void Stop() override;

    void SetPattern(const string &pattern_);
    bool IsRunning() const;
    ~TriggerTime();
};

#endif // CPP_TRIGGERTIMEPATTERN_HPP
