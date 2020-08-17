#pragma once
#include "Library.h"
#include "ListNode.h"

template<typename T>
class CacheLinkedList {

private:

    ListNode<T>* root;
    size_t currentSize;

public:

    CacheLinkedList() {
        root = new ListNode<T>(NULL,T(),NULL);
        root->next = root;
        root->prev = root;
        currentSize = 0;
    }

    ~CacheLinkedList() {
        clear();
        delete root;
    }

    ListNode<T>* nullIfRoot(ListNode<T>* node) {
        return node == root ? NULL : node;
    }

    void clear() {
        ListNode<T>* last = getLast();
        while (last != NULL) {
            remove(last);
            last = getLast();
        }
    }

    ListNode<T>* getFirst() {
        return nullIfRoot(root->next);
    }

    ListNode<T>* getLast() {
        return nullIfRoot(root->prev);
    }

    ListNode<T>* add(T value) {
        ListNode<T>* toAdd = new ListNode<T>(root->prev,value,root);
        root->prev->next = toAdd;
        root->prev = toAdd;
        currentSize++;
        return toAdd;
    }

    T remove(ListNode<T>* node) {
        assert(node != NULL);
        currentSize--;
        node->prev->next = node->next;
        node->next->prev = node->prev;
        T removed = node->value;
        delete node;
        return removed;
    }

    size_t size() {
        return currentSize;
    }

};