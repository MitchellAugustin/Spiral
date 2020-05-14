#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->notebooksListView->setMouseTracking(true);
    ui->sectionsListView->setMouseTracking(true);

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
    connect(ui->notebooksListView, SIGNAL(activated(QModelIndex)), this, SLOT(notebookSelected(QModelIndex)));
    connect(ui->sectionsListView, SIGNAL(activated(QModelIndex)), this, SLOT(sectionSelected(QModelIndex)));

    connect(ui->notebooksListView, SIGNAL(clicked(QModelIndex)), this, SLOT(notebookNameChanged(QModelIndex)));
    connect(ui->sectionsListView, SIGNAL(clicked(QModelIndex)), this, SLOT(sectionNameChanged(QModelIndex)));
    connect(ui->notebooksListView, SIGNAL(activated(QModelIndex)), this, SLOT(notebookNameChanged(QModelIndex)));
    connect(ui->sectionsListView, SIGNAL(activated(QModelIndex)), this, SLOT(sectionNameChanged(QModelIndex)));
    connect(ui->notebooksListView, SIGNAL(entered(QModelIndex)), this, SLOT(notebookNameChanged(QModelIndex)));
    connect(ui->sectionsListView, SIGNAL(entered(QModelIndex)), this, SLOT(sectionNameChanged(QModelIndex)));
    connect(tabWidget, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(pageDoubleClicked(int)));

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(pageSelected(int)));
    connect(ui->actionPrint_Content_to_Log, SIGNAL(triggered()), this, SLOT(printContentToLog()));
    connect(ui->actionClean_Empty_Boxes, SIGNAL(triggered()), this, SLOT(emptyBoxCleanup()));
    connect(ui->actionTest_Add_Box, SIGNAL(triggered()), this, SLOT(testAddBoxProgrammatically()));
    connect(ui->actionPage, SIGNAL(triggered()), this, SLOT(newPageButtonClicked()));
    connect(ui->actionSection, SIGNAL(triggered()), this, SLOT(newSectionButtonClicked()));
    connect(ui->actionNotebook, SIGNAL(triggered()), this, SLOT(newNotebookButtonClicked()));

    //The following is a demo using statically defined data of how the windowing system will be implemented
    Notebook *notebook = new Notebook();
    notebook->setName("New Notebook");
    Section *section = new Section();
    section->setName("New Section");
    Page *page = new Page();
    page->setName("New Page");
    section->addPage(page);
    Page *p2 = new Page();
    p2->setName("Page 2");
    section->addPage(p2);

    Section *section2 = new Section();
    section2->setName("Sec 2");
    Page *s2p1 = new Page();
    s2p1->setName("s2p1");
    section2->addPage(s2p1);

    notebook->addSection(section);
    notebook->addSection(section2);
    loadNotebook(notebook);
    openNotebook(notebook);

    Notebook *nb2 = new Notebook();
    nb2->setName("Notebook 2");
    Section *s2 = new Section();
    s2->setName("N2, Sec2");
    Page *n2s2p1 = new Page();
    n2s2p1->setName("n2s2p1");
    s2->addPage(n2s2p1);
    nb2->addSection(s2);
    loadNotebook(nb2);
    pageSelected(0);
    //END SAMPLE
    //NOTE: THESE OBJECTS MUST BE DELETED MANUALLY because of new being used!!!
    //These aren't, but in the actual methods, they need to be.
    checkNameChanges();
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
QWidget *generateEditorPane(QWidget *parent, QTabWidget *tabWidget, Page *parentPage) {
    DragLayout *customDragLayout = new DragLayout(tabWidget, parentPage);


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
    notebookBrowserStringListModel->append(notebook->getName());
    //TODO Open the actual notebook file as a Notebook object
}

/**
 * @brief MainWindow::openNotebook - Opens a notebook and displays its sections in the UI
 * @param notebook
 */
void MainWindow::openNotebook(Notebook *notebook) {
    checkNameChanges();
    sectionBrowserStringListModel->removeRows(0, sectionBrowserStringListModel->rowCount());

    //Open the Notebook's first section
    openSection(*(notebook->loadSectionsList()->begin()));
    //For each section in the notebook being opened, add its name to the Section Browser
    for(QVector<Section*>::Iterator it = notebook->loadSectionsList()->begin(); it != notebook->loadSectionsList()->end(); ++it) {
        Section *curSection = *it;
        sectionBrowserStringListModel->append(curSection->getName());
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
            QWidget *editorPane = generateEditorPane(this, tabWidget, curPage);
            curPage->dragLayout = editorPane;
        }
        //Add the page to the UI with its DragLayout
        tabWidget->addTab(curPage->dragLayout, curPage->getName());
        currentlyOpenPage = curPage;
    }
    currentlyOpenSection = section;
}


