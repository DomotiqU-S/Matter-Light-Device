//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_TRIGGERSTATE_HPP
#define CPP_TRIGGERSTATE_HPP

#include <vector>
#include <thread>
#include "Trigger.hpp"
#include "TriggerTime.hpp"
#include "croncpp.h"

class TriggerState : public Trigger
{
protected:
    string attribute;
    time_t for_s;
    TriggerTime *trigger_time;

    void IOSup();

public:
    TriggerState(string alias, string attribute, time_t for_s);
    void Run(condition_variable *cv_mother, mutex *cv_m_mother) override;
    void Stop() override;
    virtual void IO(string attribute, string value) = 0;
    ~TriggerState();
    string Print() override;
};

#endif // CPP_TRIGGERSTATE_HPP
