#ifndef JSONUTILS_HPP
#define JSONUTILS_HPP

#include <esp_log.h>

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

enum class TriggerType
{
    TriggerTime,
    TriggerNumericState,
    TriggerStringState
};

enum class ConditionType
{
    ConditionLogical,
    ConditionNumericState,
    ConditionStringState,
    ConditionTime,
    ConditionTrigger
};

enum class ActionType
{
    ActionDelay,
    ActionCallService
};

static const char *TAG_JSON_UTILS = "JsonUtils";
class JsonUtils
{
private:
    static Condition *Json2Condition(nlohmann::json json);
    static Trigger *Json2Trigger(nlohmann::json json);
    static Action *Json2Action(nlohmann::json json);

    static void TriggerJson2MinimizedJson(nlohmann::json trigger, nlohmann::json *minimized_json);
    static void TriggerMinimizedJson2Json(nlohmann::json trigger, nlohmann::json *expanded_json);
    static void ConditionJson2MinimizedJson(nlohmann::json condition, nlohmann::json *minimized_json);
    static void ConditionMinimizedJson2Json(nlohmann::json condition, nlohmann::json *expanded_json);
    static void ActionJson2MinimizedJson(nlohmann::json action, nlohmann::json *minimized_json);
    static void ActionMinimizedJson2Json(nlohmann::json action, nlohmann::json *expanded_json);

public:
    static Automation *Json2Automation(nlohmann::json json);
    static nlohmann::json MinimizedJson2Json(nlohmann::json json);
    static nlohmann::json Json2MinimizedJson(nlohmann::json json);
};

#endif // JSONUTILS_HPP