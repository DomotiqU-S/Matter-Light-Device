//
// Created by talgarr on 01/02/24.
//

#include "Trigger.hpp"

Trigger::Trigger(string alias)
{
    this->alias = std::move(alias);
    this->has_triggered = false;
}

void Trigger::IO(string attribute, string value)
{
    return;
}

bool Trigger::HasTriggered()
{
    if (this->has_triggered)
    {
        this->has_triggered = false;
        return true;
    }
    return false;
}

string Trigger::GetAlias()
{
    return this->alias;
}

static Trigger *Trigger::Json2Trigger(nlohmann::json json)
{
    string type = json["type"];

    switch (type)
    {
    case "TriggerNumericState":
        return TriggerNumericState::Json2Trigger(json);
    case "TriggerStringState":
        return TriggerStringState::Json2Trigger(json);
    case "TriggerTime":
        return TriggerTime::Json2Trigger(json);
    default:
        return nullptr;
    }
}
