#include "JsonUtils.hpp"

Automation *JsonUtils::Json2Automation(nlohmann::json json)
{
    string alias = json["alias"];
    string description = json["description"];
    vector<Trigger *> triggers;
    vector<Condition *> conditions;
    vector<Action *> actions;

    for (auto &trigger : json["triggers"])
    {
        triggers.push_back(Json2Trigger(trigger));
    }
    for (auto &condition : json["conditions"])
    {
        conditions.push_back(Json2Condition(condition));
    }
    for (auto &action : json["actions"])
    {
        actions.push_back(Json2Action(action));
    }

    return new Automation(alias, description, triggers, conditions, actions);
}

Condition *JsonUtils::Json2Condition(nlohmann::json json)
{
    string type = json["type"];

    if (type == "ConditionLogical")
    {
        return Json2ConditionLogical(json);
    }
    else if (type == "ConditionStringState")
    {
        return Json2ConditionStringState(json);
    }
    else if (type == "ConditionNumericState")
    {
        return Json2ConditionNumericState(json);
    }
    else if (type == "ConditionTime")
    {
        return Json2ConditionTime(json);
    }
    else if (type == "ConditionTrigger")
    {
        return Json2ConditionTrigger(json);
    }
    else
    {
        return nullptr;
    }
}

ConditionLogical *JsonUtils::Json2ConditionLogical(nlohmann::json json)
{
    string alias = json["alias"];
    string operation = json["operation"];
    vector<Condition *> conditions;

    for (auto &condition : json["conditions"])
    {
        conditions.push_back(Json2Condition(condition));
    }

    return new ConditionLogical(alias, GetLogicalOperator(operation), conditions);
}

ConditionStringState *JsonUtils::Json2ConditionStringState(nlohmann::json json)
{
    string alias = json["alias"];
    string attribute = json["attribute"];
    time_t for_ = json["for_"];
    string value = json["value"];

    return new ConditionStringState(alias, attribute, for_, value);
}

ConditionNumericState *JsonUtils::Json2ConditionNumericState(nlohmann::json json)
{
    string alias = json["alias"];
    string attribute = json["attribute"];
    time_t for_ = json["for_"];
    double above = json["above"];
    double below = json["below"];

    return new ConditionNumericState(alias, attribute, for_, above, below);
}

ConditionTime *JsonUtils::Json2ConditionTime(nlohmann::json json)
{
    string alias = json["alias"];
    time_t after = json["after"];
    time_t before = json["before"];
    vector<string> weekday = json.at("weekday").get<vector<string>>();

    return new ConditionTime(alias, *localtime(&after), *localtime(&before), weekday);
}

ConditionTrigger *JsonUtils::Json2ConditionTrigger(nlohmann::json json)
{
    string alias = json.at("alias").get<string>();
    string trigger_alias = json.at("trigger_alias").get<string>();

    return new ConditionTrigger(alias, trigger_alias);
}

Trigger *JsonUtils::Json2Trigger(nlohmann::json json)
{
    string type = json["type"];

    if (type == "TriggerNumericState")
    {
        return Json2TriggerNumericState(json);
    }
    else if (type == "TriggerStringState")
    {
        return Json2TriggerStringState(json);
    }
    else if (type == "TriggerTime")
    {
        return Json2TriggerTime(json);
    }
    else
    {
        return nullptr;
    }
}

TriggerNumericState *JsonUtils::Json2TriggerNumericState(nlohmann::json json)
{
    string alias = json["alias"];
    string attribute = json["attribute"];
    time_t for_s = json["for_s"];
    double above = json["above"];
    double below = json["below"];

    return new TriggerNumericState(alias, attribute, for_s, above, below);
}

TriggerStringState *JsonUtils::Json2TriggerStringState(nlohmann::json json)
{
    string alias = json["alias"];
    string attribute = json["attribute"];
    time_t for_s = json["for_s"];
    string from_state = json["from_state"];
    string to_state = json["to_state"];

    return new TriggerStringState(alias, attribute, for_s, from_state, to_state);
}

TriggerTime *JsonUtils::Json2TriggerTime(nlohmann::json json)
{
    string alias = json["alias"];
    string pattern = json["pattern"];
    bool one_time = json["one_time"];

    return new TriggerTime(alias, pattern, one_time);
}

Action *JsonUtils::Json2Action(nlohmann::json json)
{
    string type = json["type"];

    if (type == "ActionCallService")
    {
        return Json2ActionCallService(json);
    }
    else if (type == "ActionDelay")
    {
        return Json2ActionDelay(json);
    }
    else
    {
        return nullptr;
    }
}

ActionCallService *JsonUtils::Json2ActionCallService(nlohmann::json json)
{
    string alias = json["alias"];
    string service = json["service"];
    string service_data = json["service_data"];

    return new ActionCallService(alias, service, service_data);
}

ActionDelay *JsonUtils::Json2ActionDelay(nlohmann::json json)
{
    string alias = json["alias"];
    time_t for_s = json["for_s"];

    return new ActionDelay(alias, for_s);
}