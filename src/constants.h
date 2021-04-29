/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QTextDocument>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QString>
#include "mrichtextedit.h"

/**
 * @brief Constants - Header containing frequently used constants.
 * @author Mitchell Augustin - https://mitchellaugustin.com
 */

//Global constants
static const QString SPIRAL_VERSION = "Release 1.4-WIP";
static const QString BUILD_DATE = __DATE__;
static const QString DEFAULT_WINDOW_TITLE = "Spiral";
static const QString SPIRAL_CONTENT_MIME_TYPE = "application/x-spiralcontent";
static const QString RESIZE_MIME_TYPE = "application/x-spiralresize";
static const QString FILE_DIALOG_FORMAT_STRING = "Spiral Notebooks (*.snb)";

//UI Constants
static const int DEFAULT_TEXTBOX_WIDTH = 800;
static const int DEFAULT_TAB_SIZE = 2000;


namespace utilities {
int getMRichTextEditHeight(MRichTextEdit *richTextEdit);
}
#endif // CONSTANTS_H
