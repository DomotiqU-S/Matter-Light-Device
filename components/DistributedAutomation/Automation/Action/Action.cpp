//
// Created by talgarr on 01/02/24.
//

#include "Action.hpp"


Action::Action(std::string alias){
    this->alias = std::move(alias);
}

Action::~Action() = default;



