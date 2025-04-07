#include <iostream>
#include <string>
#include <list>

class TextBuffer {
    using CharList = std::list<char>;
    using Iterator = std::list<char>::iterator;
  private:
    CharList data;           // linked list that contains the characters
    Iterator cursor;         // current position within the list
    int row;                 // current row
    int column;              // current column
    int index;               // current index
    // ... public interface not shown
  public:
    TextBuffer(): row(0), column(0), index(0) {
        cursor = data.end();
    }
    bool forward();
    bool backward();
    void insert(char c);
    bool remove();
    void move_to_row_start();
    void move_to_row_end();
    void move_to_column(int new_column);
    bool up();
    bool down();
    bool is_at_end() const;
    char data_at_cursor() const;
    int get_row() const;
    int get_column() const;
    int get_index() const;
    int size() const;
    std::string stringify() const;
    
  };

bool TextBuffer::forward() {
    if(cursor != data.end()) {
        ++cursor;
        ++column;
        ++index;
        return true;
    }
    else {
        return false;
    }
}

bool TextBuffer::backward() {
    if(cursor != data.begin()) {
        --cursor;
        --column;
        --index;
        return true;
    }
    else {
        return false;
    }
}

void TextBuffer::insert(char c) {
    if(cursor != data.end()) {
        data.insert(cursor, c);
        ++column;
        ++index;
        // ++cursor; //MAYBE
    }
    else {
        data.push_back(c);
        ++column;
        ++index;
        // ++cursor; //MAYBE
    }
}

bool TextBuffer::remove() {
    if(cursor != data.end()) {
        Iterator temp = cursor; //use this to avoid dangling pointer
        ++cursor;
        data.erase(temp);
        --column;
        --index;
        return true;
    }
    else {
        return false;
    }
}

void TextBuffer::move_to_row_start() {
    while (cursor != data.begin() && *cursor != '\n') {
        --cursor;
        --index;
        --column;
    }
    // FIX THIS
}

void TextBuffer::move_to_row_end() {
    while (cursor != data.end() && *cursor != '\n') {
        ++cursor;
        ++index;
        ++column;
    }
}

void TextBuffer::move_to_column(int new_column) {
    int curr_column = column;
    // Already handles edge cases (out of bounds column bcs of '\n' & data.end())
    if(new_column < curr_column) {
        while(cursor != data.begin() && *cursor != '\n' && curr_column > new_column) {
            --column;
            --index;
            --cursor;
        }
    }
    else if(new_column > curr_column) {
        while(cursor != data.end() && *cursor != '\n' && curr_column < new_column) {
            ++column;
            ++index;
            ++cursor;
        }
    }
    else {
        // none if equal.
    }
}

bool TextBuffer::up() {
    
}