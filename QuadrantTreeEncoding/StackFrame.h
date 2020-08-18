#pragma once
#include <cstddef>
#include <algorithm>
#include <list>

using namespace std;

class StackFrame {

private:
    
    inline int nHeight() {
        return max(1, height / 2);
    }

    inline int hDif() {
        return height - nHeight();
    }

    inline int nWidth() {
        return max(1, width / 2);
    }

    inline int wDif() {
        return width - nWidth();
    }

    inline int newY() {
        return yPos + nHeight();
    }

    inline int newX() {
        return xPos + nWidth();
    }

    list<StackFrame>* getChildrenInRange(int start, int end);

    int getQuadrant(int r, int c);

    StackFrame getQuadrantFrame(int quadrant);

public:

    int height, width, xPos, yPos, quadrant;
    StackFrame* parent;

    StackFrame(int yPos, int xPos, int height, int width, StackFrame* parent = NULL, int quadrant = -1);

    const int size();

    const bool contains(int r, int c);

    const bool operator==(const StackFrame& other);

    const bool operator!=(const StackFrame& other);

    const StackFrame getChildContaining(int r, int c);

    static void pushFrame(list<StackFrame>& stack);

    list<StackFrame>* getChildren();

    list<StackFrame>* getChildrenBefore(int r, int c);

    list<StackFrame>* getChildrenAfter(int r, int c);

    const string toString();
};

static const StackFrame nullFrame(-1,-1,-1,-1);