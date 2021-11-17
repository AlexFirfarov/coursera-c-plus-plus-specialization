#include "node.h"

bool EmptyNode::Evaluate(const Date& date, const std::string& event) const {
    return true;
}

LogicalOperationNode::LogicalOperationNode(const LogicalOperation& logical_operation, 
                         const std::shared_ptr<Node>& left_node, 
                         const std::shared_ptr<Node>& right_node) :
                         logical_operation(logical_operation), left_node(left_node), right_node(right_node) {}

bool LogicalOperationNode::Evaluate(const Date& date, const std::string& event) const {
    if (logical_operation == LogicalOperation::Or) {
        return left_node->Evaluate(date, event) || right_node->Evaluate(date, event);
    }
    return left_node->Evaluate(date, event) && right_node->Evaluate(date, event);
}

DateComparisonNode::DateComparisonNode(const Comparison& cmp, const Date& date) : cmp(cmp), date(date) {}

bool DateComparisonNode::Evaluate(const Date& date, const std::string& event) const {
    switch (cmp) {
    case Comparison::Less:
        return date < this->date;
    case Comparison::LessOrEqual:
        return date <= this->date;
    case Comparison::Greater:
        return date > this->date;
    case Comparison::GreaterOrEqual:
        return date >= this->date;
    case Comparison::Equal:
        return date == this->date;
    case Comparison::NotEqual:
        return date != this->date;
    default:
        throw std::logic_error("Unknown comparison token\n");
    }
}

EventComparisonNode::EventComparisonNode(const Comparison& cmp, const std::string& event) : cmp(cmp), event(event) {}

bool EventComparisonNode::Evaluate(const Date& date, const std::string& event) const {
    switch (cmp) {
    case Comparison::Less:
        return event < this->event;
    case Comparison::LessOrEqual:
        return event <= this->event;
    case Comparison::Greater:
        return event > this->event;
    case Comparison::GreaterOrEqual:
        return event >= this->event;
    case Comparison::Equal:
        return event == this->event;
    case Comparison::NotEqual:
        return event != this->event;
    default:
        throw std::logic_error("Unknown comparison token\n");
    }      
}