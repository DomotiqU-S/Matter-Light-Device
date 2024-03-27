//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_CONDITIONNUMERICSTATE_HPP
#define CPP_CONDITIONNUMERICSTATE_HPP

#include "ConditionState.hpp"

class ConditionNumericState : public ConditionState
{
protected:
    double above;
    double below;

public:
    explicit ConditionNumericState(string alias, string attribute, time_t for_, double above, double below);
    ~ConditionNumericState() override;
    bool Verify(string trigger_alias) override;
};

#endif // CPP_CONDITIONNUMERICSTATE_HPP
