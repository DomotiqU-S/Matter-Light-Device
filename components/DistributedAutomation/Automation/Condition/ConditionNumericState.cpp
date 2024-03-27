//
// Created by talgarr on 01/02/24.
//

#include "ConditionNumericState.hpp"

ConditionNumericState::ConditionNumericState(string alias, string attribute, time_t for_, double above,
                                             double below) : ConditionState(std::move(alias), std::move(attribute), for_)
{
    this->above = above;
    this->below = below;
}

bool ConditionNumericState::Verify(string trigger_alias)
{
    State state_ = DistributedDevice::Instance().GetAttribute(this->attribute);
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

static ConditionNumericState *ConditionNumericState::Json2Condition(nlohmann::json json)
{
    string alias = json["alias"];
    string attribute = json["attribute"];
    time_t for_ = json["for"];
    double above = json["above"];
    double below = json["below"];
    return new ConditionNumericState(alias, attribute, for_, above, below);
}