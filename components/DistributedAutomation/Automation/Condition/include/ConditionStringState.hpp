//
// Created by talgarr on 12/02/24.
//

#ifndef DISTRIBUTED_AUTOMATIONS_CONDITIONSTRINGSTATE_HPP
#define DISTRIBUTED_AUTOMATIONS_CONDITIONSTRINGSTATE_HPP

#include "ConditionState.hpp"
#include <string>
#include "json.hpp"

using namespace std;

class ConditionStringState : public ConditionState
{

protected:
    string value;

public:
    explicit ConditionStringState(string alias, string attribute, time_t for_, string value);
    ~ConditionStringState() override;
    bool Verify(string trigger_alias) override;
};

#endif // DISTRIBUTED_AUTOMATIONS_CONDITIONSTRINGSTATE_HPP
