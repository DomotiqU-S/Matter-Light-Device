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

LogicalOperator GetLogicalOperator(std::string logicalOperator);
std::string GetLogicalOperatorString(LogicalOperator logicalOperator);

#endif // LOGICALOPERATOR_HPP