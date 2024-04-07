//
// Created by talgarr on 01/02/24.
//

#include "ConditionTrigger.hpp"

ConditionTrigger::ConditionTrigger(string trigger_alias) : Condition()
{
    this->trigger_alias = move(trigger_alias);
}

bool ConditionTrigger::Verify(string alias)
{
    return this->trigger_alias == alias;
}

ConditionTrigger::~ConditionTrigger() = default;

string ConditionTrigger::Print()
{
    return "ConditionTrigger: " + this->trigger_alias;
}