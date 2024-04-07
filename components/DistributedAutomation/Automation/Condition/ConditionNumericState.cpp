//
// Created by talgarr on 01/02/24.
//

#include "ConditionNumericState.hpp"

ConditionNumericState::ConditionNumericState(string attribute, time_t for_, double above,
                                             double below) : ConditionState(std::move(attribute), for_)
{
    this->above = above;
    this->below = below;
}

bool ConditionNumericState::Verify(string trigger_alias)
{
    State state_ = StateCollection::Instance().GetAttribute(this->attribute);
    // try
    // {
    float value = stof(state_.value);
    if (value > this->above && value < this->below)
    {
        return this->Verify_(state_);
    }
    // }
    // catch (std::invalid_argument &e)
    // {
    //     return false;
    // }
    return false;
}

ConditionNumericState::~ConditionNumericState() = default;

std::string ConditionNumericState::Print()
{
    return "ConditionNumericState: " + this->attribute + " " + to_string(this->above) + " " + to_string(this->below);
}