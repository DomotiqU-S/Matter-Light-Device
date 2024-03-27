//
// Created by talgarr on 12/02/24.
//

#include "ConditionStringState.hpp"

ConditionStringState::ConditionStringState(string alias, string attribute, time_t for_, string value) : ConditionState(std::move(alias), std::move(attribute), for_)
{
    this->value = std::move(value);
}

bool ConditionStringState::Verify(string trigger_alias)
{
    State state_ = DistributedDevice::Instance().GetAttribute(this->attribute);
    if (state_.value == this->value)
    {
        return this->Verify_(state_);
    }
    return false;
}

ConditionStringState::~ConditionStringState() = default;
