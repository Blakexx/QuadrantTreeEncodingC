#pragma once
#include "Library.h"
#include "StackFrame.h"

template <typename T>
struct StringConverter {

private:

    static const string convert(T original, false_type) {
        return typeid(T).name();
    }
    
    static const string convert(T original, true_type) {
        return to_string(original);
    }

public:

    static const string convert(T original) {
        return convert(original,is_integral<T>());
    }

};

template<typename F, typename S>
struct StringConverter<pair<F, S>> {
    static const string convert(pair<F,S> original) {
        string returned = "{";
        returned.append(StringConverter<F>::convert(original.first));
        returned.append(", ");
        returned.append(StringConverter<S>::convert(original.second));
        returned.append("}");
        return returned;
    }
};

template<>
struct StringConverter<StackFrame> {
    static const string convert(StackFrame original) {
        return original.toString();
    }
};

template<typename T>
struct StringConverter<list<T>> {
    static const string convert(list<T> original) {
        string returned = "[";
        for (auto& elem : original) {
            returned.append(StringConverter<T>::convert(elem));
            if (&elem != &original.back()) {
                returned.append(", ");
            }
        }
        returned.append("]");
        return returned;
    }
};