//
// Created by talgarr on 01/02/24.
//

#include "ConditionState.hpp"

ConditionState::ConditionState(string attribute, time_t for_) : Condition()
{
    this->attribute = std::move(attribute);
    this->for_ = for_;
}

bool ConditionState::Verify_(const State &state_) const
{
    if (this->for_ > 0)
    {
        time_t now = time(nullptr);
        if (now - state_.time >= this->for_)
        {
            return true;
        }
    }
    else
    {
        return true;
    }
    return false;
}

bool ConditionState::Verify(string trigger_alias)
{
    return false;
}

ConditionState::~ConditionState() = default;

std::string ConditionState::Print()
{
    return "ConditionState: " + this->attribute;
}