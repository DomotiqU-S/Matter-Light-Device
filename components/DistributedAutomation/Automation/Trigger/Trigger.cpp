//
// Created by talgarr on 01/02/24.
//

#include "Trigger.hpp"


Trigger::Trigger(string alias) {
    this->alias = std::move(alias);
    this->has_triggered = false;
}

void Trigger::IO(string attribute, string value) {
    return;
}

bool Trigger::HasTriggered() {
    if (this->has_triggered) {
        this->has_triggered = false;
        return true;
    }
    return false;
}

string Trigger::GetAlias() {
    return this->alias;
}



