#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setup browsing tools
    ui->browserToolBar->addWidget(ui->notebooksListView);
    ui->browserToolBar->addWidget(ui->sectionsListView);

    MainWindow::setWindowTitle("Test Notebook");

    tabWidget = new QTabWidget(this);


    ui->verticalLayout->addWidget(tabWidget);

    ui->notebooksListView->setModel(notebookBrowserStringListModel);
    ui->sectionsListView->setModel(sectionBrowserStringListModel);


    //Create a new notebook with one section that has one page (as a demo)
    Notebook *notebook = new Notebook();
    notebook->name = "New Notebook";
    Section *section = new Section();
    section->name = "New Section";
    Page *page = new Page();
    page->name = "New Page";
    section->addPage(page);
    Page *p2 = new Page();
    p2->name = "Page 2";
    section->addPage(p2);

    Section *section2 = new Section();
    section2->name = "Sec 2";
    Page *s2p1 = new Page();
    s2p1->name = "s2P1";
    section2->addPage(s2p1);

    notebook->addSection(section);
    notebook->addSection(section2);
    openNotebook(notebook);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QWidget *generateEditorPane(QWidget *parent, QTabWidget *tabWidget) {
    DragLayout *customDragLayout = new DragLayout(tabWidget);


    QScrollArea *scrollArea = new QScrollArea(parent);
    customDragLayout->resize(DEFAULT_TAB_SIZE, DEFAULT_TAB_SIZE);
    scrollArea->setWidget(customDragLayout);

    return scrollArea;
}

void MainWindow::openNotebook(Notebook *notebook) {
    openNotebooks->append(notebook);
    sectionBrowserStringListModel->removeRows(0, sectionBrowserStringListModel->rowCount());

    //Open the Notebook's first section
    openSection(*(notebook->loadSectionsList()->begin()));
    //For each section in the notebook being opened, add its name to the Section Browser
    for(QVector<Section*>::Iterator it = notebook->loadSectionsList()->begin(); it != notebook->loadSectionsList()->end(); ++it) {
        Section *curSection = *it;
        sectionBrowserStringListModel->append(curSection->name);
    }
    //Add Notebook to UI
    notebookBrowserStringListModel->append(notebook->name);
}

void MainWindow::openSection(Section *section) {
    tabWidget->clear();
    //For each page in the section being opened
    for(QVector<Page*>::Iterator p_it = section->loadPagesList()->begin(); p_it != section->loadPagesList()->end(); ++p_it) {
        Page *curPage = *p_it;
        tabWidget->addTab(generateEditorPane(this, tabWidget), curPage->name);
    }
}
