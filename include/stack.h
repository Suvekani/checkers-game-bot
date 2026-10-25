#ifndef STACK_H
#define STACK_H

#include "common.h"
#include <stdexcept>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* topNode;
    size_t stackSize;

public:
    Stack() : topNode(nullptr), stackSize(0) {}

    ~Stack() {
        clear();
    }

    void push(const T& val) {
        Node* newNode = new Node(val);
        newNode->next = topNode;
        topNode = newNode;
        stackSize++;
    }

    void pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty");
        }
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        stackSize--;
    }

    T top() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty");
        }
        return topNode->data;
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }

    size_t size() const {
        return stackSize;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
};

#endif // STACK_H
