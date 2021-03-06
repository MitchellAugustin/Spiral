/*
** Copyright (C) 2019-2021 Mitchell Augustin - https://mitchellaugustin.com/
**
** This program is licensed under the GNU General Public License v3
** https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/qtconcurrentrun.h>
#include <QMainWindow>
#include <QFontDialog>
#include <QFontComboBox>
#include <QColorDialog>
#include <QListView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QScrollArea>
#include <QStringListModel>
#include <QtPrintSupport/qabstractprintdialog.h>
#include <QtPrintSupport/qpagesetupdialog.h>
#include <QtPrintSupport/qprintdialog.h>
#include <QtPrintSupport/qprintengine.h>
#include <QtPrintSupport/qprintpreviewdialog.h>
#include <QtPrintSupport/qprintpreviewwidget.h>
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/qprinterinfo.h>

#include "draglayout.h"
#include "textbox.h"
#include "structure/notebook.h"
#include "spiralstringlist.h"
#include "structure/searchresult.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//UI Constants
static const QString FORMATTING_TOOLBAR_NAME = "Formatting Toolbar";
static const QString BROWSER_TOOLBAR_NAME = "Notebook/Section Browser";
static const int TOOLBAR_HEIGHT_BUFFER = 15;

//File format JSON keys
static const QString NOTEBOOK_NAME_KEY = "notebook_name";
static const QString NOTEBOOK_UUID_KEY = "notebook_uuid";
static const QString SECTIONS_ARR_KEY = "sections";
static const QString SECTION_NAME_KEY = "section_name";
static const QString SECTION_UUID_KEY = "section_uuid";
static const QString PAGES_ARR_KEY = "pages";
static const QString PAGE_NAME_KEY = "page_name";
static const QString PAGE_UUID_KEY = "page_uuid";
static const QString TEXTBOXES_KEY = "textboxes";
static const QString BOX_UUID_KEY = "box_uuid";
static const QString BOX_LOCATION_KEY = "box_location";
static const QString BOX_WIDTH_KEY = "box_width";
static const QString BOX_HTML_KEY = "box_html";

//Session file JSON keys
static const QString GRACEFUL_EXIT_KEY = "graceful_exit";
static const QString AUTOSAVE_KEY = "autosave";
static const QString OPEN_NOTEBOOKS_KEY = "open_notebooks";
static const QString BROWSER_WIDTH_KEY = "browser_width";
static const QString THEME_KEY = "theme";

//Theme JSON keys
static const QString THEME_SYSTEM = "THEME_SYSTEM";
static const QString THEME_LIGHT = "THEME_LIGHT";
static const QString THEME_DARK = "THEME_DARK";

/**
 * @brief MainWindow::MainWindow - Header for MainWindow that contains the bulk of Spiral's functionality.
 * @author Mitchell Augustin - https://mitchellaugustin.com
 * @param parent
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Notebook *currentlyOpenNotebook = nullptr;
    Section *currentlyOpenSection = nullptr;
    Page *currentlyOpenPage = nullptr;
    void newNotebookAtFile(QString filePath);
    void openNotebookFromFile(QString filePath);
    void loadNotebook(Notebook *notebook);
    void openNotebook(Notebook *notebook);
    void openSection(Section *section);
    void newSection(Notebook *notebook, QString sectionName);
    void newPage(Section *section, QString pageName);
    static void saveNotebookToDisk(Notebook *notebook);
    Ui::MainWindow *ui;
    void emptyBoxCleanupExternal();
    void autosave();
public slots:
    void emptyBoxCleanup();

protected slots:
    void notebookSelected(QModelIndex index);
    void sectionSelected(QModelIndex index);
    void notebookNameChanged(QModelIndex topLeft, QModelIndex bottomRight);
    void sectionNameChanged(QModelIndex topLeft, QModelIndex bottomRight);
    void pageDoubleClicked(int index);
    void pageSelected(int index);
    void newPageButtonClicked();
    void newSectionButtonClicked();
    void newNotebookButtonClicked();
    void tabCloseRequested(int index);
    void notebookMoved(QModelIndex parent, int start, int end, QModelIndex destination, int row);
    void sectionMoved(QModelIndex parent, int start, int end, QModelIndex destination, int row);
    void pageMoved(int from, int to);
    void deletePageButtonClicked();
    void deleteSectionButtonClicked();
    void closeNotebookButtonClicked();
    void printContentToLog();
    void testAddBoxProgrammatically();
    void openNotebookButtonClicked();
    void saveNotebookButtonClicked();
    void printButtonClicked();
    void generatePrintPreview(QPrinter *printer);
    void saveAllButtonClicked();
    void loadSession();
    void focusChanged(QWidget *, QWidget *);
    void notebookInfoButtonClicked();
    void explainChoicesButtonClicked();
    void aboutSpiralButtonClicked();
    void userManualButtonClicked();
    void contributeButtonClicked();
    void donateButtonClicked();
    void systemThemeButtonClicked(bool systemThemeEnabled);
    void darkThemeButtonClicked(bool darkThemeEnabled);
    void lightThemeButtonClicked(bool lightThemeEnabled);
    void checkUpdatesButtonClicked();
    void exitButtonClicked();
    void setAutosaveEnabled(bool autosaveEnabled);
    void findButtonClicked();
    void findPreviousButtonClicked();
    void findNextButtonClicked();
    void findReplaceButtonClicked();
    void findCloseButtonClicked();
    void findDialogFinished();
    void findTextChanged(QString text);
    void findMatchCaseChanged(bool value);
    void replaceTextChanged(QString text);

private:
    void setTheme();
    QVector<Notebook*> *openNotebooks = new QVector<Notebook*>();
    SpiralStringList *notebookBrowserStringListModel = new SpiralStringList();
    SpiralStringList *sectionBrowserStringListModel = new SpiralStringList();
    QTabWidget *tabWidget;
    QSplitter *browserSplitterVertical;
    QSplitter *browserSplitterHorizontal;
#ifdef Q_OS_LINUX
    QString dotspiraldir = QDir::homePath() + "/.spiral";
    QString sessionFilePath = dotspiraldir + "/session.json";
    QString defaultSessionFilePath = "/usr/share/spiral/session.json";
    QString defaultDemoSnbPath = "/usr/share/spiral/Demo.snb";
#endif
#ifdef Q_OS_WIN
    QString dotspiraldir = QDir::homePath() + "/.spiral";
    QString sessionFilePath = dotspiraldir + "/session.json";
    QString defaultSessionFilePath = QDir::currentPath() + "/session.json";
    QString defaultDemoSnbPath = QDir::currentPath() + "/Demo.snb";
#endif
    void updateSessionFile();
    bool sessionActive = false;
    bool gracefulExitFlag = false;
    bool crashDetected = false;
    bool autosaveEnabled = true;
    bool savedFlag = true;
    QString theme = THEME_SYSTEM;
    QVector<QFuture<void>> *saveThreads = new QVector<QFuture<void>>();
    QDialog *findDialog = nullptr;
    QLineEdit *findTextLineEdit = nullptr;
    QLineEdit *replaceTextLineEdit = nullptr;
    QString currentSearchQuery = nullptr;
    QString currentReplacementText = nullptr;
    bool queryUpdated = false;
    bool queryMatchCase = false;
    QVector<SearchResult*> *searchResults = new QVector<SearchResult*>();
    QVector<SearchResult*>::Iterator searchResultsIterator = nullptr;
    QVector<SearchResult*>::Iterator lastSearchResultsIterator = nullptr;
    bool findIterate(int direction, QString replacementText);
    void malformedNotebookError(QString filePath);
    bool doNotUpdateNamesFlag = false;
};
#endif // MAINWINDOW_H
