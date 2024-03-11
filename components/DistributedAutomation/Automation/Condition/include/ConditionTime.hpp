//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_CONDITIONTIME_HPP
#define CPP_CONDITIONTIME_HPP

#include <ctime>
#include "Condition.hpp"
#include <algorithm>
#include <vector>

using namespace std;

class ConditionTime : public Condition
{
protected:
    tm after;
    tm before;
    vector<string> weekday;

public:
    explicit ConditionTime(string alias, tm after, tm before, vector<string> weekday);
    ~ConditionTime() override;
    bool Verify(string trigger_alias) override;
};

#endif // CPP_CONDITIONTIME_HPP
