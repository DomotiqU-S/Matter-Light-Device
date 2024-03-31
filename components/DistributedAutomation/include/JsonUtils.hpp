#ifndef JSONUTILS_HPP
#define JSONUTILS_HPP

#include "json.hpp"
#include "Automation.hpp"

#include "Condition.hpp"
#include "ConditionLogical.hpp"
#include "ConditionStringState.hpp"
#include "ConditionNumericState.hpp"
#include "ConditionTime.hpp"
#include "ConditionTrigger.hpp"

#include "Trigger.hpp"
#include "TriggerNumericState.hpp"
#include "TriggerStringState.hpp"
#include "TriggerTime.hpp"

#include "Action.hpp"
#include "ActionCallService.hpp"
#include "ActionDelay.hpp"

class JsonUtils
{
private:
    static Condition *Json2Condition(nlohmann::json json);
    static ConditionLogical *Json2ConditionLogical(nlohmann::json json);
    static ConditionStringState *Json2ConditionStringState(nlohmann::json json);
    static ConditionNumericState *Json2ConditionNumericState(nlohmann::json json);
    static ConditionTime *Json2ConditionTime(nlohmann::json json);
    static ConditionTrigger *Json2ConditionTrigger(nlohmann::json json);

    static Trigger *Json2Trigger(nlohmann::json json);
    static TriggerNumericState *Json2TriggerNumericState(nlohmann::json json);
    static TriggerStringState *Json2TriggerStringState(nlohmann::json json);
    static TriggerTime *Json2TriggerTime(nlohmann::json json);

    static Action *Json2Action(nlohmann::json json);
    static ActionCallService *Json2ActionCallService(nlohmann::json json);
    static ActionDelay *Json2ActionDelay(nlohmann::json json);

public:
    static Automation *Json2Automation(nlohmann::json json);
};

#endif // JSONUTILS_HPP