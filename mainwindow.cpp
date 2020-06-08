#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow - Constructs the main window
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    #ifdef Q_OS_WIN
    QIcon::setThemeName("tango");
    #endif

    //Use the existence of the "bold" icon to check to see if there is an icon theme on the system. If not, use Tango.
    if (!QIcon::hasThemeIcon("format-text-bold")) {
        QIcon::setThemeName("tango");
    }

    ui->setupUi(this);
    ui->notebooksListView->setMouseTracking(true);
    ui->sectionsListView->setMouseTracking(true);

    ui->toolBar->addWidget(ui->f_toolbar);
    ui->toolBar->setMinimumHeight(ui->f_toolbar->height() + 15);
    ui->toolBar->setObjectName("Formatting Toolbar");
    ui->toolBar->setAccessibleName("Formatting Toolbar");
    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->hiddenToolBar->setVisible(false);
    ui->hiddenToolBar->setDisabled(true);
    ui->hiddenToolBar->setFloatable(false);
    ui->hiddenToolBar->setMovable(false);
    ui->hiddenToolBar->setAllowedAreas(Qt::NoToolBarArea);
    ui->hiddenToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
//    ui->toolBar->setVisible(false);

    //Setup browsing tools
    ui->browserToolBar->setObjectName("Notebook/Section Browser");
    ui->browserToolBar->setAccessibleName("Notebook/Section Browser");
    ui->browserToolBar->addWidget(ui->notebooksListView);
    ui->browserToolBar->addWidget(ui->sectionsListView);
    ui->browserToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //Set window title and instantiate tab widget
    MainWindow::setWindowTitle("Spiral");
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);


    //Add the UI Elements to their proper locations
    ui->verticalLayout->addWidget(tabWidget);
    ui->notebooksListView->setModel(notebookBrowserStringListModel);
    ui->sectionsListView->setModel(sectionBrowserStringListModel);

    connect(QApplication::instance(), SIGNAL(focusChanged(QWidget *, QWidget*)), this, SLOT(focusChanged(QWidget *, QWidget *)));

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
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveNotebookButtonClicked()));
    connect(ui->actionSave_All, SIGNAL(triggered()), this, SLOT(saveAllButtonClicked()));
    connect(ui->actionNotebook_Properties, SIGNAL(triggered()), this, SLOT(notebookInfoButtonClicked()));
    connect(ui->actionExplain_Choices_on_this_menu, SIGNAL(triggered()), this, SLOT(explainChoicesButtonClicked()));
    connect(ui->actionAbout_Spiral, SIGNAL(triggered()), this, SLOT(aboutSpiralButtonClicked()));
    connect(ui->actionUser_Manual, SIGNAL(triggered()), this, SLOT(userManualButtonClicked()));
    connect(ui->actionContact_Us_Contribute, SIGNAL(triggered()), this, SLOT(contributeButtonClicked()));
    connect(ui->actionCheck_for_Updates, SIGNAL(triggered()), this, SLOT(checkUpdatesButtonClicked()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitButtonClicked()));
    connect(ui->actionAutosave, SIGNAL(toggled(bool)), this, SLOT(setAutosaveEnabled(bool)));
    connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(findButtonClicked()));

    connect(ui->openSession, SIGNAL(clicked()), this, SLOT(loadSession()));
}


/**
 * @brief MainWindow::~MainWindow - Autosaves all open notebooks and destroys the window
 */
MainWindow::~MainWindow()
{
    if (!autosaveEnabled) {
        QMessageBox::StandardButton res = QMessageBox::question(this, "Unsaved Work", "Would you like to save your unsaved work?",
                                                        QMessageBox::Yes|QMessageBox::No);
        if (res == QMessageBox::Yes) {
            saveAllButtonClicked();
        }
    }
    autosave();

    //Wait for all save threads to finish
    for(QVector<QFuture<void>>::iterator f_it = saveThreads->begin(); f_it != saveThreads->end(); ++f_it) {
        (*f_it).waitForFinished();
    }
    delete ui;
}

/**
 * @brief MainWindow::autosave - Autosaves all open notebooks
 */
