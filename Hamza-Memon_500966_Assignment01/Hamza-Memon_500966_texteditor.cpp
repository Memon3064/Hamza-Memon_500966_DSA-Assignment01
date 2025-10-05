#include "texteditor.h"
#include <string>
#include <iostream>
using namespace std;

void TextEditor::insertChar(char) {}
void TextEditor::deleteChar() {}
void TextEditor::moveLeft() {}
void TextEditor::moveRight() {}
string TextEditor::getTextWithCursor() const { return ""; }

class TextEditorImpl : public TextEditor {
private:
    string text; 
    int cursor;  

public:
    TextEditorImpl() : cursor(0) {}

    void insertChar(char c) override {
        text.insert(text.begin() + cursor, c);
        cursor++;
    }

    void deleteChar() override {
        if (cursor > 0) {
            text.erase(text.begin() + cursor - 1);
            cursor--;
        }
    }

    void moveLeft() override {
        if (cursor > 0) cursor--;
    }

    void moveRight() override {
        if (cursor < (int)text.size()) cursor++;
    }

    string getTextWithCursor() const override {
        string result = text;
        result.insert(result.begin() + cursor, '|');
        return result;
    }
};

int main() {
    TextEditorImpl ed;

    ed.insertChar('a');
    cout << "After insert 'a': " << ed.getTextWithCursor() << "\n";

    ed.insertChar('b');
    cout << "After insert 'b': " << ed.getTextWithCursor() << "\n";

    ed.moveLeft();
    cout << "After move left: " << ed.getTextWithCursor() << "\n";

    ed.insertChar('c');
    cout << "After insert 'c': " << ed.getTextWithCursor() << "\n";

    ed.deleteChar();
    cout << "After delete: " << ed.getTextWithCursor() << "\n";

    ed.moveLeft();
    ed.moveLeft();
    cout << "After move left twice: " << ed.getTextWithCursor() << "\n";

    ed.moveRight();
    ed.moveRight();
    cout << "After move right twice: " << ed.getTextWithCursor() << "\n";

    return 0;
}