/**
 * @brief MainWindow::newPageButtonClicked - Called when the "New Page" button is clicked
 */
void MainWindow::newPageButtonClicked() {
    bool res;
    QString text = QInputDialog::getText(0, "New Page Name", "Name: ", QLineEdit::Normal, "", &res);
    if (res && !text.isEmpty()) {
        newPage(currentlyOpenSection, text);
    }
}

/**
 * @brief MainWindow::newSectionButtonClicked - Called when the "New Section" button is clicked
 */
void MainWindow::newSectionButtonClicked() {
    bool res;
    QString text = QInputDialog::getText(0, "New Section Name", "Name: ", QLineEdit::Normal, "", &res);
    if (res && !text.isEmpty()) {
        newSection(currentlyOpenNotebook, text);
    }
}

/**
 * @brief MainWindow::newNotebookButtonClicked - Called when the "New Notebook" button is clicked
 */
void MainWindow::newNotebookButtonClicked() {
    //TODO Implement this after file I/O is implemented
    qDebug() << "New notebook not implemented";
}

/**
 * @brief MainWindow::newNotebook - Creates a new Notebook
 * @param notebookName
 */
void MainWindow::newNotebook(QString notebookName) {
    qDebug() << "New notebook:" << notebookName;
}

/**
 * @brief MainWindow::newSection - Creates a new section within the parameterized Notebook
 * @param notebook
 * @param sectionName
 */
void MainWindow::newSection(Notebook *notebook, QString sectionName) {
    qDebug() << "New section:" << sectionName;
    Section *section = new Section();
    section->setName(sectionName);
    currentlyOpenNotebook->addSection(section);
    openSection(section);
    sectionBrowserStringListModel->append(section->getName());
}

/**
 * @brief MainWindow::newPage - Adds a new page within the parameterized Section
 * @param section
 * @param pageName
 */