void MainWindow::autosave() {
    if (!autosaveEnabled && currentlyOpenPage) {
        MainWindow::setWindowTitle(currentlyOpenPage->getName() + "* - Spiral");
        savedFlag = false;
        return;
    }
    if (!openNotebooks) {
        return;
    }
    for(QVector<Notebook*>::Iterator n_it = openNotebooks->begin(); n_it != openNotebooks->end(); ++n_it) {
        QFuture<void> saveNotebookThread = QtConcurrent::run(saveNotebookToDisk, *n_it);
        saveThreads->append(saveNotebookThread);
    }
    savedFlag = true;
}

/**
 * @brief MainWindow::focusChanged - Handles toolbar switching when the focus is changed between widgets
 * @param oldWidget
 * @param newWidget
 */
void MainWindow::focusChanged(QWidget *oldWidget, QWidget *newWidget) {
    //Autosave notebooks any time the user leaves the Spiral window
    if (newWidget == nullptr) {
        autosave();
    }
    qDebug() << "Focus changed, old:" << oldWidget;
    qDebug() << "Focus changed, new:" << newWidget;

    //Loops through all the parents of the widget that gained focus to see if any of the parents are an MRichTextEdit.
    QWidget *parentNew = newWidget;
    MRichTextEdit *newRichText = dynamic_cast<MRichTextEdit*>(parentNew);
    while (parentNew && !newRichText) {
        parentNew = parentNew->parentWidget();
        newRichText = dynamic_cast<MRichTextEdit*>(parentNew);
    }

    //If the focused widget is a child of a valid MRichTextEdit, swap in its toolbar and hide all other toolbars.
    if (newRichText) {
        newRichText->f_toolbar->setVisible(true);
        bool currentlyVisible = false;

        //Loop through all children of the UI's formatting toolbar.
        foreach (QObject *child, ui->toolBar->children()) {
            if(QWidget *w = dynamic_cast<QWidget*>(child)) {

                //If the toolbar that we want to swap in is already there, set the currentlyVisible flag to true.
                if (w == newRichText->f_toolbar) {
                    currentlyVisible = true;
                }

                //Hide all other toolbars
                else {
                    ui->hiddenToolBar->addWidget(w);
                }
            }
        }

        //If the toolbar we want to make visible is NOT already visible, make it visible.
        if (!currentlyVisible) {
            qDebug() << "Toolbar is now visible";
            ui->toolBar->addWidget(newRichText->f_toolbar);
        }
        newRichText->f_toolbar->setEnabled(true);
    }
}

/**
 * @brief MainWindow::setAutosaveEnabled - Enables/disables autosave in the current session
 * @param autosaveEnabled
 */
void MainWindow::setAutosaveEnabled(bool autosaveEnabled) {
    this->autosaveEnabled = autosaveEnabled;
    ui->actionAutosave->setChecked(autosaveEnabled);
    updateSessionFile();
    if (currentlyOpenPage) {
        MainWindow::setWindowTitle(currentlyOpenPage->getName() + " - Spiral");
    }
    savedFlag = true;
}

/**
 * @brief MainWindow::loadSession - Loads the notebooks located in session.json
 */
void MainWindow::loadSession() {
    ui->openSession->hide();
    if (sessionFilePath != nullptr) {
        QFile file(sessionFilePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Session file not found.";
        }
        else {
            QTextStream inputStream(&file);

            QString jsonString = inputStream.readAll();
            inputStream.flush();
            QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject rootObj = doc.object();
            bool autosave = rootObj.value("autosave").toBool();
            setAutosaveEnabled(autosave);

            QJsonArray arr = rootObj.value("open_notebooks").toArray();

            for(QJsonValueRef notebookRef : arr) {
                QString notebookPathJson = notebookRef.toString();
                if (notebookPathJson != nullptr && !notebookPathJson.isEmpty()) {
                    openNotebookFromFile(notebookPathJson);
                }
            }
        }
    }
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

    parentPage->dragLayout = customDragLayout;
    return scrollArea;
}

/**
 * @brief MainWindow::newNotebookButtonClicked - Called when the "New Notebook" button is clicked
 */
