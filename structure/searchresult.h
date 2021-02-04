/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

class Notebook;
class Section;
class Page;
class TextBox;

/**
 * @brief SearchResult - Structure class for search results returned by the "find" dialog.
 * @author Mitchell Augustin - https://mitchellaugustin.com
 */
class SearchResult {
public:
    //Note: If a destructor is ever added here, DO NOT delete document elements from it.
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
