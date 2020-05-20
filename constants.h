#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QTextDocument>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QString>
#include "mrichtextedit.h"

static const int DEFAULT_TEXTBOX_WIDTH = 1000;
static const int DEFAULT_TAB_SIZE = 2000;
static const QString SPIRAL_VERSION = "Beta 1.0";
static const QString SPIRAL_CONTENT_MIME_TYPE = "application/x-spiralcontent";
static const QString RESIZE_MIME_TYPE = "application/x-spiralresize";
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