void MainWindow::newNotebookButtonClicked() {
    QString fileURL = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/", tr("Spiral Notebooks (*.snb)"));
    if (fileURL != nullptr && !fileURL.isEmpty()) {
        qDebug() << "New notebook URL:";
        newNotebookAtFile(fileURL);
    }
}

/**
 * @brief MainWindow::openNotebookButtonPressed - Called when the "Open Notebook" button is clicked
 */
void MainWindow::openNotebookButtonClicked() {
    QString fileURL = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", tr("Spiral Notebooks (*.snb)"));
    if (fileURL != nullptr && !fileURL.isEmpty()) {
        openNotebookFromFile(fileURL);
    }
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
    currentlyOpenPage = newPage;
    currentlyOpenSection = newSection;
    currentlyOpenNotebook = newNotebook;
    openNotebook(newNotebook);
    //Save this notebook to its corresponding .snb file
    saveNotebookToDisk(newNotebook);
}

/**
 * @brief MainWindow::saveNotebookButtonClicked - Called when the "Save" button is clicked
 * Saves the currently open notebook to its file
 */
void MainWindow::saveNotebookButtonClicked() {
    if (currentlyOpenNotebook != nullptr) {
        qDebug() << "Saving currently open notebook to file";
        saveNotebookToDisk(currentlyOpenNotebook);
    }
}

/**
 * @brief MainWindow::saveAllButtonClicked - Saves all currently open notebooks
 */
void MainWindow::saveAllButtonClicked() {
    if (!openNotebooks) {
        return;
    }
    for(QVector<Notebook*>::Iterator n_it = openNotebooks->begin(); n_it != openNotebooks->end(); ++n_it) {
        saveNotebookToDisk(*n_it);
    }
    MainWindow::setWindowTitle(currentlyOpenPage->getName() + " - Spiral");
    savedFlag = true;
}

/**
 * @brief MainWindow::saveNotebookToDisk - Saves the notebook to its corresponding .snb file
 * @param notebook
 */
void MainWindow::saveNotebookToDisk(Notebook *notebook) {
    if (notebook->path != nullptr) {
        QFile file(notebook->path);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(0, "Unable to save notebook:", file.errorString());
        }
        else {
            qDebug() << "Writing to path:" << notebook->path;
            QJsonObject obj;
            obj.insert("notebook_name", notebook->getName());
            obj.insert("notebook_uuid", notebook->getUUID());

            QJsonArray sections;

            QTextStream outputStream(&file);

            for(QVector<Section*>::Iterator it = notebook->loadSectionsList()->begin(); it != notebook->loadSectionsList()->end(); ++it) {
                Section *curSection = *it;
                qDebug() << "Section: " << curSection->getName() << "(UUID: " << curSection->getUUID() << ")";
                QJsonObject thisSectionJson;
                thisSectionJson.insert("section_name", curSection->getName());
                thisSectionJson.insert("section_uuid", curSection->getUUID());
                QJsonArray pages;
                for(QVector<Page*>::Iterator p_it = curSection->loadPagesList()->begin(); p_it != curSection->loadPagesList()->end(); ++p_it) {
                    Page *curPage = *p_it;
                    QJsonObject thisPageJson;
                    thisPageJson.insert("page_name", curPage->getName());
                    thisPageJson.insert("page_uuid", curPage->getUUID());
                    qDebug() << "Page: " << curPage->getName() << "(UUID: " << curPage->getUUID() << ")";
                    QVector<TextBox*> children = curPage->textBoxList;
                    int iter = 0;
                    QJsonArray textboxes;
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
                            QJsonObject thisTextboxJson;
                            qDebug() << "HTML:";
                            qDebug() << obj->richTextEdit->toHtml();
                            thisTextboxJson.insert("box_uuid", obj->uuid);
                            thisTextboxJson.insert("box_location", QString::number(obj->location.x()) + "," + QString::number(obj->location.y()));
                            thisTextboxJson.insert("box_width", obj->width());
                            thisTextboxJson.insert("box_html", obj->richTextEdit->toHtml());
                            textboxes.append(thisTextboxJson);
                        }
                    }
                    thisPageJson.insert("textboxes", textboxes);
                    pages.append(thisPageJson);
                }
                thisSectionJson.insert("pages", pages);
                sections.append(thisSectionJson);
            }
            obj.insert("sections", sections);
            QJsonDocument doc(obj);
            outputStream << doc.toJson() << endl;
        }
    }
}

