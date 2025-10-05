#include "texteditor.h"
#include <stack>
#include <string>
#include <map>
#include <stdexcept>
#include <iostream>
#include <algorithm>

using namespace std;

   //Internal structure that stores what’s to the left and right of the cursor in two stacks

struct EditorState {
    stack<char> before;   // chars on the left of the cursor
    stack<char> after;    // chars on the right of the cursor
};

// Each TextEditor object is linked to a separate EditorState.
// We keep them in a global map so that no members are needed
// inside the TextEditor class itself.
static map<const TextEditor*, EditorState*> stateTable;

/*
   Helper to obtain the state for a given editor.
   If it doesn’t exist yet, allocate one.
*/
static EditorState* fetchState(const TextEditor* ed) {
    if (stateTable.find(ed) == stateTable.end()) {
        stateTable[ed] = new EditorState();
    }
    return stateTable[ed];
}

// Insert a printable character at the current cursor position.
// Characters are only pushed to the "before" stack.
void TextEditor::insertChar(char c) {
    if (c < 32 || c > 126) {
        throw invalid_argument("Character must be printable ASCII");
    }

    EditorState* st = fetchState(this);
    st->before.push(c);
}

// Remove the character that appears directly before the cursor.
// If cursor is already at the start, do nothing.
void TextEditor::deleteChar() {
    EditorState* st = fetchState(this);
    if (!st->before.empty()) {
        st->before.pop();
    }
}

// Move the cursor one position left.
// If not possible, cursor stays at the beginning.
void TextEditor::moveLeft() {
    EditorState* st = fetchState(this);
    if (!st->before.empty()) {
        st->after.push(st->before.top());
        st->before.pop();
    }
}

// Move the cursor one position right.
// If not possible, cursor stays at the end.
void TextEditor::moveRight() {
    EditorState* st = fetchState(this);
    if (!st->after.empty()) {
        st->before.push(st->after.top());
        st->after.pop();
    }
}

// Build and return the text including a '|' marker for cursor.
// We reverse the "before" stack for correct order, then
// append the "after" stack in normal order.
string TextEditor::getTextWithCursor() const {
    EditorState* st = fetchState(this);

    string leftPart;
    stack<char> tempLeft = st->before;
    while (!tempLeft.empty()) {
        leftPart.push_back(tempLeft.top());
        tempLeft.pop();
    }
    reverse(leftPart.begin(), leftPart.end());

    string rightPart;
    stack<char> tempRight = st->after;
    while (!tempRight.empty()) {
        rightPart.push_back(tempRight.top());
        tempRight.pop();
    }

    return leftPart + "|" + rightPart;
}
