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
