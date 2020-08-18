#include "Library.h"
#include "StackFrame.h"

StackFrame::StackFrame(int yPos, int xPos, int height, int width, StackFrame* parent, int quadrant){
    this->height = height;
    this->width = width;
    this->xPos = xPos;
    this->yPos = yPos;
    this->parent = parent;
    this->quadrant = quadrant;
}

const int StackFrame::size() {
    return height * width;
}

const bool StackFrame::contains(int r, int c) {
    return r >= yPos && r < yPos + height && c >= xPos && c < xPos + width;
}

const bool StackFrame::operator==(const StackFrame& other) {
    return (yPos == other.yPos) && (xPos == other.xPos) && (height == other.height) && (width == other.width);
}

const bool StackFrame::operator!=(const StackFrame& other) {
    return !(*this==other);
}

int StackFrame::getQuadrant(int r, int c) {
    return (r >= yPos + nHeight() ? 2 : 0) + (c >= xPos + nWidth() ? 1 : 0);
}

StackFrame StackFrame::getQuadrantFrame(int quadrant) {
    StackFrame returned(-1,-1,-1,-1,this,quadrant);
    switch (quadrant) {
    case 0:
        returned.yPos = yPos;
        returned.xPos = xPos;
        returned.height = nHeight();
        returned.width = nWidth();
        break;
    case 1:
        returned.yPos = yPos;
        returned.xPos = newX();
        returned.height = nHeight();
        returned.width = wDif();
        break;
    case 2:
        returned.yPos = newY();
        returned.xPos = xPos;
        returned.height = hDif();
        returned.width = nWidth();
        break;
    case 3:
        returned.yPos = newY();
        returned.xPos = newX();
        returned.height = hDif();
        returned.width = wDif();
        break;
    }
    if (returned.size() == 0) {
        return nullFrame;
    }
    return returned;;
}

void StackFrame::pushFrame(list<StackFrame>& stack) {
    StackFrame parent = stack.back();
    stack.pop_back();
    for (int q = 3; q > -1; q--) {
        StackFrame toAdd = parent.getQuadrantFrame(q);
        if (toAdd != nullFrame) {
            stack.push_back(toAdd);
        }
    }
}

const StackFrame StackFrame::getChildContaining(int r, int c) {
    return getQuadrantFrame(getQuadrant(r, c));
}

list<StackFrame>* StackFrame::getChildrenInRange(int start, int end) {
    list<StackFrame>* stack = new list<StackFrame>();
    for (int q = start; q <= end; q++) {
        StackFrame toAdd = getQuadrantFrame(q);
        if (toAdd != nullFrame) {
            stack->push_back(toAdd);
        }
    }
    return stack;
}

list<StackFrame>* StackFrame::getChildren() {
    return getChildrenInRange(0, 3);
}

list<StackFrame>* StackFrame::getChildrenBefore(int r, int c) {
    return getChildrenInRange(0, getQuadrant(r, c) - 1);
}

list<StackFrame>* StackFrame::getChildrenAfter(int r, int c) {
    return getChildrenInRange(getQuadrant(r, c) + 1, 3);
}

const string StackFrame::toString() {
    return "(" + to_string(yPos) + ", " + to_string(xPos) + ", " + to_string(height) + ", " + to_string(width) + ")";
}