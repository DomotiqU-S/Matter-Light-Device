#include "JsonUtils.hpp"

nlohmann::json JsonUtils::MinimizedJson2Json(nlohmann::json json)
{
    nlohmann::json expanded_json = nlohmann::json::object();
    expanded_json["alias"] = json[0];
    expanded_json["description"] = json[1];
    expanded_json["triggers"] = nlohmann::json::array();
    for (auto &trigger : json[2])
        TriggerMinimizedJson2Json(trigger, &expanded_json["triggers"]);

    expanded_json["conditions"] = nlohmann::json::array();
    for (auto &condition : json[3])
        ConditionMinimizedJson2Json(condition, &expanded_json["conditions"]);

    expanded_json["actions"] = nlohmann::json::array();
    for (auto &action : json[4])
        ActionMinimizedJson2Json(action, &expanded_json["actions"]);

    return expanded_json;
}

nlohmann::json JsonUtils::Json2MinimizedJson(nlohmann::json json)
{
    nlohmann::json minimized_json = nlohmann::json::array();
    minimized_json.push_back(json["alias"]);
    minimized_json.push_back(json["description"]);
    minimized_json.push_back(nlohmann::json::array());
    for (auto &trigger : json["triggers"])
        TriggerJson2MinimizedJson(trigger, &minimized_json[2]);

    minimized_json.push_back(nlohmann::json::array());
    for (auto &condition : json["conditions"])
        ConditionJson2MinimizedJson(condition, &minimized_json[3]);

    minimized_json.push_back(nlohmann::json::array());
    for (auto &action : json["actions"])
        ActionJson2MinimizedJson(action, &minimized_json[4]);

    return minimized_json;
}

void JsonUtils::TriggerJson2MinimizedJson(nlohmann::json trigger, nlohmann::json *minimized_json)
{
    auto temp = nlohmann::json::array();
    temp.push_back(trigger["alias"]);
    if (trigger["type"] == "TriggerTime")
    {
        temp.push_back(TriggerType::TriggerTime);
        temp.push_back(trigger["pattern"]);
        temp.push_back(trigger["one_time"]);
    }
    else if (trigger["type"] == "TriggerNumericState")
    {
        temp.push_back(TriggerType::TriggerNumericState);
        temp.push_back(trigger["attribute"]);
        temp.push_back(trigger["for_s"]);
        temp.push_back(trigger["above"]);
        temp.push_back(trigger["below"]);
    }
    else if (trigger["type"] == "TriggerStringState")
    {
        temp.push_back(TriggerType::TriggerStringState);
        temp.push_back(trigger["attribute"]);
        temp.push_back(trigger["for_s"]);
        temp.push_back(trigger["from_state"]);
        temp.push_back(trigger["to_state"]);
    }
    minimized_json->push_back(temp);
}

void JsonUtils::TriggerMinimizedJson2Json(nlohmann::json trigger, nlohmann::json *expanded_json)
{
    nlohmann::json expanded_trigger = nlohmann::json::object();
    expanded_trigger["alias"] = trigger[0];

    if (trigger[1] == TriggerType::TriggerTime)
    {
        expanded_trigger["type"] = "TriggerTime";
        expanded_trigger["pattern"] = trigger[2];
        expanded_trigger["one_time"] = trigger[3];
    }
    else if (trigger[1] == TriggerType::TriggerNumericState)
    {
        expanded_trigger["type"] = "TriggerNumericState";
        expanded_trigger["attribute"] = trigger[2];
        expanded_trigger["for_s"] = trigger[3];
        expanded_trigger["above"] = trigger[4];
        expanded_trigger["below"] = trigger[5];
    }
    else if (trigger[1] == TriggerType::TriggerStringState)
    {
        expanded_trigger["type"] = "TriggerStringState";
        expanded_trigger["attribute"] = trigger[2];
        expanded_trigger["for_s"] = trigger[3];
        expanded_trigger["from_state"] = trigger[4];
        expanded_trigger["to_state"] = trigger[5];
    }
    expanded_json->push_back(expanded_trigger);
}

