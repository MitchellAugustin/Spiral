#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QTextDocument>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include "mrichtextedit.h"

static const int DEFAULT_TEXTBOX_WIDTH = 1000;
static const int DEFAULT_TAB_SIZE = 6000;
namespace utilities {
static int getMRichTextEditHeight(MRichTextEdit *richTextEdit) {
    QTextDocument *doc = richTextEdit->document();
    QAbstractTextDocumentLayout *layout = doc->documentLayout();
    int h = richTextEdit->f_toolbar->height() * 2;
    QTextBlock b = doc->begin();
    while (b != doc->end()) {
        h += layout->blockBoundingRect(b).height();
        b = b.next();
    }
    return h;
}
}
#endif // CONSTANTS_H