/**
 * @brief MainWindow::openNotebookFromFile - Parses the notebook file at the given path and loads it into the viewport.
 * @param filePath
 */
void MainWindow::openNotebookFromFile(QString filePath) {
    if (ui->openSession->isVisible()) {
        loadSession();
    }
    for(QVector<Notebook*>::Iterator n_it = openNotebooks->begin(); n_it != openNotebooks->end(); ++n_it) {
        if ((*n_it)->path.compare(filePath) == 0) {
            QMessageBox::information(this, "Cannot open notebook", "This notebook is already open.");
            return;
        }
    }
    qDebug() << "Opening notebook at location:" << filePath;
    if (filePath != nullptr) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "Unable to open notebook:", file.errorString());
        }
        else {
            QTextStream inputStream(&file);
            QString jsonString = inputStream.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject rootObj = doc.object();
            //Read notebook properties
            Notebook *notebook = new Notebook(rootObj.value("notebook_uuid").toString());
            notebook->setName(rootObj.value("notebook_name").toString());
            notebook->path = filePath;

            //Read sections
            QJsonArray sections = rootObj.value("sections").toArray();
            for(QJsonValueRef sectionRef : sections) {
                QJsonObject sectionJson = sectionRef.toObject();
                //Read section properties
                Section *section = new Section(sectionJson.value("section_uuid").toString());
                section->setName(sectionJson.value("section_name").toString());

                //Read pages
                QJsonArray pages = sectionJson.value("pages").toArray();
                for(QJsonValueRef pagesRef : pages) {
                    QJsonObject pageJson = pagesRef.toObject();
                    //Read page properties
                    Page *page = new Page(pageJson.value("page_uuid").toString());
                    page->setName(pageJson.value("page_name").toString());

                    //Read textboxes
                    QJsonArray textboxes = pageJson.value("textboxes").toArray();
                    for (QJsonValueRef textboxesRef : textboxes) {
                        QJsonObject textboxJson = textboxesRef.toObject();
                        //Read textbox properties
                        //If the page has not yet been generated a DragLayout, generate one
                        if(page->editorPane == nullptr) {
                            QWidget *editorPane = generateEditorPane(this, tabWidget, page);
                            page->editorPane = editorPane;
                            currentlyOpenNotebook = notebook;
                            currentlyOpenSection = section;
                            currentlyOpenPage = page;
                            tabWidget->addTab(page->editorPane, page->getName());
                            tabWidget->setCurrentIndex(tabWidget->count() - 1);
                        }

                        checkNameChanges();
                        qDebug() << "Draglayout added";
                        if (page && page->editorPane) {
                            qDebug() << "Draglayout valid";
                            DragLayout *childDrag = (DragLayout*) page->dragLayout;
                            if (childDrag) {
                                qDebug() << "Adding box to page:" << page->getName();
                                QString locationString = textboxJson.value("box_location").toString();
                                int boxX = locationString.split(",")[0].toInt();
                                int boxY = locationString.split(",")[1].toInt();
                                int boxWidth = textboxJson.value("box_width").toInt();
                                TextBox *thisBox = childDrag->newTextBoxAtLocation(QPoint(boxX, boxY), boxWidth);
                                thisBox->uuid = textboxJson.value("box_uuid").toString();
                                thisBox->richTextEdit->setHtml(textboxJson.value("box_html").toString());
//                                ui->toolBar->addWidget(thisBox->richTextEdit->f_toolbar);
                                thisBox->richTextEdit->f_toolbar->setVisible(false);
                                qDebug() << "Box has content:" << textboxJson.value("box_html").toString();
                            }
                        }
                    }

                    //Add page to section
                    section->addPage(page);
                }

                //Add section to notebook
                notebook->addSection(section);
            }



            //Load and open notebook
            loadNotebook(notebook);
            openNotebook(notebook);
        }
    }
}

/**
 * @brief MainWindow::loadNotebook - Loads a notebook into the browser (DOES NOT OPEN)
 * @param notebook
 */
