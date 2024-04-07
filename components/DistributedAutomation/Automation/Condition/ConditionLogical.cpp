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
    case INVALID:
        return false;
    }

    return false;
}

ConditionLogical::ConditionLogical(LogicalOperator logicalOperator, vector<Condition *> conditions) : Condition()
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

string ConditionLogical::Print()
{
    string result = "(";
    result += GetLogicalOperatorString(this->logicalOperator);
    for (auto &condition : this->conditions)
    {
        result += condition->Print();
        result += ",";
    }
    result.pop_back();
    result += ")";
    return result;
}