void JsonUtils::ConditionJson2MinimizedJson(nlohmann::json condition, nlohmann::json *minimized_json)
{
    nlohmann::json minimized_condition = nlohmann::json::array();

    if (condition["type"] == "ConditionLogical")
    {
        minimized_condition.push_back(ConditionType::ConditionLogical);
        minimized_condition.push_back(condition["logical_operator"]);
        minimized_condition.push_back(nlohmann::json::array());
        for (auto &condition : condition["conditions"])
            ConditionJson2MinimizedJson(condition, &minimized_condition[2]);
    }
    else if (condition["type"] == "ConditionStringState")
    {
        minimized_condition.push_back(ConditionType::ConditionStringState);
        minimized_condition.push_back(condition["attribute"]);
        minimized_condition.push_back(condition["for_"]);
        minimized_condition.push_back(condition["value"]);
    }
    else if (condition["type"] == "ConditionNumericState")
    {
        minimized_condition.push_back(ConditionType::ConditionNumericState);
        minimized_condition.push_back(condition["attribute"]);
        minimized_condition.push_back(condition["for_"]);
        minimized_condition.push_back(condition["above"]);
        minimized_condition.push_back(condition["below"]);
    }
    else if (condition["type"] == "ConditionTime")
    {
        minimized_condition.push_back(ConditionType::ConditionTime);
        minimized_condition.push_back(condition["pattern"]);
        minimized_condition.push_back(condition["after"]);
        minimized_condition.push_back(condition["before"]);
        minimized_condition.push_back(condition["weekdays"]);
    }
    else if (condition["type"] == "ConditionTrigger")
    {
        minimized_condition.push_back(ConditionType::ConditionTrigger);
        minimized_condition.push_back(condition["trigger_alias"]);
    }
    else
    {
        return;
    }

    minimized_json->push_back(minimized_condition);
}

void JsonUtils::ConditionMinimizedJson2Json(nlohmann::json condition, nlohmann::json *expanded_json)
{
    nlohmann::json expanded_condition = nlohmann::json::object();
    if (condition[0] == ConditionType::ConditionLogical)
    {
        expanded_condition["type"] = "ConditionLogical";
        expanded_condition["logical_operator"] = condition[1];
        expanded_condition["conditions"] = nlohmann::json::array();
        for (auto &condition : condition[2])
            ConditionMinimizedJson2Json(condition, &expanded_condition["conditions"]);
    }
    else if (condition[0] == ConditionType::ConditionStringState)
    {
        expanded_condition["type"] = "ConditionStringState";
        expanded_condition["attribute"] = condition[1];
        expanded_condition["for_"] = condition[2];
        expanded_condition["value"] = condition[3];
    }
    else if (condition[0] == ConditionType::ConditionNumericState)
    {
        expanded_condition["type"] = "ConditionNumericState";
        expanded_condition["attribute"] = condition[1];
        expanded_condition["for_"] = condition[2];
        expanded_condition["above"] = condition[3];
        expanded_condition["below"] = condition[4];
    }
    else if (condition[0] == ConditionType::ConditionTime)
    {
        expanded_condition["type"] = "ConditionTime";
        expanded_condition["pattern"] = condition[1];
        expanded_condition["after"] = condition[2];
        expanded_condition["before"] = condition[3];
        expanded_condition["weekdays"] = condition[4];
    }
    else if (condition[0] == ConditionType::ConditionTrigger)
    {
        expanded_condition["type"] = "ConditionTrigger";
        expanded_condition["trigger_alias"] = condition[1];
    }
    else
    {
        return;
    }

    expanded_json->push_back(expanded_condition);
}

void JsonUtils::ActionJson2MinimizedJson(nlohmann::json action, nlohmann::json *minimized_json)
{
    nlohmann::json minimized_action = nlohmann::json::array();

    if (action["type"] == "ActionCallService")
    {
        minimized_action.push_back(ActionType::ActionCallService);
        minimized_action.push_back(action["service"]);
        minimized_action.push_back(action["service_data"]);
    }
    else if (action["type"] == "ActionDelay")
    {
        minimized_action.push_back(ActionType::ActionDelay);
        minimized_action.push_back(action["for_s"]);
    }
    else
    {
        return;
    }

    minimized_json->push_back(minimized_action);
}

