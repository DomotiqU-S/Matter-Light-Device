#ifndef LOGICALOPERATOR_HPP
#define LOGICALOPERATOR_HPP

#include <string>
enum LogicalOperator
{
    AND,
    OR,
    NOT,
    INVALID
};

LogicalOperator GetLogicalOperator(std::string logicalOperator)
{
    if (logicalOperator == "AND")
    {
        return LogicalOperator::AND;
    }
    else if (logicalOperator == "OR")
    {
        return LogicalOperator::OR;
    }
    else if (logicalOperator == "NOT")
    {
        return LogicalOperator::NOT;
    }
    else
    {
        return LogicalOperator::INVALID;
    }
}

#endif // LOGICALOPERATOR_HPP