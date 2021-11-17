#include "Common.h"

using namespace std;

class ValueNode : public Expression {
public:
    ValueNode(int value) : value_(value) {}

    int Evaluate() const override {
        return value_;
    }

    std::string ToString() const override {
        return to_string(value_); 
    }

private:
    int value_;
};

class SumNode : public Expression {
public:
    SumNode(ExpressionPtr left, ExpressionPtr right) 
        : left_(move(left)), right_(move(right)) {}
    
    int Evaluate() const override {
        return left_->Evaluate() + right_->Evaluate();
    }

    std::string ToString() const override {
        return '(' + left_->ToString() + ')' + 
               '+' + 
               '(' + right_->ToString() + ')';
    }

private:
    ExpressionPtr left_;
    ExpressionPtr right_;
};

class ProductNode : public Expression {
public:
    ProductNode(ExpressionPtr left, ExpressionPtr right) 
        : left_(move(left)), right_(move(right)) {}
    
    int Evaluate() const override {
        return left_->Evaluate() * right_->Evaluate();
    }

    std::string ToString() const override {
        return '(' + left_->ToString() + ')' + 
               '*' + 
               '(' + right_->ToString() + ')';
    }

private:
    ExpressionPtr left_;
    ExpressionPtr right_;
};

ExpressionPtr Value(int value) {
    return make_unique<ValueNode>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<SumNode>(move(left), move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<ProductNode>(move(left), move(right));
}