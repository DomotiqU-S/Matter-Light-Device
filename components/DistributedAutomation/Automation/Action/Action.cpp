//
// Created by talgarr on 01/02/24.
//

#include "Action.hpp"

Action::Action(std::string alias)
{
    this->alias = std::move(alias);
}

Action::~Action() = default;

static Action *Action::Json2Action(nlohmann::json json)
{
    std::string type = json["type"];
    switch (type)
    {
    case "ActionCallService":
        return ActionCallService::Json2Action(json);
    case "ActionDelay":
        return ActionDelay::Json2Action(json);
    default:
        return nullptr;
    }
}