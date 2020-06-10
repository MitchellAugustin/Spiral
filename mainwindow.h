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

#include "draglayout.h"
#include "textbox.h"
#include "structure/notebook.h"
#include "spiralstringlist.h"
#include "structure/searchresult.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    void newNotebook(QString notebookName);
    void newSection(Notebook *notebook, QString sectionName);
    void newPage(Section *section, QString pageName);
    void checkNameChanges();
    static void saveNotebookToDisk(Notebook *notebook);
    Ui::MainWindow *ui;
    void emptyBoxCleanupExternal();
    void autosave();
public slots:
    void emptyBoxCleanup();

protected slots:
    void notebookSelected(QModelIndex index);
    void sectionSelected(QModelIndex index);
    void notebookNameChanged(QModelIndex index);
    void sectionNameChanged(QModelIndex index);
    void pageDoubleClicked(int index);
    void pageSelected(int index);
    void newPageButtonClicked();
    void newSectionButtonClicked();
    void newNotebookButtonClicked();
    void tabCloseRequested(int index);
    void deletePageButtonClicked();
    void deleteSectionButtonClicked();
    void closeNotebookButtonClicked();
    void printContentToLog();
    void testAddBoxProgrammatically();
    void openNotebookButtonClicked();
    void saveNotebookButtonClicked();
    void saveAllButtonClicked();
    void loadSession();
    void focusChanged(QWidget *, QWidget *);
    void notebookInfoButtonClicked();
    void explainChoicesButtonClicked();
    void aboutSpiralButtonClicked();
    void userManualButtonClicked();
    void contributeButtonClicked();
    void checkUpdatesButtonClicked();
    void exitButtonClicked();
    void setAutosaveEnabled(bool autosaveEnabled);
    void findButtonClicked();
    void findPreviousButtonClicked();
    void findNextButtonClicked();
    void findReplaceButtonClicked();
    void findCloseButtonClicked();
    void findDialogFinished(int);
    void findTextChanged(QString text);
    void findMatchCaseChanged(bool value);
    void replaceTextChanged(QString text);

private:
    QVector<Notebook*> *openNotebooks = new QVector<Notebook*>();
    SpiralStringList *notebookBrowserStringListModel = new SpiralStringList();
    SpiralStringList *sectionBrowserStringListModel = new SpiralStringList();
    QTabWidget *tabWidget;
    QString sessionFilePath = QDir::currentPath() + "/session.json";
    void updateSessionFile();
    bool autosaveEnabled = true;
    bool savedFlag = true;
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
};
#endif // MAINWINDOW_H
