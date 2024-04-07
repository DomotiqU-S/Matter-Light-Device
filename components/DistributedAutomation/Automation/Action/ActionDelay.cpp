//
// Created by talgarr on 01/02/24.
//

#include "ActionDelay.hpp"

ActionDelay::ActionDelay(time_t delay_s) : Action()
{
    this->delay_s = delay_s;
}

ActionDelay::~ActionDelay() = default;

void ActionDelay::Do()
{
    this_thread::sleep_for(chrono::seconds(this->delay_s));
}

std::string ActionDelay::Print()
{
    std::string str = "ActionDelay: ";
    str += to_string(this->delay_s);
    return str;
}