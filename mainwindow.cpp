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
    tabWidget->setTabsClosable(true);


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
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(pageSelected(int)));
    connect(ui->actionPrint_Content_to_Log, SIGNAL(triggered()), this, SLOT(printContentToLog()));
    connect(ui->actionClean_Empty_Boxes, SIGNAL(triggered()), this, SLOT(emptyBoxCleanup()));
    connect(ui->actionTest_Add_Box, SIGNAL(triggered()), this, SLOT(testAddBoxProgrammatically()));
    connect(ui->actionPage, SIGNAL(triggered()), this, SLOT(newPageButtonClicked()));
    connect(ui->actionSection, SIGNAL(triggered()), this, SLOT(newSectionButtonClicked()));
    connect(ui->actionNotebook, SIGNAL(triggered()), this, SLOT(newNotebookButtonClicked()));
    connect(ui->actionDelete_Current_Page, SIGNAL(triggered()), this, SLOT(deletePageButtonClicked()));
    connect(ui->actionDelete_Current_Section, SIGNAL(triggered()), this, SLOT(deleteSectionButtonClicked()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openNotebookButtonClicked()));
    connect(ui->actionClose_This_Notebook, SIGNAL(triggered()), this, SLOT(closeNotebookButtonClicked()));

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
 * @brief MainWindow::newNotebookButtonClicked - Called when the "New Notebook" button is clicked
 */
void MainWindow::newNotebookButtonClicked() {
    QString fileURL = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/", tr("Spiral Notebooks (*.snb)"));
    newNotebookAtFile(fileURL);
}

/**
 * @brief MainWindow::openNotebookButtonPressed - Called when the "Open Notebook" button is clicked
 */
void MainWindow::openNotebookButtonClicked() {
    QString fileURL = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", tr("Spiral Notebooks (*.snb)"));
    openNotebookFromFile(fileURL);
}

/**
 * @brief MainWindow::newNotebookAtFile - Generates a new Notebook object with the given path, loads it into the viewport,
 * and saves it at that path
 * @param filePath
 */
void MainWindow::newNotebookAtFile(QString filePath) {
    Notebook *newNotebook = new Notebook();
    newNotebook->path = filePath;
    bool res;
    QString text = QInputDialog::getText(0, "New Notebook Name", "Name: ", QLineEdit::Normal, "", &res);
    if (res && !text.isEmpty()) {
        newNotebook->setName(text);
    }
    else {
        newNotebook->setName("New Notebook");
    }
    Section *newSection = new Section();
    newSection->setName("New Section");
    Page *newPage = new Page();
    newPage->setName("New Page");
    newSection->addPage(newPage);
    newNotebook->addSection(newSection);
    loadNotebook(newNotebook);
    openNotebook(newNotebook);
    //TODO Save this notebook to its corresponding .snb file
}

/**
 * @brief MainWindow::openNotebookFromFile - Parses the notebook file at the given path and loads it into the viewport.
 * @param filePath
 */
void MainWindow::openNotebookFromFile(QString filePath) {
    qDebug() << "Opening notebook at location:" << filePath;
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
 * @brief MainWindow::deletePageButtonClicked - Called when the "Delete Page" button is clicked
 */
void MainWindow::deletePageButtonClicked() {
    tabCloseRequested(tabWidget->currentIndex());
}

/**
 * @brief MainWindow::deleteSectionButtonClicked - Called when the "Delete Section" button is clicked
 */
void MainWindow::deleteSectionButtonClicked() {
    QMessageBox::StandardButton res = QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this section?",
                                                    QMessageBox::Yes|QMessageBox::No);
    if (res == QMessageBox::Yes) {
        if (currentlyOpenNotebook->loadSectionsList()->count() > 1) {
            Section *toDelete = currentlyOpenSection;
            int index = currentlyOpenNotebook->loadSectionsList()->indexOf(toDelete);
            currentlyOpenNotebook->loadSectionsList()->removeAt(index);
            sectionBrowserStringListModel->removeRow(index);
            openSection(currentlyOpenNotebook->loadSectionsList()->first());
            delete toDelete;
        }
        else {
            QMessageBox::information(this, "Cannot Delete", "You cannot delete the only section of a notebook");
        }
    }

}

/**
 * @brief MainWindow::closeNotebookButtonClicked - Closes the currently open notebook
 */
void MainWindow::closeNotebookButtonClicked() {
    QMessageBox::StandardButton res = QMessageBox::question(this, "Confirm Close", "Are you sure you want to close this notebook?"
                                                                                   " (Closing the notebook will NOT delete it from the disk)",
                                                    QMessageBox::Yes|QMessageBox::No);
    if (res == QMessageBox::Yes) {
        Notebook *toClose = currentlyOpenNotebook;
        int index = openNotebooks->indexOf(currentlyOpenNotebook);
        notebookBrowserStringListModel->removeRow(index);
        openNotebooks->removeAt(index);
        if (openNotebooks->count() > 0) {
            openNotebook(openNotebooks->first());
            delete toClose;
        }
        else {
            sectionBrowserStringListModel->removeRows(0, sectionBrowserStringListModel->rowCount());
            tabWidget->clear();
            currentlyOpenNotebook = nullptr;
            currentlyOpenSection = nullptr;
            currentlyOpenPage = nullptr;
            delete toClose;
        }
    }
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
 * @brief MainWindow::tabCloseRequested - Removes the page from the notebook
 * @param index
 */
void MainWindow::tabCloseRequested(int index) {
    QMessageBox::StandardButton res = QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this page?",
                                                    QMessageBox::Yes|QMessageBox::No);
    if (res == QMessageBox::Yes) {
        qDebug() << "Tab closed:" << index;
        tabWidget->removeTab(index);
        delete currentlyOpenSection->loadPagesList()->at(index)->dragLayout;
        currentlyOpenSection->loadPagesList()->removeAt(index);
    }
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
    if (notebook == nullptr) {
        QMessageBox::information(this, "Cannot Create Section", "Please open or create a new notebook before creating a new section");
        return;
    }
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
    if (section == nullptr) {
        QMessageBox::information(this, "Cannot Create Page", "Please open or create a new notebook and section before creating a new page");
        return;
    }
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
    if (openNotebooks->at(index.row())->getName().compare(index.data().toString()) != 0) {
        qDebug() << "Notebook name changed: " << openNotebooks->at(index.row())->getName() << "->" << index.data().toString();
        openNotebooks->at(index.row())->setName(index.data().toString());
    }
}

/**
 * @brief MainWindow::sectionNameChanged - Called when the name of a section is changed
 * @param newName
 */
void MainWindow::sectionNameChanged(QModelIndex index) {
    if (currentlyOpenNotebook->loadSectionsList()->at(index.row())->getName().compare(index.data().toString()) != 0) {
        qDebug() << "Section name changed: " << currentlyOpenNotebook->loadSectionsList()->at(index.row())->getName() << "->" << index.data().toString();
        currentlyOpenNotebook->loadSectionsList()->at(index.row())->setName(index.data().toString());
    }
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
