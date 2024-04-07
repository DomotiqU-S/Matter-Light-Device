#include "LogicalOperator.hpp"

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

std::string GetLogicalOperatorString(LogicalOperator logicalOperator)
{
    switch (logicalOperator)
    {
    case LogicalOperator::AND:
        return "AND";
    case LogicalOperator::OR:
        return "OR";
    case LogicalOperator::NOT:
        return "NOT";
    default:
        return "INVALID";
    }
}