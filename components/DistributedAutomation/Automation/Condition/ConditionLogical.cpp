//
// Created by talgarr on 01/02/24.
//

#include "ConditionLogical.hpp"

bool ConditionLogical::Verify(string trigger_alias)
{
    switch (this->logicalOperator)
    {
    case AND:
        for (auto &condition : this->conditions)
        {
            if (!condition->Verify(trigger_alias))
            {
                return false;
            }
        }
        return true;
    case OR:
        for (auto &condition : this->conditions)
        {
            if (condition->Verify(trigger_alias))
            {
                return true;
            }
        }
        return false;
    case NOT:
        for (auto &condition : this->conditions)
        {
            if (!condition->Verify(trigger_alias))
            {
                return true;
            }
        }
        return false;
    }
    return false;
}

ConditionLogical::ConditionLogical(string alias, LogicalOperator logicalOperator, vector<Condition *> conditions) : Condition(alias)
{
    this->logicalOperator = logicalOperator;
    this->conditions = std::move(conditions);
}

ConditionLogical::~ConditionLogical()
{
    for (auto &condition : this->conditions)
    {
        delete condition;
    }
    this->conditions.clear();
};

static ConditionLogical *ConditionLogical::Json2Condition(nlohmann::json json)
{
    string alias = json["alias"];
    LogicalOperator logicalOperator = json["logicalOperator"];
    vector<Condition *> conditions;
    for (auto &condition : json["conditions"])
    {
        conditions.push_back(Condition::Json2Condition(condition.dump()));
    }
    return new ConditionLogical(alias, logicalOperator, conditions);
}