#include "TextEditor.h"
#include <string>

struct Node {
    char c;
    Node* prev;
    Node* next;
    Node(char ch) : c(ch), prev(nullptr), next(nullptr) {}
};

class TextEditorConcrete : public TextEditor {
private:
    Node* head;     // start of text
    Node* tail;     // end of text
    Node* cursor;   // cursor is **between cursor->prev and cursor**

public:
    TextEditorConcrete() {
        head = nullptr;
        tail = nullptr;
        cursor = nullptr; // cursor at beginning (before head)
    }

    ~TextEditorConcrete() {
        Node* temp = head;
        while (temp) {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
    }

    void insertChar(char ch) override {
        Node* newNode = new Node(ch);
        if (!head) {
            // first character
            head = tail = newNode;
            cursor = newNode->next; // still points after inserted node
        }
        else if (cursor == head) {
            // insert at beginning
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        else if (!cursor) {
            // insert at end
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        else {
            // insert in middle
            Node* prevNode = cursor->prev;
            prevNode->next = newNode;
            newNode->prev = prevNode;
            newNode->next = cursor;
            cursor->prev = newNode;
        }
    }

    void deleteChar() override {
        if (!head || cursor == head) return; // nothing before cursor

        Node* delNode = cursor ? cursor->prev : tail;

        if (delNode->prev) delNode->prev->next = delNode->next;
        else head = delNode->next; // deleting head

        if (delNode->next) delNode->next->prev = delNode->prev;
        else tail = delNode->prev; // deleting tail

        delete delNode;
    }

    void moveLeft() override {
        if (!head) return;
        if (!cursor) cursor = tail;       // if at end, move to last
        else cursor = cursor->prev;       // move left
    }

    void moveRight() override {
        if (!head) return;
        if (!cursor) return;              // already at end
        cursor = cursor->next;            // move right
    }

    std::string getTextWithCursor() const override {
        std::string s;
        Node* temp = head;
        while (temp) {
            if (temp == cursor) s += '|';
            s += temp->c;
            temp = temp->next;
        }
        if (!cursor) s += '|'; // cursor at end
        return s;
    }
};
