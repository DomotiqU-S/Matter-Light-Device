//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_CONDITION_HPP
#define CPP_CONDITION_HPP

#include <string>
#include "json.hpp"

using namespace std;

class Condition
{
protected:
    string alias;

public:
    explicit Condition(string alias);
    virtual ~Condition();
    virtual bool Verify(string trigger_alias);

    static Condition *Json2Condition(nlohmann::json json);
};

#endif // CPP_CONDITION_HPP
