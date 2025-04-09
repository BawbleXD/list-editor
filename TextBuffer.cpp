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

    int compute_column() const;
  public:
    TextBuffer(): row(1), column(0), index(0) {
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

int TextBuffer::compute_column() const {
    if (cursor == data.begin()) return 0;

    Iterator it = cursor;
    int col = 0;

    // Go backwards to find the start of the line
    while (it != data.begin()) {
        --it;
        if (*it == '\n') break;
        ++col;
    }

    return col;
}

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
    if (cursor == data.begin()) {
        return false;
    }

    --cursor;
    --index;

    if (*cursor == '\n') {
        --row;
        column = compute_column();
    } else {
        --column;
    }

    return true;
}

void TextBuffer::insert(char c) {
    if (cursor != data.end()) {
        data.insert(cursor, c); // insert BEFORE cursor
    } else {
        data.push_back(c);      // append to end if cursor is at end
    }

    if (c == '\n') {
        ++row;
        column = 0;  // new line starts, cursor still on next char
    } else {
        ++column;
    }

    ++index;
}

bool TextBuffer::remove() {\
    if (cursor == data.end()) {
        return false;
    }

    Iterator temp = cursor;
    ++cursor; // advance before erasing to avoid dangling
    char removed = *temp;
    data.erase(temp);

    if (removed == '\n') {
        --row;
        column = compute_column();  // or 0, depending on design
    } else {
        --column;
    }

    --index;
    return true;
}

void TextBuffer::move_to_row_start() {
    while (cursor != data.begin() && *cursor != '\n') {
        --cursor;
        --index;
        --column;
    }
    if(*cursor == '\n') {
        ++cursor;
        ++index;
    }
    column = 0;
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
            --curr_column ;
            --index;
            --cursor;
        }
    }
    else if(new_column > curr_column) {
        while(cursor != data.end() && *cursor != '\n' && curr_column < new_column) {
            ++curr_column;
            ++index;
            ++cursor;
        }
    }
    column = curr_column;
    
}

bool TextBuffer::up() {
    if(row == 0) {
        return false;
    }
    int ori_counter = 0;

    // Step 1: Go to end of current row
    while (cursor != data.end() && *cursor != '\n') {
        ++cursor;
        ++index;
        ++column;
        ++ori_counter;
    }
    if (cursor != data.begin() && *cursor == '\n') {
        ++cursor;
        ++index;
    }
    column = 0;

    // Step 2: Go to end of previous row (just before the '\n')
    if (cursor == data.begin()) return false;
    --cursor;
    --index;

    // Step 3: Move to start of previous row
    int prev_counter = 0;
    while (cursor != data.begin() && *std::prev(cursor) != '\n') {
        --cursor;
        --index;
        ++prev_counter;
    }
    if (cursor != data.begin() && *cursor == '\n') {
        ++cursor;
        ++index;
    }
    column = 0;

    // Step 4: Move to column based on comparison
    int steps = std::min(ori_counter, prev_counter);
    for (int i = 0; i < steps && cursor != data.end() && *cursor != '\n'; ++i) {
        ++cursor;
        ++index;
        ++column;
    }

    --row;
    return true;
}

bool TextBuffer::down() {
    if(is_at_end()) {
        return false;
    }
    int target_col = column;

    // Step 1: Go to end of current row
    while (cursor != data.end() && *cursor != '\n') {
        ++cursor;
        ++index;
        ++column;
    }

    // Step 2: If at the end there no row below so return false
    if (cursor == data.end()) {
        return false;
    }

    // Step 3: Move to start of next row (right after '\n')
    ++cursor;
    ++index;
    column = 0;
    ++row;

    // Step 4: Move to end of next row
    int steps = 0;
    while (cursor != data.end() && *cursor != '\n' && steps < target_col) {
        ++cursor;
        ++index;
        ++column;
        ++steps;
    }

    return true;
}

bool TextBuffer::is_at_end() const {
    return cursor == data.end();
}

char TextBuffer::data_at_cursor() const {
    return *cursor;
}

int TextBuffer::get_row() const {
    return row;
}

int TextBuffer::get_column() const {
    return column;
}

int TextBuffer::get_index() const {
    return index;
}

int TextBuffer::size() const {
    return data.size();
}

std::string TextBuffer::stringify() const {
    return std::string(data.begin(), data.end());
}