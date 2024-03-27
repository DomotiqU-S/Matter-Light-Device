//
// Created by talgarr on 01/02/24.
//

#ifndef CPP_CONDITIONLOGICAL_HPP
#define CPP_CONDITIONLOGICAL_HPP

#include <vector>
#include "Condition.hpp"
#include <utility>
#include "json.hpp"

enum LogicalOperator
{
    AND,
    OR,
    NOT
};

class ConditionLogical : public Condition
{
protected:
    LogicalOperator logicalOperator;
    vector<Condition *> conditions;

public:
    explicit ConditionLogical(string alias, LogicalOperator logicalOperator, vector<Condition *> conditions);
    ~ConditionLogical() override;
    bool Verify(string trigger_alias) override;

    static ConditionLogical *Json2Condition(nlohmann::json json);
};

#endif // CPP_CONDITIONLOGICAL_HPP