void MainWindow::newPage(Section *section, QString pageName) {
    Page *curPage = new Page();
    curPage->setName(pageName);
    //If the page has not yet been generated a DragLayout, generate one
    if(curPage->dragLayout == nullptr) {
        QWidget *editorPane = generateEditorPane(this, tabWidget, curPage);
        curPage->dragLayout = editorPane;
    }
    //Add the page to the UI with its DragLayout
    tabWidget->addTab(curPage->dragLayout, curPage->getName());
    tabWidget->setCurrentIndex(tabWidget->count() - 1);
    currentlyOpenPage = curPage;
    currentlyOpenSection->addPage(curPage);
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

/**
 * @brief MainWindow::pageDoubleClicked - Called when a page tab is double-clicked
 * @param index
 */
void MainWindow::pageDoubleClicked(int index) {
    qDebug() << "Page double clicked:" << index;
    bool res;
    QString text = QInputDialog::getText(0, "New Page Name", "Name: ", QLineEdit::Normal, "", &res);
    bool validIndex = (index >= 0) && (index < currentlyOpenSection->loadPagesList()->size());
    if (res && !text.isEmpty() && validIndex) {
        qDebug() << "Page name changed:" << currentlyOpenSection->loadPagesList()->at(index)->getName() << "->" << text;
        currentlyOpenSection->loadPagesList()->at(index)->setName(text);
        tabWidget->setTabText(index, text);
    }
}

/**
 * @brief MainWindow::checkNameChanges - Updates any unchanged names in the list views within their structure class counterparts
 * Note: This must be called on any list transitions and whenever the file is saved in order to prevent naming conflicts.
 */
void MainWindow::checkNameChanges() {
    for (int i = 0; i < sectionBrowserStringListModel->rowCount(); ++i) {
        sectionNameChanged(sectionBrowserStringListModel->index(i, 0));
    }
    for (int i = 0; i < notebookBrowserStringListModel->rowCount(); ++i) {
        notebookNameChanged(notebookBrowserStringListModel->index(i, 0));
    }
}

/**
 * @brief MainWindow::notebookNameChanged - Called when the name of a notebook is changed
 * @param newName
 */
void MainWindow::notebookNameChanged(QModelIndex index) {
    qDebug() << "Notebook name changed: " << openNotebooks->at(index.row())->getName() << "->" << index.data().toString();
    openNotebooks->at(index.row())->setName(index.data().toString());
}

/**
 * @brief MainWindow::sectionNameChanged - Called when the name of a section is changed
 * @param newName
 */
void MainWindow::sectionNameChanged(QModelIndex index) {
    qDebug() << "Section name changed: " << currentlyOpenNotebook->loadSectionsList()->at(index.row())->getName() << "->" << index.data().toString();
    currentlyOpenNotebook->loadSectionsList()->at(index.row())->setName(index.data().toString());
}

/**
 * @brief MainWindow::pageSelected - Called when a page in the TabView is selected
 * @param index
 */
void MainWindow::pageSelected(int index) {
    qDebug() << "Page selected: " << index;
    if (currentlyOpenNotebook && currentlyOpenSection) {
        bool validIndex = (index >= 0) && (index < currentlyOpenSection->loadPagesList()->size());
        if (validIndex) {
            currentlyOpenPage = currentlyOpenSection->loadPagesList()->at(index);
            emptyBoxCleanup();
        }
    }
}

/**
 * @brief MainWindow::testAddBoxProgrammatically - Adds a text box to the current page at 50, 50
 */
void MainWindow::testAddBoxProgrammatically() {
    checkNameChanges();
    if (currentlyOpenPage && currentlyOpenPage->dragLayout) {
        QWidget *childAt = currentlyOpenPage->dragLayout->childAt(50, 50);
        DragLayout *childDrag = dynamic_cast<DragLayout*>(childAt);
        if (childDrag) {
            childDrag->newTextBoxAtLocation(QPoint(50, 50));
        }
    }
}

/**
 * @brief MainWindow::printContentToLog - Slot method that is called when the "Print Content To Log" button
 * is clicked that prints all open notebook content to the QDebug output stream
 */
void MainWindow::printContentToLog() {
    checkNameChanges();
    //Print all notebook content to the log
    qDebug() << "Content in Notebook: " << currentlyOpenNotebook->getName() << "(UUID: " << currentlyOpenNotebook->getUUID() << ")";
    for(QVector<Section*>::Iterator it = currentlyOpenNotebook->loadSectionsList()->begin(); it != currentlyOpenNotebook->loadSectionsList()->end(); ++it) {
        Section *curSection = *it;
        qDebug() << "Section: " << curSection->getName() << "(UUID: " << curSection->getUUID() << ")";
        for(QVector<Page*>::Iterator p_it = curSection->loadPagesList()->begin(); p_it != curSection->loadPagesList()->end(); ++p_it) {
            //TEMPORARY - Iterate through and display all of this page's child textbox objects
            Page *curPage = *p_it;

            qDebug() << "Page: " << curPage->getName() << "(UUID: " << curPage->getUUID() << ")";
            QVector<TextBox*> children = curPage->textBoxList;
            int iter = 0;
            foreach(TextBox *obj, children) {
                qDebug() << "Box @ " << obj->location << "(UUID: " << obj->uuid <<
                            ") (" << obj->size().width() << "x" << obj->size().height() <<
                            ") (#" << iter << ")";
                iter++;
                if (obj == nullptr || obj->richTextEdit == nullptr) {
                    qDebug() << "Removed TextBox was at this index";
                    continue;
                }
                if (obj->richTextEdit->toPlainText().isEmpty()) {
                    qDebug() << "Empty box found, deleting...";
                    curPage->textBoxList.removeOne(obj);
                    obj->close();
                }
                else {
                    qDebug() << "HTML:";
                    qDebug() << obj->richTextEdit->toHtml();
                }
            }
        }
    }
}

/**
 * @brief MainWindow::emptyBoxCleanup - Removes all empty boxes
 */
void MainWindow::emptyBoxCleanup() {
    checkNameChanges();
    //Print all notebook content to the log
    for(QVector<Section*>::Iterator it = currentlyOpenNotebook->loadSectionsList()->begin(); it != currentlyOpenNotebook->loadSectionsList()->end(); ++it) {
        Section *curSection = *it;
        for(QVector<Page*>::Iterator p_it = curSection->loadPagesList()->begin(); p_it != curSection->loadPagesList()->end(); ++p_it) {
            //TEMPORARY - Iterate through and display all of this page's child textbox objects
            Page *curPage = *p_it;

            QVector<TextBox*> children = curPage->textBoxList;
            int iter = 0;
            foreach(TextBox *obj, children) {
                iter++;
                if (obj == nullptr || obj->richTextEdit == nullptr) {
                    qDebug() << "Box @ " << obj->location << "(UUID: " << obj->uuid << ") (#" << iter << ") is empty, removing";
                    qDebug() << "Removed TextBox was at this index";
                    continue;
                }
                if (obj->richTextEdit->toPlainText().isEmpty()) {
                    qDebug() << "Box @ " << obj->location << "(UUID: " << obj->uuid << ") (#" << iter << ") is empty, removing";
                    qDebug() << "Empty box found, deleting...";
                    curPage->textBoxList.removeOne(obj);
                    obj->close();
                }
            }
        }
    }
}