void JsonUtils::ActionMinimizedJson2Json(nlohmann::json action, nlohmann::json *expanded_json)
{
    nlohmann::json expanded_action = nlohmann::json::object();

    if (action[0] == ActionType::ActionCallService)
    {
        expanded_action["type"] = "ActionCallService";
        expanded_action["service"] = action[1];
        expanded_action["service_data"] = action[2];
    }
    else if (action[0] == ActionType::ActionDelay)
    {
        expanded_action["type"] = "ActionDelay";
        expanded_action["for_s"] = action[1];
    }
    else
    {
        return;
    }

    expanded_json->push_back(expanded_action);
}

Automation *JsonUtils::Json2Automation(nlohmann::json json)
{
    auto alias = json["alias"].get<string>();
    auto description = json["description"].get<string>();
    auto triggers = new vector<Trigger *>();
    auto conditions = new vector<Condition *>();
    auto actions = new vector<Action *>();

    for (auto &trigger : json["triggers"])
    {
        triggers->push_back(Json2Trigger(trigger));
    }

    for (auto &condition : json["conditions"])
    {
        conditions->push_back(Json2Condition(condition));
    }

    for (auto &action : json["actions"])
    {
        actions->push_back(Json2Action(action));
    }

    return new Automation(alias, description, *triggers, *conditions, *actions);
}

Condition *JsonUtils::Json2Condition(nlohmann::json json)
{
    string type = json["type"];

    if (type == "ConditionLogical")
    {
        ESP_LOGE(TAG_JSON_UTILS, "Json2ConditionLogical");
        string operation = json["operation"];
        vector<Condition *> conditions;

        for (auto &condition : json["conditions"])
        {
            conditions.push_back(Json2Condition(condition));
        }

        return new ConditionLogical(GetLogicalOperator(operation), conditions);
    }
    else if (type == "ConditionStringState")
    {
        string attribute = json["attribute"];
        time_t for_ = json["for_"];
        string value = json["value"];

        return new ConditionStringState(attribute, for_, value);
    }
    else if (type == "ConditionNumericState")
    {
        string attribute = json["attribute"];
        time_t for_ = json["for_"];
        double above = json["above"];
        double below = json["below"];

        return new ConditionNumericState(attribute, for_, above, below);
    }
    else if (type == "ConditionTime")
    {
        time_t after = json["after"];
        time_t before = json["before"];
        vector<string> weekday = json.at("weekday").get<vector<string>>();

        return new ConditionTime(*localtime(&after), *localtime(&before), weekday);
    }
    else if (type == "ConditionTrigger")
    {
        string trigger_alias = json.at("trigger_alias").get<string>();

        return new ConditionTrigger(trigger_alias);
    }
    else
    {
        return nullptr;
    }
}

Trigger *JsonUtils::Json2Trigger(nlohmann::json json)
{
    ESP_LOGE(TAG_JSON_UTILS, "Json2Trigger %s", json.dump().c_str());
    string type = json["type"];

    if (type == "TriggerNumericState")
    {
        string alias = json["alias"];
        string attribute = json["attribute"];
        time_t for_s = json["for_s"];
        double above = json["above"];
        double below = json["below"];

        return new TriggerNumericState(alias, attribute, for_s, above, below);
    }
    else if (type == "TriggerStringState")
    {
        string alias = json["alias"];
        string attribute = json["attribute"];
        time_t for_s = json["for_s"];
        string from_state = json["from_state"];
        string to_state = json["to_state"];

        return new TriggerStringState(alias, attribute, for_s, from_state, to_state);
    }
    else if (type == "TriggerTime")
    {
        string alias = json["alias"];
        string pattern = json["pattern"];
        bool one_time = json["one_time"];

        return new TriggerTime(alias, pattern, one_time);
    }
    else
    {
        return nullptr;
    }
}

Action *JsonUtils::Json2Action(nlohmann::json json)
{
    string type = json["type"];

    if (type == "ActionCallService")
    {
        string service = json["service"];
        string service_data = json["service_data"];

        return new ActionCallService(service, service_data);
    }
    else if (type == "ActionDelay")
    {
        time_t for_s = json["for_s"];

        return new ActionDelay(for_s);
    }
    else
    {
        return nullptr;
    }
}
