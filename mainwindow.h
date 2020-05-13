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
    void loadNotebook(Notebook *notebook);
    void openNotebook(Notebook *notebook);
    void openSection(Section *section);
    Ui::MainWindow *ui;

protected slots:
    void notebookSelected(QModelIndex index);
    void sectionSelected(QModelIndex index);
    void printContentToLog();

private:
    QVector<Notebook*> *openNotebooks = new QVector<Notebook*>();
    SpiralStringList *notebookBrowserStringListModel = new SpiralStringList();
    SpiralStringList *sectionBrowserStringListModel = new SpiralStringList();
    QTabWidget *tabWidget;
};
#endif // MAINWINDOW_H
