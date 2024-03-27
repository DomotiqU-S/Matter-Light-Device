//
// Created by talgarr on 01/02/24.
//

#include "ConditionTrigger.hpp"

ConditionTrigger::ConditionTrigger(string alias, string trigger_alias) : Condition(move(alias))
{
    this->trigger_alias = move(trigger_alias);
}

bool ConditionTrigger::Verify(string alias)
{
    return this->trigger_alias == alias;
}

ConditionTrigger::~ConditionTrigger() = default;

static ConditionTrigger *ConditionTrigger::Json2Condition(nlohmann::json json)
{
    string alias = json["alias"];
    string trigger_alias = json["trigger_alias"];
    return new ConditionTrigger(alias, trigger_alias);
}