void MainWindow::loadNotebook(Notebook *notebook) {
    openNotebooks->append(notebook);
    notebookBrowserStringListModel->append(notebook->getName());
    updateSessionFile();
}

/**
 * @brief MainWindow::updateSessionFile - Updates session.json with the list of currently open notebooks
 */
void MainWindow::updateSessionFile() {
    if (sessionFilePath != nullptr) {
        QFile file(sessionFilePath);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(0, "Unable to access session file", file.errorString());
        }
        else {
            QJsonObject obj;
            QJsonArray openNotebooksArray;
            for(QVector<Notebook*>::Iterator n_it = openNotebooks->begin(); n_it != openNotebooks->end(); ++n_it) {
                openNotebooksArray.append((*n_it)->path);
            }
            obj.insert("open_notebooks", openNotebooksArray);
            obj.insert("autosave", autosaveEnabled);
            qDebug() << "Autosave? " << autosaveEnabled;
            QTextStream outputStream(&file);
            outputStream << QJsonDocument(obj).toJson();
        }
    }
}

/**
 * @brief MainWindow::openNotebook - Opens a notebook and displays its sections in the UI
 * @param notebook
 */
void MainWindow::openNotebook(Notebook *notebook) {
    checkNameChanges();
    sectionBrowserStringListModel->removeRows(0, sectionBrowserStringListModel->rowCount());

    qDebug() << "Notebook opened" << notebook->getName();
    //Open the Notebook's first section
    openSection(*(notebook->loadSectionsList()->begin()));
    //For each section in the notebook being opened, add its name to the Section Browser
    for(QVector<Section*>::Iterator it = notebook->loadSectionsList()->begin(); it != notebook->loadSectionsList()->end(); ++it) {
        Section *curSection = *it;
        sectionBrowserStringListModel->append(curSection->getName());
    }
    //Add Notebook to UI
    currentlyOpenNotebook = notebook;
    MainWindow::setWindowTitle(currentlyOpenPage->getName() +  (savedFlag ? "" : "*") + " - Spiral");
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
        if(curPage->editorPane == nullptr) {
            QWidget *editorPane = generateEditorPane(this, tabWidget, curPage);
            curPage->editorPane = editorPane;
        }
        //Add the page to the UI with its DragLayout
        tabWidget->addTab(curPage->editorPane, curPage->getName());
        currentlyOpenPage = section->loadPagesList()->first();
        MainWindow::setWindowTitle(currentlyOpenPage->getName() +  (savedFlag ? "" : "*") + " - Spiral");
    }
    currentlyOpenSection = section;
}

/**
 * @brief MainWindow::deletePageButtonClicked - Called when the "Delete Page" button is clicked
 */
void MainWindow::deletePageButtonClicked() {
    if (tabWidget == nullptr) {
        QMessageBox::information(this, "Cannot Delete Page", "No pages are open.");
        return;
    }
    tabCloseRequested(tabWidget->currentIndex());
}

/**
 * @brief MainWindow::deleteSectionButtonClicked - Called when the "Delete Section" button is clicked
 */
void MainWindow::deleteSectionButtonClicked() {
    if (currentlyOpenNotebook == nullptr || currentlyOpenSection == nullptr) {
        QMessageBox::information(this, "Cannot Delete Section", "No sections are open.");
        return;
    }
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
    if (currentlyOpenNotebook == nullptr) {
        QMessageBox::information(this, "Cannot Close Notebook", "No notebooks are open.");
        return;
    }
    QMessageBox::StandardButton res = QMessageBox::question(this, "Confirm Close", "Are you sure you want to close this notebook?"
                                                                                   " (Closing the notebook will NOT delete it from the disk)",
                                                    QMessageBox::Yes|QMessageBox::No);
    if (res == QMessageBox::Yes) {
        //Wait for all save threads to finish
        for(QVector<QFuture<void>>::iterator f_it = saveThreads->begin(); f_it != saveThreads->end(); ++f_it) {
            (*f_it).waitForFinished();
        }

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
            MainWindow::setWindowTitle("Spiral");
        }
    }
    updateSessionFile();
}

/**
 * @brief MainWindow::newPageButtonClicked - Called when the "New Page" button is clicked
 */
