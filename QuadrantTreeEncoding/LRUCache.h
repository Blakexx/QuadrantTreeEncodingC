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
    
    bool remove(ListNode<Item>* toRemove) {
        if (toRemove == NULL) {
            return false;
        }
        lookup.erase(toRemove->value.first);
        controller->remove(toRemove);
        return true;
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
            if (!remove(controller->getFirst())) {
                return;
            }
        }
        else if (val != NULL) {
            controller->remove(val);
        }
        lookup[key] = controller->add(Item(key,value));
    }

    bool remove(K key) {
        return remove(lookup[key]);
    }

    bool contains(K key) {
        return lookup[key] != NULL;
    }

    V getNoCache(K key) {
        ListNode<Item>* val = lookup[key];
        assert(val != NULL);
        return val->value.second;
    }

    V get(K key) {
        improveItem(key);
        return getNoCache(key);
    }

    string toString() {
        string returned = "";
        returned.append("CAPACITY: ");
        returned.append(to_string(capacity));
        returned.append(", [");
        ListNode<Item>* first = controller->getFirst();
        while (first != NULL) {
            returned.append("(");
            returned.append(StringConverter<K>::convert(first->value.first));
            returned.append("=");
            returned.append(StringConverter<V>::convert(first->value.second));
            returned.append(")");
            first = controller->nullIfRoot(first->next);
            if (first != NULL) {
                returned.append(", ");
            }
        }
        returned.append("]");
        return returned;
    }
};