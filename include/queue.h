#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"
#include <stdexcept>

template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* frontNode;
    Node* rearNode;
    size_t queueSize;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), queueSize(0) {}

    ~Queue() {
        clear();
    }

    void enqueue(const T& val) {
        Node* newNode = new Node(val);
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        queueSize++;
    }

    void dequeue() {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }
        Node* temp = frontNode;
        frontNode = frontNode->next;
        if (frontNode == nullptr) {
            rearNode = nullptr;
        }
        delete temp;
        queueSize--;
    }

    T front() const {
        if (isEmpty()) {
            throw std::underflow_error("Queue is empty");
        }
        return frontNode->data;
    }

    bool isEmpty() const {
        return frontNode == nullptr;
    }

    size_t size() const {
        return queueSize;
    }

    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

#endif // QUEUE_H