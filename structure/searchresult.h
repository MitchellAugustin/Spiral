#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

class Notebook;
class Section;
class Page;
class TextBox;

class SearchResult {
public:
    SearchResult() {

    }

    bool valid() {
        return notebook && section && page && textBox;
    }

    Notebook *notebook = nullptr;
    Section *section = nullptr;
    Page *page = nullptr;
    TextBox *textBox = nullptr;
    int cursorStartIndex = 0;
};

#endif // SEARCHRESULT_H
