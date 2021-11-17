#pragma once

#include "date.h"

#include <string>
#include <memory>

enum class LogicalOperation {
    Or,
    And
};

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

class Node {
public:
    virtual bool Evaluate(const Date& date, const std::string& event) const = 0;
};

class EmptyNode : public Node {
public:  
    bool Evaluate(const Date& date, const std::string& event) const override;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(const LogicalOperation& logical_operation, 
                         const std::shared_ptr<Node>& left_node, 
                         const std::shared_ptr<Node>& right_node);

    bool Evaluate(const Date& date, const std::string& event) const override;

private:
    const LogicalOperation logical_operation;
    const std::shared_ptr<Node> left_node;
    const std::shared_ptr<Node> right_node;
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison& cmp, const Date& date);

    bool Evaluate(const Date& date, const std::string& event) const override;
private: 
    const Comparison cmp;
    const Date date;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison& cmp, const std::string& event);

    bool Evaluate(const Date& date, const std::string& event) const override;

private:
    const Comparison cmp;
    const std::string event;
};