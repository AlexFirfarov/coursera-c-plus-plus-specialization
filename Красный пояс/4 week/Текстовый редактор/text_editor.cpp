#include <string>
#include <list>
#include <algorithm>

#include "test_runner.h"
using namespace std;

class Editor {
public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() : cursor_it(text.begin()) {}

  void Left() {
    if (cursor_it != text.begin()) {
      --cursor_it;
    }
  }

  void Right() {
    if (cursor_it != text.end()) {
      ++cursor_it;
    }
  }

  void Insert(char token) {
    text.insert(cursor_it, token);
  }

  void Cut(size_t tokens = 1) {
    size_t dist = min(static_cast<size_t>(distance(cursor_it, text.end())), tokens);
    buffer.assign(cursor_it, next(cursor_it, dist));

    std::list<char>::iterator new_cursor_it = next(cursor_it, dist);
    text.erase(cursor_it, new_cursor_it);
    cursor_it = new_cursor_it;
  }

  void Copy(size_t tokens = 1) {
    size_t dist = min(static_cast<size_t>(distance(cursor_it, text.end())), tokens);
    buffer.assign(cursor_it, next(cursor_it, dist));
  }

  void Paste() {
    text.insert(cursor_it, buffer.begin(), buffer.end());
  }

  string GetText() const {
    string result(text.begin(), text.end());

    return result;
  }

private:
  std::list<char> text;
  std::list<char> buffer;
  std::list<char>::iterator cursor_it;
};

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}