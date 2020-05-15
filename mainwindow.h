#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Notebook *currentlyOpenNotebook;
    Section *currentlyOpenSection;
    Page *currentlyOpenPage;
    void newNotebookAtFile(QString filePath);
    void openNotebookFromFile(QString filePath);
    void loadNotebook(Notebook *notebook);
    void openNotebook(Notebook *notebook);
    void openSection(Section *section);
    void newNotebook(QString notebookName);
    void newSection(Notebook *notebook, QString sectionName);
    void newPage(Section *section, QString pageName);
    void checkNameChanges();
    Ui::MainWindow *ui;
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

private:
    QVector<Notebook*> *openNotebooks = new QVector<Notebook*>();
    SpiralStringList *notebookBrowserStringListModel = new SpiralStringList();
    SpiralStringList *sectionBrowserStringListModel = new SpiralStringList();
    QTabWidget *tabWidget;
};
#endif // MAINWINDOW_H
