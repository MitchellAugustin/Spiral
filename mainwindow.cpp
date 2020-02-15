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

    //Set window title and instantiate tab widget
    MainWindow::setWindowTitle("Test Notebook");
    tabWidget = new QTabWidget(this);


    //Add the UI Elements to their proper locations
    ui->verticalLayout->addWidget(tabWidget);
    ui->notebooksListView->setModel(notebookBrowserStringListModel);
    ui->sectionsListView->setModel(sectionBrowserStringListModel);


    //Connect slots (listeners) for the NotebooksListView and SectionsListView
    connect(ui->notebooksListView, SIGNAL(clicked(QModelIndex)), this, SLOT(notebookSelected(QModelIndex)));
    connect(ui->sectionsListView, SIGNAL(clicked(QModelIndex)), this, SLOT(sectionSelected(QModelIndex)));

    //The following is a demo using statically defined data of how the windowing system will be implemented
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
    loadNotebook(notebook);
    openNotebook(notebook);

    Notebook *nb2 = new Notebook();
    nb2->name = "Notebook 2";
    Section *s2 = new Section();
    s2->name = "N2, Sec2";
    Page *n2s2p1 = new Page();
    n2s2p1->name = "n2s2p1";
    s2->addPage(n2s2p1);
    nb2->addSection(s2);
    loadNotebook(nb2);
    //END SAMPLE
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief generateEditorPane - Generates a QWidget that will be used as the editor pane for a Tab
 * @param parent
 * @param tabWidget
 * @return
 */
QWidget *generateEditorPane(QWidget *parent, QTabWidget *tabWidget) {
    DragLayout *customDragLayout = new DragLayout(tabWidget);


    QScrollArea *scrollArea = new QScrollArea(parent);
    customDragLayout->resize(DEFAULT_TAB_SIZE, DEFAULT_TAB_SIZE);
    scrollArea->setWidget(customDragLayout);

    return scrollArea;
}

/**
 * @brief MainWindow::loadNotebook - Loads a notebook into the browser (DOES NOT OPEN)
 * @param notebook
 */
void MainWindow::loadNotebook(Notebook *notebook) {
    openNotebooks->append(notebook);
    notebookBrowserStringListModel->append(notebook->name);
    //TODO Open the actual notebook file as a Notebook object
}

/**
 * @brief MainWindow::openNotebook - Opens a notebook and displays its sections in the UI
 * @param notebook
 */
void MainWindow::openNotebook(Notebook *notebook) {
    sectionBrowserStringListModel->removeRows(0, sectionBrowserStringListModel->rowCount());

    //Open the Notebook's first section
    openSection(*(notebook->loadSectionsList()->begin()));
    //For each section in the notebook being opened, add its name to the Section Browser
    for(QVector<Section*>::Iterator it = notebook->loadSectionsList()->begin(); it != notebook->loadSectionsList()->end(); ++it) {
        Section *curSection = *it;
        sectionBrowserStringListModel->append(curSection->name);
    }
    //Add Notebook to UI
    currentlyOpenNotebook = notebook;
}

/**
 * @brief MainWindow::openSection - Opens a section and displays its pages in the UI
 * @param section
 */
void MainWindow::openSection(Section *section) {
    tabWidget->clear();
    //For each page in the section being opened
    for(QVector<Page*>::Iterator p_it = section->loadPagesList()->begin(); p_it != section->loadPagesList()->end(); ++p_it) {
        Page *curPage = *p_it;
        //If the page has not yet been generated a DragLayout, generate one
        if(curPage->dragLayout == nullptr) {
            QWidget *editorPane = generateEditorPane(this, tabWidget);
            curPage->dragLayout = editorPane;
        }
        //Add the page to the UI with its DragLayout
        tabWidget->addTab(curPage->dragLayout, curPage->name);
    }
}


/**
 * @brief MainWindow::sectionSelected - Open the selected section in the UI
 * @param index
 */
void MainWindow::sectionSelected(QModelIndex index) {
    openSection(currentlyOpenNotebook->loadSectionsList()->at(index.row()));
}

/**
 * @brief MainWindow::notebookSelected - Open the selected notebook in the UI
 * @param index
 */
void MainWindow::notebookSelected(QModelIndex index) {
    openNotebook(openNotebooks->at(index.row()));
}
