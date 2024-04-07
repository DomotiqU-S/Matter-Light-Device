//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_CONDITIONSTATE_HPP
#define CPP_CONDITIONSTATE_HPP

#include "Condition.hpp"
#include <utility>
#include <string>
#include "State.hpp"

class ConditionState : public Condition
{
protected:
    string attribute;
    string state;
    time_t for_;

    bool Verify_(const State &state_) const;

public:
    explicit ConditionState(string attribute, time_t for_);
    ~ConditionState() override;
    bool Verify(string trigger_alias) override;
    string Print() override;
};

#endif // CPP_CONDITIONSTATE_HPP
