#ifndef STATECOLLECTION_HPP
#define STATECOLLECTION_HPP

#include <map>
#include <string>
#include <ctime>
#include "State.hpp"

class StateCollection
{
private:
    std::map<std::string, State> states;

public:
    static StateCollection &Instance()
    {
        static StateCollection instance;
        return instance;
    };

    State GetAttribute(const std::string &attribute);
    void SetAttribute(const std::string &attribute, State state);
    bool IsIn(const std::string &attribute);
};

#endif // STATECOLLECTION_HPP