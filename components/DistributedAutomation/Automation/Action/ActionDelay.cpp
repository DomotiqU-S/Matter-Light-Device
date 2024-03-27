//
// Created by talgarr on 01/02/24.
//

#include "ActionDelay.hpp"

ActionDelay::ActionDelay(string alias, time_t delay_s) : Action(std::move(alias))
{
    this->delay_s = delay_s;
}

ActionDelay::~ActionDelay() = default;

void ActionDelay::Do()
{
    this_thread::sleep_for(chrono::seconds(this->delay_s));
}

static ActionDelay *ActionDelay::Json2Action(nlohmann::json json)
{
    string alias = json["alias"];
    time_t delay_s = json["delay_s"];
    return new ActionDelay(alias, delay_s);
}