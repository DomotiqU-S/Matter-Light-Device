//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_AUTOMATION_HPP
#define CPP_AUTOMATION_HPP

#include <vector>
#include <mutex>
#include <utility>
#include <condition_variable>
#include <thread>
#include "json.hpp"

#include "Trigger.hpp"
#include "Condition.hpp"
#include "Action.hpp"
// #include "Device.hpp"

class Automation
{
private:
    string alias;
    string description;
    vector<Condition *> conditions;
    vector<Action *> actions;
    vector<Trigger *> triggers;

    string last_trigger;
    bool has_triggered;
    bool running;
    bool starting;

    mutex cv_m;
    condition_variable cv;

public:
    Automation(string alias, string description, vector<Trigger *> triggers, vector<Condition *> conditions, vector<Action *> actions);
    ~Automation();

    // Setter
    void AddTrigger(Trigger *trigger);
    void AddCondition(Condition *condition);
    void AddAction(Action *action);

    // Getter
    string GetAlias();
    string GetDescription();
    vector<Condition *> GetConditions();
    vector<Action *> GetActions();
    vector<Trigger *> GetTriggers();
    bool HasTriggered();
    bool IsRunning();
    bool IsStarting();
    string Print();

    // Functional
    void Run(condition_variable *cv, mutex *cv_m);
    void Stop();
    void SetTrigger();
    bool Verify();
    void Do();
    void IO(string attribute, string value);
};

#endif // CPP_AUTOMATION_HPP
