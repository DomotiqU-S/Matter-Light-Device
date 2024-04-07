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
public:
    explicit Condition();
    virtual ~Condition();
    virtual bool Verify(string trigger_alias);
    virtual string Print() = 0;
};

#endif // CPP_CONDITION_HPP
