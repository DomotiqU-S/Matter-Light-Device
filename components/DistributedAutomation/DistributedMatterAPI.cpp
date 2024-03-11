//
// Created by talgarr on 01/02/24.
//

#include "DistributedMatterAPI.hpp"
#include "TriggerStringState.hpp"
#include "ConditionLogical.hpp"
#include "ConditionTrigger.hpp"
#include "ConditionState.hpp"
#include "ActionDelay.hpp"
#include "TriggerTime.hpp"
#include "TriggerNumericState.hpp"
#include "ConditionStringState.hpp"
#include "ConditionNumericState.hpp"

vector<string> DistributedMatterAPI::GetDevices()
{
    vector<string> devices_alias = {"device1", "device2"};
    return devices_alias;
}

vector<Automation *> DistributedMatterAPI::GetAutomations(const string &alias)
{
    vector<Trigger *> triggers = vector<Trigger *>();
    //    auto trigger_1 = new TriggerStringState("trigger_1", "brightness", 4, "", "on");
    //    triggers.push_back(trigger_1);

    auto trigger_4 = new TriggerNumericState("trigger_4", "brightness", 0, 0, 255);
    triggers.push_back(trigger_4);
    //    auto trigger_2 = new TriggerTime("trigger_2", "*/3 * * * * ?");
    //    triggers.push_back(trigger_2);
    //    auto trigger_3 = new TriggerTime("trigger_3", "*/5 * * * * ?");
    //    triggers.push_back(trigger_3);

    vector<Condition *> conditions = vector<Condition *>();
    vector<Condition *> conditions_log = vector<Condition *>();
    auto condition_1 = new ConditionTrigger("condition_1", "trigger_4");
    conditions_log.push_back(condition_1);
    auto condition_2 = new ConditionNumericState("condition_2", "brightness", 0, 30, 255);
    conditions_log.push_back(condition_2);
    auto condition_3 = new ConditionLogical("condition_3", AND, conditions_log);
    conditions.push_back(condition_3);

    vector<Action *> actions = vector<Action *>();
    auto action = new ActionDelay("action_1", 0);
    actions.push_back(action);

    vector<Automation *> automation_vector;
    automation_vector.reserve(1);
    for (int i = 0; i < 1; i++)
    {
        automation_vector.push_back(new Automation("alias", "descript", triggers, conditions, actions));
    }
    return automation_vector;
}

bool DistributedMatterAPI::AddAutomationToDevice(const string &device_alias, const Automation &automation)
{
    return false;
}

bool DistributedMatterAPI::RemoveAutomationToDevice(const string &device_alias, const string &automation_alias)
{
    return false;
}
