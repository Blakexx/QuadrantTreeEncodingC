#pragma once
#include "Library.h"
#include "CacheLinkedList.h"

template<typename K, typename V>
class LRUCache {

private:

    using Item = pair<K, V>;

    unordered_map<K,ListNode<Item>*> lookup;
    CacheLinkedList<Item>* controller;
    size_t capacity;
    
    void remove(ListNode<Item>* toRemove) {
        if (toRemove == NULL) {
            return;
        }
        lookup.erase(toRemove->value.first);
        controller->remove(toRemove);
    }

public:
    
    LRUCache(size_t capacity) {
        this->capacity = capacity;
        controller = new CacheLinkedList<Item>();
    }

    ~LRUCache() {
        delete controller;
    }

    void improveItem(K key) {
        ListNode<Item>* val = lookup[key];
        if (val != NULL) {
            val = controller->add(controller->remove(val));
            lookup[key] = val;
        }
    }

    void put(K key, V value) {
        ListNode<Item>* val = lookup[key];
        if (val == NULL && capacity == controller->size()) {
            remove(controller->getFirst());
        }
        else if (val != NULL) {
            controller->remove(val);
        }
        lookup[key] = controller->add(Item(key,value));
    }

    void remove(K key) {
        remove(lookup[key]);
    }

    bool contains(K key) {
        return lookup[key] != NULL;
    }

    V getNoCache(K key) {
        ListNode<Item>* val = lookup[key];
        assert(val != NULL);
        return val->value.first;
    }

    V get(K key) {
        improveItem(key);
        return getNoCache(key);
    }

    string toString(const function<string(Item)>& stringMapper) {
        string returned = "[";
        ListNode<Item>* first = controller->getFirst();
        while (first != NULL) {
            returned.append(stringMapper(first->value));
            first = controller->nullIfRoot(first->next);
            if (first != NULL) {
                returned.append(", ");
            }
        }
        returned.append("]");
        return returned;
    }
};