void MainWindow::newPageButtonClicked() {
    if (currentlyOpenNotebook == nullptr || currentlyOpenSection == nullptr) {
        QMessageBox::information(this, "Cannot Create Page", "You are not currently in a section.");
        return;
    }
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
    if (currentlyOpenNotebook == nullptr) {
        QMessageBox::information(this, "Cannot Create Section", "You are not currently in a notebook.");
        return;
    }
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
    if (currentlyOpenSection == nullptr || currentlyOpenPage == nullptr) {
        QMessageBox::information(this, "Cannot Delete Page", "You are not currently in a page.");
        return;
    }
    QMessageBox::StandardButton res = QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this page?",
                                                    QMessageBox::Yes|QMessageBox::No);
    if (res == QMessageBox::Yes) {
        qDebug() << "Tab closed:" << index;
        tabWidget->removeTab(index);
        delete currentlyOpenSection->loadPagesList()->at(index)->editorPane;
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
    if(curPage->editorPane == nullptr) {
        QWidget *editorPane = generateEditorPane(this, tabWidget, curPage);
        curPage->editorPane = editorPane;
    }
    //Add the page to the UI with its DragLayout
    tabWidget->addTab(curPage->editorPane, curPage->getName());
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
    return;
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
    if (index.row() < openNotebooks->count() && openNotebooks->at(index.row())->getName().compare(index.data().toString()) != 0) {
        qDebug() << "Notebook name changed: " << openNotebooks->at(index.row())->getName() << "->" << index.data().toString();
        openNotebooks->at(index.row())->setName(index.data().toString());
    }
}

/**
 * @brief MainWindow::sectionNameChanged - Called when the name of a section is changed
 * @param newName
 */
void MainWindow::sectionNameChanged(QModelIndex index) {
    if (index.row() < currentlyOpenNotebook->loadSectionsList()->count() && currentlyOpenNotebook->loadSectionsList()->at(index.row())->getName().compare(index.data().toString()) != 0) {
        qDebug() << "Section name changed: " << currentlyOpenNotebook->loadSectionsList()->at(index.row())->getName() << "->" << index.data().toString();
        currentlyOpenNotebook->loadSectionsList()->at(index.row())->setName(index.data().toString());
    }
}

/**
 * @brief MainWindow::pageSelected - Called when a page in the TabView is selected
 * @param index
 */
void MainWindow::pageSelected(int index) {
//    qDebug() << "Page selected: " << index;
    if (currentlyOpenNotebook && currentlyOpenSection) {
        bool validIndex = (index >= 0) && (index < currentlyOpenSection->loadPagesList()->size());
        if (validIndex) {
            currentlyOpenPage = currentlyOpenSection->loadPagesList()->at(index);
            emptyBoxCleanup();
            MainWindow::setWindowTitle(currentlyOpenPage->getName() + (savedFlag ? "" : "*") + " - Spiral");
        }
    }
}

/**
 * @brief MainWindow::testAddBoxProgrammatically - Adds a text box to the current page at 50, 50
 */
void MainWindow::testAddBoxProgrammatically() {
    checkNameChanges();
    if (currentlyOpenPage && currentlyOpenPage->editorPane) {
        DragLayout *childDrag = (DragLayout*) currentlyOpenPage->dragLayout;
        if (childDrag) {
            childDrag->newTextBoxAtLocation(QPoint(50, 50), DEFAULT_TEXTBOX_WIDTH);
        }
    }
}

/**
 * @brief MainWindow::printContentToLog - Slot method that is called when the "Print Content To Log" button
 * is clicked that prints all open notebook content to the QDebug output stream
 */
