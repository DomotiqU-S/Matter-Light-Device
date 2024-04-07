//
// Created by talgarr on 12/02/24.
//

#include "ConditionStringState.hpp"

ConditionStringState::ConditionStringState(string attribute, time_t for_, string value) : ConditionState(std::move(attribute), for_)
{
    this->value = std::move(value);
}

bool ConditionStringState::Verify(string trigger_alias)
{
    State state_ = StateCollection::Instance().GetAttribute(this->attribute);
    if (state_.value == this->value)
    {
        return this->Verify_(state_);
    }
    return false;
}

ConditionStringState::~ConditionStringState() = default;

string ConditionStringState::Print()
{
    return "ConditionStringState: " + this->attribute + " " + this->value;
}