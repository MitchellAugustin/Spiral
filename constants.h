#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QTextDocument>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QString>
#include "mrichtextedit.h"

//Global constants
static const QString SPIRAL_VERSION = "Release 1.0";
static const QString DEFAULT_WINDOW_TITLE = "Spiral";
static const QString SPIRAL_CONTENT_MIME_TYPE = "application/x-spiralcontent";
static const QString RESIZE_MIME_TYPE = "application/x-spiralresize";
static const QString FILE_DIALOG_FORMAT_STRING = "Spiral Notebooks (*.snb)";

//UI Constants
static const int DEFAULT_TEXTBOX_WIDTH = 800;
static const int DEFAULT_TAB_SIZE = 2000;


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