void MainWindow::printContentToLog() {
    if (currentlyOpenNotebook == nullptr) {
        return;
    }
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
 * @brief MainWindow::emptyBoxCleanup - Slot wrapper for emptyBoxCleanupExternal()
 */
void MainWindow::emptyBoxCleanup() {
    emptyBoxCleanupExternal();
}

/**
 * @brief MainWindow::emptyBoxCleanup - Removes all empty boxes
 */
void MainWindow::emptyBoxCleanupExternal() {
    if (currentlyOpenNotebook == nullptr) {
        return;
    }
    checkNameChanges();
    //Print all notebook content to the log
    for(QVector<Section*>::Iterator it = currentlyOpenNotebook->loadSectionsList()->begin(); it != currentlyOpenNotebook->loadSectionsList()->end(); ++it) {
        Section *curSection = *it;
        for(QVector<Page*>::Iterator p_it = curSection->loadPagesList()->begin(); p_it != curSection->loadPagesList()->end(); ++p_it) {
            Page *curPage = *p_it;

            QVector<TextBox*> children = curPage->textBoxList;
            int iter = 0;
            foreach(TextBox *obj, children) {
                iter++;
                if (obj == nullptr || obj->richTextEdit == nullptr) {
                    qDebug() << "Box @ " << obj->location << "(UUID: " << obj->uuid << ") (#" << iter << ") is empty, removing";
                    qDebug() << "Removed TextBox was at this index";
                    obj->richTextEdit->f_toolbar->setEnabled(false);
                    continue;
                }
                if (obj->richTextEdit->toPlainText().isEmpty()) {
                    qDebug() << "Box @ " << obj->location << "(UUID: " << obj->uuid << ") (#" << iter << ") is empty, removing";
                    qDebug() << "Empty box found, deleting...";
                    curPage->textBoxList.removeOne(obj);
                    obj->expandLabel->close();
                    obj->contractLabel->close();
                    obj->close();
                    obj->richTextEdit->f_toolbar->setEnabled(false);
                }
            }
        }
    }
}

void MainWindow::findPreviousButtonClicked() {

}

void MainWindow::findNextButtonClicked() {

}

void MainWindow::findReplaceButtonClicked() {

}

void MainWindow::findCloseButtonClicked() {
    findDialog->close();
}

void MainWindow::findTextChanged(QString text) {
    qDebug() << "Find text:" << text;
}

/**
 * @brief MainWindow::findButtonClicked - Opens find and replace dialog
 */
void MainWindow::findButtonClicked() {
    qDebug() << "Find button clicked";
    findDialog = new QDialog(this);
    QVBoxLayout *findLayout = new QVBoxLayout(findDialog);
    QHBoxLayout *findBoxLayout = new QHBoxLayout(findDialog);
    QHBoxLayout *replaceBoxLayout = new QHBoxLayout(findDialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout(findDialog);
    QPushButton *previousButton = new QPushButton();
    previousButton->setText("Find Previous");
    QPushButton *nextButton = new QPushButton();
    nextButton->setText("Find Next");
    QPushButton *replaceAllButton = new QPushButton();
    replaceAllButton->setText("Replace All");
    QPushButton *closeButton = new QPushButton();
    closeButton->setText("Close");
    buttonLayout->addWidget(previousButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(closeButton);
    QLabel *findLabel = new QLabel();
    QLabel *replaceLabel = new QLabel();
    findLabel->setText("Text to Find:");
    replaceLabel->setText("Replacement Text:");
    findTextLineEdit = new QLineEdit(findDialog);
    replaceTextLineEdit = new QLineEdit(findDialog);
    findBoxLayout->addWidget(findLabel);
    findBoxLayout->addWidget(findTextLineEdit);
    replaceBoxLayout->addWidget(replaceLabel);
    replaceBoxLayout->addWidget(replaceTextLineEdit);
    findLayout->addLayout(findBoxLayout);
    findLayout->addLayout(replaceBoxLayout);
    findLayout->addLayout(buttonLayout);
    findDialog->setLayout(findLayout);
    findDialog->setAttribute(Qt::WA_DeleteOnClose);
    findDialog->setModal(false);
    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();

    connect(previousButton, SIGNAL(clicked()), this, SLOT(findPreviousButtonClicked()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(findNextButtonClicked()));
    connect(findTextLineEdit, SIGNAL(returnPressed()), this, SLOT(findNextButtonClicked()));
    connect(findTextLineEdit, SIGNAL(textChanged(QString)), this, SLOT(findTextChanged(QString)));
    connect(replaceAllButton, SIGNAL(clicked()), this, SLOT(findReplaceButtonClicked()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(findCloseButtonClicked()));
}

/**
 * @brief MainWindow::notebookInfoButtonClicked - Called when the notebook info button is clicked
 */
void MainWindow::notebookInfoButtonClicked() {
    if (currentlyOpenNotebook == nullptr) {
        QMessageBox::information(this, "No Information Available", "You are not currently in a notebook.");
        return;
    }
    int pages = 0;
    for(QVector<Section*>::Iterator it = currentlyOpenNotebook->loadSectionsList()->begin(); it != currentlyOpenNotebook->loadSectionsList()->end(); ++it) {
        pages += (*it)->loadPagesList()->count();
    }
    QString notebookInfoString("File: " + currentlyOpenNotebook->path + "\nNotebook Name: " + currentlyOpenNotebook->getName()
                               + "\nNotebook UUID: " + currentlyOpenNotebook->getUUID() + "\nTotal Sections: " +
                               QString::number(currentlyOpenNotebook->loadSectionsList()->count()) + "\nTotal Pages: " +
                               QString::number(pages));
    QMessageBox::information(this, "Notebook Information", notebookInfoString);
}

/**
 * @brief MainWindow::explainChoicesButtonClicked - Called when the "Explain Choices on this menu" button is clicked
 */
void MainWindow::explainChoicesButtonClicked() {
    QMessageBox::information(this, "Notebook Divisions", "A Spiral notebook is structured like a real-life \"spiral notebook.\""
        " Every notebook has several sections (usually separated by topic), and every section has many pages. Spiral (the app)"
        " structures notebooks in the same way.");
}

/**
 * @brief MainWindow::userManualButtonClicked - Called when the "User Manual" button is clicked
 * Opens the manual on Spiral's website in the user's main browser
 */
void MainWindow::userManualButtonClicked() {
    QDesktopServices::openUrl(QUrl("https://mitchellaugustin.com/spiral/manual.html"));
}

/**
 * @brief MainWindow::contributeButtonClicked - Called when the "Contribute" button is clicked
 * Opens the contribute page on Spiral's website in the user's main browser
 */
void MainWindow::contributeButtonClicked() {
    QDesktopServices::openUrl(QUrl("https://mitchellaugustin.com/spiral/contribute.html"));
}

/**
 * @brief MainWindow::checkUpdatesButtonClicked - Called when the "Check Updates" button is clicked
 * Opens the downloads page on Spiral's website in the user's main browser
 */
void MainWindow::checkUpdatesButtonClicked() {
    QDesktopServices::openUrl(QUrl("https://mitchellaugustin.com/spiral/downloads.html"));
}

/**
 * @brief MainWindow::aboutSpiralButtonClicked - Called when the "About Spiral" button is clicked
 */
void MainWindow::aboutSpiralButtonClicked() {
    QMessageBox aboutSpiralBox(this);
    aboutSpiralBox.setIconPixmap(QPixmap("logo.png").scaled(110, 100));
    aboutSpiralBox.setWindowTitle("About Spiral");
    aboutSpiralBox.setText("Spiral - https://mitchellaugustin.com/spiral/\n\nVersion: " + SPIRAL_VERSION + "\n\nAuthor: Mitchell Augustin\n\n" +
        "Licensed under the GNU General Public License v3\nhttps://www.gnu.org/licenses/gpl-3.0.en.html\n\nSpiral was built on the QT framework (https://www.qt.io/) and utilizes elements from " +
                           "Anchakor's MRichTextEdit, which can be found at \nhttps://github.com/Anchakor/MRichTextEditor." +
                           "\n\nThe Spiral logo is based on the following image from WikiMedia Commons, which is labeled for " +
                           "reuse with modification:\nhttps://commons.wikimedia.org/wiki/File:Triple-Spiral-Symbol-filled.svg.\n\n" +
                           "Spiral's build for Windows also utilizes an adaptation of Jürgen Skrotzky's QT dark style, which can be found at \n" +
                           "https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle.\n\n" +
                           "On systems without the freedesktop.org icon set, Spiral uses the Tango Icon Library.\n" +
                           "http://tango-project.org/Tango_Icon_Library/");
    aboutSpiralBox.setParent(this);
    aboutSpiralBox.exec();
}

/**
 * @brief MainWindow::exitButtonClicked - Exits Spiral
 */
void MainWindow::exitButtonClicked() {
    QApplication::exit(0);
}
