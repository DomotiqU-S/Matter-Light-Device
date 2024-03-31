#include "StateCollection.hpp"

State StateCollection::GetAttribute(const std::string &attribute)
{
    if (IsIn(attribute))
        return this->states[attribute];
    else
        return {};
}

void StateCollection::SetAttribute(const std::string &attribute, State state)
{
    if (IsIn(attribute))
        this->states[attribute] = state;
    else
        this->states.insert({attribute, state});
}

bool StateCollection::IsIn(const std::string &attribute)
{
    return this->states.find(attribute) != this->states.end();
}