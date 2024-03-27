//
// Created by talgarr on 01/02/24.
//

#include "TriggerNumericState.hpp"

TriggerNumericState::TriggerNumericState(string alias, string attribute, time_t for_s, double above, double below)
    : TriggerState(std::move(alias), std::move(attribute), for_s)
{

    this->above = above;
    this->below = below;
}

void TriggerNumericState::IO(string attribute, string value)
{
    if (attribute.empty() or value.empty())
        return;

    if (this->attribute == attribute)
    {
        // try {
        float value_f = stof(value);
        if (this->above < value_f and value_f < this->below)
        {
            this->IOSup();
        }
        else
        {
            this->Stop();
        }
        // } catch (exception &e) {
        // }
    }
}

TriggerNumericState::~TriggerNumericState() = default;
