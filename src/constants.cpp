#include "constants.h"

namespace utilities {
    int getMRichTextEditHeight(MRichTextEdit *richTextEdit) {
        QTextDocument *doc = richTextEdit->document();
        QAbstractTextDocumentLayout *layout = doc->documentLayout();
        int h = richTextEdit->f_toolbar->height() * 2;
        QTextBlock b = doc->begin();
        while (b != doc->end()) {
            h += static_cast<int>(layout->blockBoundingRect(b).height());
            b = b.next();
        }
        return h;
    }
}