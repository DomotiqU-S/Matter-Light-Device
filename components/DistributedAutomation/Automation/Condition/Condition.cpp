//
// Created by talgarr on 01/02/24.
//

#include "Condition.hpp"

Condition::Condition(string alias)
{
    this->alias = alias;
}

bool Condition::Verify(string trigger_alias)
{
    return false;
}

Condition::~Condition() = default;

static Condition *Condition::Json2Condition(nlohmann::json json)
{
    string type = json["type"];
    switch (type)
    {
    case "ConditionTrigger":
        return ConditionTrigger::Json2Condition(json);
    case "ConditionNumericState":
        return ConditionNumericState::Json2Condition(json);
    case "ConditionStringState":
        return ConditionStringState::Json2Condition(json);
    case "ConditionLogical":
        return ConditionLogical::Json2Condition(json);
    case "ConditionTimeState":
        return ConditionTimeState::Json2Condition(json);
    default:
        return nullptr;
    }
}