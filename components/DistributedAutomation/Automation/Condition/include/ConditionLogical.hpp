//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_CONDITIONLOGICAL_HPP
#define CPP_CONDITIONLOGICAL_HPP

#include <vector>
#include "Condition.hpp"
#include <utility>
#include "json.hpp"
#include "LogicalOperator.hpp"

class ConditionLogical : public Condition
{
protected:
    LogicalOperator logicalOperator;
    vector<Condition *> conditions;

public:
    explicit ConditionLogical(LogicalOperator logicalOperator, vector<Condition *> conditions);
    ~ConditionLogical() override;
    bool Verify(string trigger_alias) override;
    string Print() override;
};

#endif // CPP_CONDITIONLOGICAL_HPP
