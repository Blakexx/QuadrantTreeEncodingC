#pragma once
#include "Library.h"

template<typename T>
class ListNode {

public:

    ListNode<T>* prev;
    ListNode<T>* next;
    T value;

    ListNode(ListNode<T>* prev, const T& value, ListNode<T>* next) {
        this->prev = prev;
        this->value = value;
        this->next = next;
    }
};