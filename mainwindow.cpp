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
    ui->f_toolbar->setEnabled(false);
    ui->toolBar->setMinimumHeight(ui->f_toolbar->height() + TOOLBAR_HEIGHT_BUFFER);
    ui->toolBar->setObjectName(FORMATTING_TOOLBAR_NAME);
    ui->toolBar->setAccessibleName(FORMATTING_TOOLBAR_NAME);
    ui->toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->hiddenToolBar->setVisible(false);
    ui->hiddenToolBar->setDisabled(true);
    ui->hiddenToolBar->setFloatable(false);
    ui->hiddenToolBar->setMovable(false);
    ui->hiddenToolBar->setAllowedAreas(Qt::NoToolBarArea);
    ui->hiddenToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //Setup browsing tools
    ui->browserToolBar->setObjectName(BROWSER_TOOLBAR_NAME);
    ui->browserToolBar->setAccessibleName(BROWSER_TOOLBAR_NAME);
    ui->browserToolBar->addWidget(ui->notebooksListView);
    ui->browserToolBar->addWidget(ui->sectionsListView);
    ui->browserToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    //Set window title and instantiate tab widget
    MainWindow::setWindowTitle(DEFAULT_WINDOW_TITLE);
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);


    //Add the UI Elements to their proper locations
    ui->verticalLayout->addWidget(tabWidget);
    ui->notebooksListView->setModel(notebookBrowserStringListModel);
    ui->sectionsListView->setModel(sectionBrowserStringListModel);
    //Unfortunately, QT's rowsMoved signal is not consistent across QT versions, so I have to disable this until further notice.
//    ui->notebooksListView->setDragEnabled(true);
//    ui->sectionsListView->setDragEnabled(true);
//    ui->notebooksListView->setDragDropMode(QListView::DragDropMode::InternalMove);
//    ui->sectionsListView->setDragDropMode(QListView::DragDropMode::InternalMove);
//    ui->notebooksListView->setMovement(QListView::Movement::Snap);
//    ui->sectionsListView->setMovement(QListView::Movement::Snap);

    connect(QApplication::instance(), SIGNAL(focusChanged(QWidget *, QWidget*)), this, SLOT(focusChanged(QWidget *, QWidget *)));

    //Connect slots (listeners) for the NotebooksListView and SectionsListView
    connect(ui->notebooksListView, SIGNAL(clicked(QModelIndex)), this, SLOT(notebookSelected(QModelIndex)));
    connect(ui->sectionsListView, SIGNAL(clicked(QModelIndex)), this, SLOT(sectionSelected(QModelIndex)));
    connect(ui->notebooksListView, SIGNAL(activated(QModelIndex)), this, SLOT(notebookSelected(QModelIndex)));
    connect(ui->sectionsListView, SIGNAL(activated(QModelIndex)), this, SLOT(sectionSelected(QModelIndex)));
    connect(ui->notebooksListView->model(), SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int)), this, SLOT(notebookMoved(QModelIndex, int, int, QModelIndex, int)));
    connect(ui->sectionsListView->model(), SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int)), this, SLOT(sectionMoved(QModelIndex, int, int, QModelIndex, int)));

    connect(notebookBrowserStringListModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(notebookNameChanged(QModelIndex, QModelIndex)));
    connect(sectionBrowserStringListModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(sectionNameChanged(QModelIndex, QModelIndex)));
    connect(tabWidget, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(pageDoubleClicked(int)));
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
    connect(tabWidget->tabBar(), SIGNAL(tabMoved(int, int)), this, SLOT(pageMoved(int, int)));

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
    connect(ui->actionDonate, SIGNAL(triggered()), this, SLOT(donateButtonClicked()));
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
    gracefulExitFlag = true;
    updateSessionFile();
    //Ask the user if they want to save their work if autosave is disabled
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

    for(QVector<Notebook*>::Iterator n_it = openNotebooks->begin(); n_it != openNotebooks->end(); ++n_it) {
        delete *n_it;
        *n_it = nullptr;
    }

    delete notebookBrowserStringListModel;
    delete sectionBrowserStringListModel;
    delete saveThreads;

    delete tabWidget;
    tabWidget = nullptr;
    delete ui;
    ui = nullptr;
}

/**
 * @brief MainWindow::autosave - Autosaves all open notebooks
 */
void MainWindow::autosave() {
    if (!autosaveEnabled && currentlyOpenPage) {
        MainWindow::setWindowTitle(currentlyOpenPage->getName() + "* - " + DEFAULT_WINDOW_TITLE);
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
    //If focus is lost, reset the find and replace query
    queryUpdated = false;
    //Deselect text if the widget losing focus is a QTextEdit
    QWidget *parentNewToolbar = newWidget;
    QToolBar *newToolbar = dynamic_cast<QToolBar*>(parentNewToolbar);
    while (parentNewToolbar && !newToolbar) {
        parentNewToolbar = parentNewToolbar->parentWidget();
        newToolbar = dynamic_cast<QToolBar*>(parentNewToolbar);
    }
    QTextEdit *oldTextEdit = dynamic_cast<QTextEdit*>(oldWidget);
    if (!newToolbar && oldTextEdit) {
        oldTextEdit->moveCursor(QTextCursor::Start);
    }

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
        MainWindow::setWindowTitle(currentlyOpenPage->getName() + " - " + DEFAULT_WINDOW_TITLE);
    }
    savedFlag = true;
}

/**
 * @brief MainWindow::updateSessionFile - Updates session.json with the list of currently open notebooks and current autosave preference
 */
void MainWindow::updateSessionFile() {
    if (sessionFilePath != nullptr) {
        QFile file(sessionFilePath);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, "Unable to access session file", file.errorString());
        }
        else {
            QJsonObject obj;
            QJsonArray openNotebooksArray;
            for(QVector<Notebook*>::Iterator n_it = openNotebooks->begin(); n_it != openNotebooks->end(); ++n_it) {
                openNotebooksArray.append((*n_it)->path);
            }
            obj.insert(GRACEFUL_EXIT_KEY, gracefulExitFlag);
            obj.insert(OPEN_NOTEBOOKS_KEY, openNotebooksArray);
            obj.insert(AUTOSAVE_KEY, autosaveEnabled);
            qDebug() << "Autosave? " << autosaveEnabled;
            QTextStream outputStream(&file);
            outputStream << QJsonDocument(obj).toJson();
        }
    }
}

/**
 * @brief MainWindow::loadSession - Loads the notebooks located in session.json and applies saved autosave preference
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
            bool gracefulExit = rootObj.value(GRACEFUL_EXIT_KEY).toBool();
            if (!rootObj.value(GRACEFUL_EXIT_KEY).isUndefined() && !gracefulExit) {
                crashDetected = true;
                QMessageBox::information(this, "Crash detected", "It appears that Spiral may have crashed. If your notebooks do not appear as you left them,"
                                                                 " you can restore Spiral's automatically backed up versions of them by navigating to"
                                                                 " the locations of any affected notebooks on the disk and replacing their .snb files"
                                                                 " with their corresponding .snb.bak files. (Each notebook's .snb.bak file is saved"
                                                                 " when its corresponding notebook is initially opened, so some edits may have been lost"
                                                                 " if Spiral crashed before it was able to successfully autosave.)"
                                                                 " If your notebooks are in their correct states, you can safely ignore this message.");
            }
            //Set the graceful exit flag to false on session load since Spiral is now running and in a session.
            //It will be set to 'true' in the destructor.
            gracefulExitFlag = false;
            bool autosave = rootObj.value(AUTOSAVE_KEY).toBool();
            setAutosaveEnabled(autosave);

            QJsonArray arr = rootObj.value(OPEN_NOTEBOOKS_KEY).toArray();

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
    QString fileURL = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/", FILE_DIALOG_FORMAT_STRING);
    if (fileURL != nullptr && !fileURL.isEmpty()) {
        qDebug() << "New notebook URL:";
        newNotebookAtFile(fileURL);
    }
}

/**
 * @brief MainWindow::openNotebookButtonPressed - Called when the "Open Notebook" button is clicked
 */
void MainWindow::openNotebookButtonClicked() {
    QString fileURL = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", FILE_DIALOG_FORMAT_STRING);
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
    if (currentlyOpenPage) {
        MainWindow::setWindowTitle(currentlyOpenPage->getName() + " - " + DEFAULT_WINDOW_TITLE);
    }
    else {
        MainWindow::setWindowTitle(DEFAULT_WINDOW_TITLE);
    }
    savedFlag = true;
}

/**
 * @brief MainWindow::saveNotebookToDisk - Saves the notebook to its corresponding .snb file
 * @param notebook
 */
void MainWindow::saveNotebookToDisk(Notebook *notebook) {
    if (notebook->path != nullptr) {
        qDebug() << "Saving notebook to disk: " << notebook->path;
        QFile file(notebook->path);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(0, "Unable to save notebook:", file.errorString());
        }
        else {
//            qDebug() << "Writing to path:" << notebook->path;
            QJsonObject obj;
            obj.insert(NOTEBOOK_NAME_KEY, notebook->getName());
            obj.insert(NOTEBOOK_UUID_KEY, notebook->getUUID());

            QJsonArray sections;

            QTextStream outputStream(&file);

            for(QVector<Section*>::Iterator it = notebook->loadSectionsList()->begin(); it != notebook->loadSectionsList()->end(); ++it) {
                Section *curSection = *it;
//                qDebug() << "Section: " << curSection->getName() << "(UUID: " << curSection->getUUID() << ")";
                QJsonObject thisSectionJson;
                thisSectionJson.insert(SECTION_NAME_KEY, curSection->getName());
                thisSectionJson.insert(SECTION_UUID_KEY, curSection->getUUID());
                QJsonArray pages;
                for(QVector<Page*>::Iterator p_it = curSection->loadPagesList()->begin(); p_it != curSection->loadPagesList()->end(); ++p_it) {
                    Page *curPage = *p_it;
                    QJsonObject thisPageJson;
                    thisPageJson.insert(PAGE_NAME_KEY, curPage->getName());
                    thisPageJson.insert(PAGE_UUID_KEY, curPage->getUUID());
                    if (curPage->opened) {
    //                    qDebug() << "Page: " << curPage->getName() << "(UUID: " << curPage->getUUID() << ")";
                        QVector<TextBox*> children = curPage->textBoxList;
                        int iter = 0;
                        QJsonArray textboxes;
                        foreach(TextBox *obj, children) {
    //                        qDebug() << "Box @ " << obj->location << "(UUID: " << obj->uuid <<
    //                                    ") (" << obj->size().width() << "x" << obj->size().height() <<
    //                                    ") (#" << iter << ")";
                            iter++;
                            if (obj == nullptr || obj->richTextEdit == nullptr) {
    //                            qDebug() << "Removed TextBox was at this index";
                                continue;
                            }
                            if (obj->richTextEdit->toPlainText().isEmpty()) {
    //                            qDebug() << "Empty box found, deleting...";
                                curPage->textBoxList.removeOne(obj);
                                obj->close();
                            }
                            else {
                                QJsonObject thisTextboxJson;
    //                            qDebug() << "HTML:";
    //                            qDebug() << obj->richTextEdit->toHtml();
                                thisTextboxJson.insert(BOX_UUID_KEY, obj->uuid);
                                thisTextboxJson.insert(BOX_LOCATION_KEY, QString::number(obj->location.x()) + "," + QString::number(obj->location.y()));
                                thisTextboxJson.insert(BOX_WIDTH_KEY, obj->width());
                                thisTextboxJson.insert(BOX_HTML_KEY, obj->richTextEdit->toHtml());
                                textboxes.append(thisTextboxJson);
                            }
                        }
                        thisPageJson.insert(TEXTBOXES_KEY, textboxes);
                    }
                    else {
                        thisPageJson.insert(TEXTBOXES_KEY, curPage->textboxes);
                    }
                    pages.append(thisPageJson);
                }
                thisSectionJson.insert(PAGES_ARR_KEY, pages);
                sections.append(thisSectionJson);
            }
            obj.insert(SECTIONS_ARR_KEY, sections);
            QJsonDocument doc(obj);
            outputStream << doc.toJson() << endl;
        }
        qDebug() << "Notebook saved successfully: " << notebook->path;
    }
}

/**
 * @brief MainWindow::malformedNotebookError - Displays whenever the user attempts to open a malformed .snb file
 * @param filePath
 */
void MainWindow::malformedNotebookError(QString filePath) {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, "Cannot open notebook", "Unable to open notebook '" + filePath + "': " + "Malformed .snb file");
}

/**
 * @brief MainWindow::openNotebookFromFile - Parses the notebook file at the given path and loads it into the viewport.
 * @param filePath
 */
void MainWindow::openNotebookFromFile(QString filePath) {
    qDebug() << "Opening notebook: " << filePath;
    QApplication::setOverrideCursor(Qt::WaitCursor);
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
            QMessageBox::information(this, "Unable to open notebook:", file.errorString());
        }
        else {
            QTextStream inputStream(&file);
            QString jsonString = inputStream.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (doc.isNull()) {
                malformedNotebookError(filePath);
                return;
            }
            QJsonObject rootObj = doc.object();
            if (rootObj.isEmpty() || rootObj.value(NOTEBOOK_UUID_KEY).isNull() || rootObj.value(SECTIONS_ARR_KEY).isNull()) {
                malformedNotebookError(filePath);
                return;
            }
            //Read notebook properties
            Notebook *notebook = new Notebook(rootObj.value(NOTEBOOK_UUID_KEY).toString());
            notebook->setName(rootObj.value(NOTEBOOK_NAME_KEY).toString());
            notebook->path = filePath;

            //Read sections
            QJsonArray sections = rootObj.value(SECTIONS_ARR_KEY).toArray();
            for(QJsonValueRef sectionRef : sections) {
                QJsonObject sectionJson = sectionRef.toObject();
                //Read section properties
                Section *section = new Section(sectionJson.value(SECTION_UUID_KEY).toString());
                section->setName(sectionJson.value(SECTION_NAME_KEY).toString());

                if (rootObj.value(PAGES_ARR_KEY).isNull()) {
                    malformedNotebookError(filePath);
                    return;
                }
                //Read pages
                QJsonArray pages = sectionJson.value(PAGES_ARR_KEY).toArray();
                for(QJsonValueRef pagesRef : pages) {
                    QJsonObject pageJson = pagesRef.toObject();
                    //Read page properties
                    Page *page = new Page(pageJson.value(PAGE_UUID_KEY).toString());
                    page->setName(pageJson.value(PAGE_NAME_KEY).toString());

                    //Read textboxes
                    if (rootObj.value(TEXTBOXES_KEY).isNull()) {
                        malformedNotebookError(filePath);
                        return;
                    }
                    page->textboxes = pageJson.value(TEXTBOXES_KEY).toArray();

                    currentlyOpenNotebook = notebook;
                    currentlyOpenSection = section;
                    currentlyOpenPage = page;
                    tabWidget->addTab(page->editorPane, page->getName());
                    tabWidget->setCurrentIndex(tabWidget->count() - 1);
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
    QApplication::restoreOverrideCursor();
    qDebug() << "Notebook opened successfully: " << filePath;
}

/**
 * @brief MainWindow::loadNotebook - Loads a notebook into the browser (DOES NOT OPEN)
 * @param notebook
 */
void MainWindow::loadNotebook(Notebook *notebook) {
    //Save .snb.bak file on initial load as long as Spiral exited normally.
    if (!crashDetected) {
        QString realPath = notebook->path;
        notebook->path = notebook->path + ".bak";
        saveNotebookToDisk(notebook);
        notebook->path = realPath;
    }
    openNotebooks->append(notebook);
    notebookBrowserStringListModel->append(notebook->getName());
    updateSessionFile();
}

/**
 * @brief MainWindow::openNotebook - Opens a notebook and displays its sections in the UI
 * @param notebook
 */
void MainWindow::openNotebook(Notebook *notebook) {
    doNotUpdateNamesFlag = true;
    sectionBrowserStringListModel->removeRows(0, sectionBrowserStringListModel->rowCount());

    qDebug() << "Notebook opened" << notebook->getName();
    //Open the Notebook's first section
    if (!notebook->loadSectionsList()->isEmpty()) {
        openSection(*(notebook->loadSectionsList()->begin()));
    }
    else {
        tabWidget->clear();
    }
    //For each section in the notebook being opened, add its name to the Section Browser
    for(QVector<Section*>::Iterator it = notebook->loadSectionsList()->begin(); it != notebook->loadSectionsList()->end(); ++it) {
        Section *curSection = *it;
        sectionBrowserStringListModel->append(curSection->getName());
    }
    //Add Notebook to UI
    currentlyOpenNotebook = notebook;
    MainWindow::setWindowTitle(currentlyOpenPage->getName() +  (savedFlag ? "" : "*") + " - " + DEFAULT_WINDOW_TITLE);
    doNotUpdateNamesFlag = false;
}

/**
 * @brief MainWindow::openSection - Opens a section and displays its pages in the UI
 * @param section
 */
void MainWindow::openSection(Section *section) {
    tabWidget->clear();
    currentlyOpenSection = section;
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
    }
    currentlyOpenPage = section->loadPagesList()->first();
    //A section *shouldn't* have 0 pages, but since it is technically possible, I'll check for it.
    if (currentlyOpenPage) {
        MainWindow::setWindowTitle(currentlyOpenPage->getName() +  (savedFlag ? "" : "*") + " - " + DEFAULT_WINDOW_TITLE);
        //We can safely open page 0 here since it will always be the first page in the section
        pageSelected(currentlyOpenSection->loadPagesList()->indexOf(currentlyOpenPage));
    }
    else {
        MainWindow::setWindowTitle(DEFAULT_WINDOW_TITLE);
    }
}

/**
 * @brief MainWindow::deletePageButtonClicked - Called when the "Delete Page" button is clicked
 */
void MainWindow::deletePageButtonClicked() {
    if (tabWidget == nullptr || currentlyOpenPage == nullptr) {
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
            currentlyOpenNotebook->removeSection(index);
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

        //Close the notebook, deallocate its memory, and remove it from the view.
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
 * @param index - The index of the tab being closed
 */
void MainWindow::tabCloseRequested(int index) {
    if (currentlyOpenSection == nullptr || currentlyOpenPage == nullptr || index < 0) {
        QMessageBox::information(this, "Cannot Delete Page", "You are not currently in a page.");
        return;
    }
    QMessageBox::StandardButton res = QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this page?",
                                                    QMessageBox::Yes|QMessageBox::No);
    if (res == QMessageBox::Yes) {
        qDebug() << "Tab closed:" << index;
        tabWidget->removeTab(index);
        delete currentlyOpenSection->loadPagesList()->at(index)->editorPane;
        currentlyOpenSection->removePage(index);
    }
}

/**
 * @brief MainWindow::pageMoved - Moves the page to its new place on reorganization in the Section structure
 * @param from
 * @param to
 */
void MainWindow::pageMoved(int from, int to) {
    if (currentlyOpenSection != nullptr) {
        bool validIndices = (from >= 0) && (to >= 0) && (from < currentlyOpenSection->loadPagesList()->size()) && (to < currentlyOpenSection->loadPagesList()->size());
        if (validIndices) {
            qDebug() << "Moved page " << from << " to " << to;
            currentlyOpenSection->loadPagesList()->move(from, to);
        }
    }
}

/**
 * @brief MainWindow::newSection - Creates a new section within the parameterized Notebook
 * @param notebook - The notebook in which the new section will be added
 * @param sectionName - The name of the section to be added
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
 * @param section - The section in which the page will be added
 * @param pageName - The name of the page to be added
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
    MainWindow::setWindowTitle(currentlyOpenPage->getName() + (savedFlag ? "" : "*") + " - " + DEFAULT_WINDOW_TITLE);
}

/**
 * @brief MainWindow::sectionSelected - Open the selected section in the UI
 * @param index - The index of the selected section.
 */
void MainWindow::sectionSelected(QModelIndex index) {
    openSection(currentlyOpenNotebook->loadSectionsList()->at(index.row()));
}

/**
 * @brief MainWindow::notebookSelected - Open the selected notebook in the UI
 * @param index - The index of the selected notebook.
 */
void MainWindow::notebookSelected(QModelIndex index) {
    openNotebook(openNotebooks->at(index.row()));
}

/**
 * @brief MainWindow::notebookMoved - Moves the notebook in the view
 * @param indexList
 */
void MainWindow::notebookMoved(QModelIndex parent, int start, int end, QModelIndex destination, int row) {
    //If the item is being moved down, subtract 1 from row
    if (start < row) {
        row -= 1;
    }
    if (openNotebooks != nullptr) {
        bool validIndices = (start >= 0) && (row >= 0) && (start < openNotebooks->size()) && (row < openNotebooks->size());
        if (validIndices) {
            qDebug() << "Moved Notebook " << start << " to " << row;
            openNotebooks->move(start, row);
            openNotebook(openNotebooks->at(row));
        }
    }
}

/**
 * @brief MainWindow::sectionMoved - Moves the section within its notebook
 * @param indexList
 */
void MainWindow::sectionMoved(QModelIndex parent, int start, int end, QModelIndex destination, int row) {
    //If the item is being moved down, subtract 1 from row
    if (start < row) {
        row -= 1;
    }
    if (currentlyOpenNotebook != nullptr) {
        bool validIndices = (start >= 0) && (row >= 0) && (start < currentlyOpenNotebook->loadSectionsList()->size()) && (row < currentlyOpenNotebook->loadSectionsList()->size());
        if (validIndices) {
            qDebug() << "Moved Section " << start << " to " << row;
            currentlyOpenNotebook->loadSectionsList()->move(start, row);
            openSection(currentlyOpenNotebook->loadSectionsList()->at(row));
        }
    }
}

/**
 * @brief MainWindow::pageDoubleClicked - Called when a page tab is double-clicked. Opens the page rename dialog.
 * @param index - Index of the page being modified.
 */
void MainWindow::pageDoubleClicked(int index) {
    bool res;
    QString text = QInputDialog::getText(0, "New Page Name", "Name: ", QLineEdit::Normal, "", &res);
    bool validIndex = (index >= 0) && (index < currentlyOpenSection->loadPagesList()->size());
    if (res && !text.isEmpty() && validIndex) {
        qDebug() << "Page name changed:" << currentlyOpenSection->loadPagesList()->at(index)->getName() << "->" << text;
        currentlyOpenSection->loadPagesList()->at(index)->setName(text);
        tabWidget->setTabText(index, text);
    }
    MainWindow::setWindowTitle(currentlyOpenPage->getName() + (savedFlag ? "" : "*") + " - " + DEFAULT_WINDOW_TITLE);
}

/**
 * @brief MainWindow::notebookNameChanged - Called when the name of a notebook is changed.
 * @param index - The index of the modified notebook
 */
void MainWindow::notebookNameChanged(QModelIndex topLeft, QModelIndex bottomRight) {
    if (doNotUpdateNamesFlag) {
        return;
    }
    QModelIndex index = topLeft;
    if (index.row() < openNotebooks->count() && openNotebooks->at(index.row())->getName().compare(index.data().toString()) != 0) {
        qDebug() << "Notebook name changed: " << openNotebooks->at(index.row())->getName() << "->" << index.data().toString();
        openNotebooks->at(index.row())->setName(index.data().toString());
    }
}

/**
 * @brief MainWindow::sectionNameChanged - Called when the name of a section is changed.
 * @param index - The index of the modified section
 */
void MainWindow::sectionNameChanged(QModelIndex topLeft, QModelIndex bottomRight) {
    if (doNotUpdateNamesFlag) {
        return;
    }
    QModelIndex index = topLeft;
    if (index.row() < currentlyOpenNotebook->loadSectionsList()->count() && currentlyOpenNotebook->loadSectionsList()->at(index.row())->getName().compare(index.data().toString()) != 0) {
        qDebug() << "Section name changed: " << currentlyOpenNotebook->loadSectionsList()->at(index.row())->getName() << "->" << index.data().toString();
        currentlyOpenNotebook->loadSectionsList()->at(index.row())->setName(index.data().toString());
    }
}

/**
 * @brief MainWindow::pageSelected - Called when a page in the TabView is selected
 * @param index - The index of the selected page (tab)
 */
void MainWindow::pageSelected(int index) {
    if (currentlyOpenNotebook && currentlyOpenSection) {
        bool validIndex = (index >= 0) && (index < currentlyOpenSection->loadPagesList()->size());
        if (validIndex && currentlyOpenSection->loadPagesList()->at(index) != nullptr) {
            currentlyOpenPage = currentlyOpenSection->loadPagesList()->at(index);
            emptyBoxCleanup();
            MainWindow::setWindowTitle(currentlyOpenPage->getName() + (savedFlag ? "" : "*") + " - " + DEFAULT_WINDOW_TITLE);
            QJsonArray textboxes = currentlyOpenPage->textboxes;
            if (!currentlyOpenPage->opened) {
                for (QJsonValueRef textboxesRef : textboxes) {
                    QJsonObject textboxJson = textboxesRef.toObject();
                    //Read textbox properties
                    //If the page has not yet been generated a DragLayout, generate one
                    if(currentlyOpenPage->editorPane == nullptr) {
                        QWidget *editorPane = generateEditorPane(this, tabWidget, currentlyOpenPage);
                        currentlyOpenPage->editorPane = editorPane;
                    }

                    if (!currentlyOpenPage->opened && currentlyOpenPage->dragLayout->isVisible()) {
                        currentlyOpenPage->opened = true;
                        qDebug() << "Opened page: " << currentlyOpenPage->getName() << " in " << currentlyOpenSection->getName();
                    }
                    else if (!currentlyOpenPage->opened && !currentlyOpenPage->dragLayout->isVisible()) {
                        qDebug() << "Attempted to mark page that is not currently visible as opened. Layout not generated.";
                        break;
                    }

    //                        qDebug() << "Draglayout added";
                    if (currentlyOpenPage && currentlyOpenPage->editorPane) {
    //                            qDebug() << "Draglayout valid";
                        DragLayout *childDrag = (DragLayout*) currentlyOpenPage->dragLayout;
                        if (childDrag) {
    //                                qDebug() << "Adding box to page:" << page->getName();
                            QString locationString = textboxJson.value(BOX_LOCATION_KEY).toString();
                            int boxX = locationString.split(",")[0].toInt();
                            int boxY = locationString.split(",")[1].toInt();
                            int boxWidth = textboxJson.value(BOX_WIDTH_KEY).toInt();
                            TextBox *thisBox = childDrag->newTextBoxAtLocation(QPoint(boxX, boxY), boxWidth);
                            thisBox->uuid = textboxJson.value(BOX_UUID_KEY).toString();
                            thisBox->richTextEdit->setHtml(textboxJson.value(BOX_HTML_KEY).toString());
    //                                ui->toolBar->addWidget(thisBox->richTextEdit->f_toolbar);
                            thisBox->richTextEdit->f_toolbar->setVisible(false);
    //                                qDebug() << "Box has content:" << textboxJson.value(BOX_HTML_KEY).toString();
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief MainWindow::testAddBoxProgrammatically - Adds a text box to the current page at 50, 50 for testing purposes
 */
void MainWindow::testAddBoxProgrammatically() {
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
    //Print all notebook content to the log
    for(QVector<Section*>::Iterator it = currentlyOpenNotebook->loadSectionsList()->begin(); it != currentlyOpenNotebook->loadSectionsList()->end(); ++it) {
        Section *curSection = *it;
        if (curSection == nullptr) {
            continue;
        }
        for(QVector<Page*>::Iterator p_it = curSection->loadPagesList()->begin(); p_it != curSection->loadPagesList()->end(); ++p_it) {
            Page *curPage = *p_it;
            if (curPage != nullptr) {
                QVector<TextBox*> children = curPage->textBoxList;
                int iter = 0;
                foreach(TextBox *obj, children) {
                    iter++;
                    if (obj == nullptr || obj->richTextEdit == nullptr) {
                        qDebug() << "Removed TextBox was at this index";
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
}

/**
 * @brief MainWindow::findIterate - Iterates to the next or previous found search result depending on the direction parameter.
 * @param direction -1: Previous, 1: Next
 * @param replacementText: If nullptr, this function just iterates to next found instance of searchQuery. If not null, it replaces that word with the replacement.
 * @return true if the last word in the result list was replaced, false otherwise.
 */
bool MainWindow::findIterate(int direction, QString replacementText) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //Alert the user that the search has looped back to the beginning
    if (direction == 1 && searchResultsIterator == (searchResults->end() - 1)) {
        QApplication::beep();
    }
    else if (direction == -1 && searchResultsIterator == searchResults->begin()) {
        QApplication::beep();
    }

    //Updates the search result list if the query was changed
    if (!queryUpdated) {
        for (QVector<SearchResult*>::Iterator s_it = searchResults->begin(); s_it != searchResults->end(); ++s_it) {
            delete *s_it;
        }
        searchResults->clear();
        //Search every textbox in every open notebook for any cursors matching the query and add to the search result list.
        for(QVector<Notebook*>::Iterator n_it = openNotebooks->begin(); n_it != openNotebooks->end(); ++n_it) {
            for(QVector<Section*>::Iterator s_it = (*n_it)->loadSectionsList()->begin(); s_it != (*n_it)->loadSectionsList()->end(); ++s_it) {
                for (QVector<Page*>::Iterator p_it = (*s_it)->loadPagesList()->begin(); p_it != (*s_it)->loadPagesList()->end(); ++p_it) {
                    if (!(*p_it)->opened) {
                        //If the page has not been opened, we have to iterate through the QJsonArrayList to find text content. If some is found, the page will be loaded.
                        for (QJsonArray::Iterator t_it = (*p_it)->textboxes.begin(); t_it != (*p_it)->textboxes.end(); ++t_it) {
                            if ((*t_it).isObject()) {
                                QJsonObject curObj = (*t_it).toObject();
                                if (curObj.value(BOX_HTML_KEY).toString().contains(currentSearchQuery, Qt::CaseInsensitive)) {
                                    qDebug() << "Found in unopened page: " << (*p_it)->getName();
                                    if (currentlyOpenNotebook != *n_it) {
                                        openNotebook(*n_it);
                                    }
                                    if (currentlyOpenSection != *s_it) {
                                        openSection(*s_it);
                                    }
                                    tabWidget->setCurrentIndex((*s_it)->loadPagesList()->indexOf(*p_it));
                                }
                            }
                        }
                    }


                    for (QVector<TextBox*>::Iterator t_it = (*p_it)->textBoxList.begin(); t_it != (*p_it)->textBoxList.end(); ++t_it) {
                        if ((*t_it)->richTextEdit->toPlainText().contains(currentSearchQuery, Qt::CaseInsensitive)) {
                            qDebug() << "Found in opened page: " << (*p_it)->getName();
                            int currentFrom = 0;
                            QTextCursor currentCursor;
                            if (queryMatchCase) {
                                 currentCursor = (*t_it)->richTextEdit->f_textedit->document()->find(currentSearchQuery, currentFrom, QTextDocument::FindCaseSensitively);
                            }
                            else {
                                 currentCursor = (*t_it)->richTextEdit->f_textedit->document()->find(currentSearchQuery, currentFrom);
                            }
                            /* Since positions are changed whenever an instance of the word is removed, arrange
                             * the search results within each text box in reverse order to prevent instances from being missed.
                             */
                            QStack<SearchResult*> tmpResultStack;
                            while (!currentCursor.isNull()) {
                                SearchResult *thisResult = new SearchResult();
                                thisResult->notebook = (*n_it);
                                thisResult->section = (*s_it);
                                thisResult->page = (*p_it);
                                thisResult->textBox = (*t_it);
                                thisResult->cursorStartIndex = currentFrom;
                                tmpResultStack.push(thisResult);
                                if (queryMatchCase) {
                                     currentCursor = (*t_it)->richTextEdit->f_textedit->document()->find(currentSearchQuery, currentFrom, QTextDocument::FindCaseSensitively);
                                }
                                else {
                                     currentCursor = (*t_it)->richTextEdit->f_textedit->document()->find(currentSearchQuery, currentFrom);
                                }
                                currentFrom = currentCursor.position();
                            }

                            foreach(SearchResult* res, tmpResultStack) {
                                searchResults->append(res);
                            }
                        }
                    }
                }
            }
        }
        qDebug() << "Results:" << searchResults->count();
        queryUpdated = true;
        lastSearchResultsIterator = nullptr;
        if (direction == -1) {
            searchResultsIterator = searchResults->end();
        }
        else {
            searchResultsIterator = searchResults->begin();
        }
    }
    QApplication::restoreOverrideCursor();
    if (searchResults->count() == 0) {
        QApplication::beep();
        return true;
    }



    //Deselects the last search result's cursor if necessary
    if (lastSearchResultsIterator && lastSearchResultsIterator != searchResults->end() && (*lastSearchResultsIterator)->valid()) {
        (*lastSearchResultsIterator)->textBox->richTextEdit->f_textedit->moveCursor(QTextCursor::Start);
    }


    //Navigate to next found item and show
    if (searchResultsIterator && searchResultsIterator != searchResults->end() && (*searchResultsIterator)->valid()) {
        if (currentlyOpenNotebook != (*searchResultsIterator)->notebook) {
            openNotebook((*searchResultsIterator)->notebook);
        }
        if (currentlyOpenSection != (*searchResultsIterator)->section) {
            openSection((*searchResultsIterator)->section);
        }
        if (currentlyOpenPage != (*searchResultsIterator)->page && currentlyOpenSection->loadPagesList()->contains((*searchResultsIterator)->page)) {
            tabWidget->setCurrentIndex(currentlyOpenSection->loadPagesList()->indexOf((*searchResultsIterator)->page));
            //May be redundant
            pageSelected(currentlyOpenSection->loadPagesList()->indexOf((*searchResultsIterator)->page));
        }
        if (currentlyOpenPage->textBoxList.contains((*searchResultsIterator)->textBox)) {
            QScrollArea *scrollArea = dynamic_cast<QScrollArea*>(currentlyOpenPage->editorPane);
            scrollArea->verticalScrollBar()->setValue((*searchResultsIterator)->textBox->location.y());
            scrollArea->horizontalScrollBar()->setValue((*searchResultsIterator)->textBox->location.x());
            (*searchResultsIterator)->textBox->richTextEdit->f_textedit->setFocus();
            QTextCursor cursorFindResult;
            if (queryMatchCase) {
                cursorFindResult = (*searchResultsIterator)->textBox->richTextEdit->f_textedit->document()->find(currentSearchQuery, (*searchResultsIterator)->cursorStartIndex, QTextDocument::FindCaseSensitively);
            }
            else {
                cursorFindResult = (*searchResultsIterator)->textBox->richTextEdit->f_textedit->document()->find(currentSearchQuery, (*searchResultsIterator)->cursorStartIndex);
            }
            if (!cursorFindResult.isNull()) {
                (*searchResultsIterator)->textBox->richTextEdit->f_textedit->setTextCursor((*searchResultsIterator)->textBox->richTextEdit->f_textedit->document()->find(currentSearchQuery, (*searchResultsIterator)->cursorStartIndex));
            }

            if (replacementText != nullptr) {
                (*searchResultsIterator)->textBox->richTextEdit->f_textedit->textCursor().removeSelectedText();
                (*searchResultsIterator)->textBox->richTextEdit->f_textedit->textCursor().insertText(replacementText);
                //If this was the last item to replace, terminate the 'replace all' function.
                if ((searchResultsIterator + 1) == searchResults->end()) {
                    return true;
                }
            }
        }
    }

    //Increment the iterator or loop back to beginning if it is at the end
    lastSearchResultsIterator = searchResultsIterator;
    if (direction == -1) {
        if (searchResultsIterator == searchResults->begin()) {
            searchResultsIterator = searchResults->end();
            searchResultsIterator--;
        }
        else {
            searchResultsIterator--;
        }
    }
    else {
        if ((searchResultsIterator + 1) == searchResults->end()) {
            if (replacementText != nullptr) {
                return true;
            }
            searchResultsIterator = searchResults->begin();
        }
        else {
            searchResultsIterator++;
        }
    }

    return false;
}

/**
 * @brief MainWindow::findPreviousButtonClicked - Iterates to previous found search result
 */
void MainWindow::findPreviousButtonClicked() {
    findIterate(-1, nullptr);
}

/**
 * @brief MainWindow::findNextButtonClicked - Iterates to next found search result
 */
void MainWindow::findNextButtonClicked() {
    findIterate(1, nullptr);
}

/**
 * NOTE: This is disabled in the master branch of Spiral. See Issue #9 for details.
 * @brief MainWindow::findReplaceButtonClicked - Replaces all instances of the text in the "Text to find" QLineEdit with that in the "Replacement Text" QLineEdit
 */
void MainWindow::findReplaceButtonClicked() {
    //Navigate to first search result to populate the list, then reset iterator to beginning.
    findIterate(1, nullptr);
    searchResultsIterator = searchResults->begin();
    if (currentReplacementText == nullptr) {
        currentReplacementText = " ";
    }
    bool lastReplaced = false;
    while (!lastReplaced) {
        lastReplaced = findIterate(1, currentReplacementText);
    }
    queryUpdated = true;
}

/**
 * @brief MainWindow::findDialogFinished - Called when the find/replace dialog is closed
 * @param result
 */
void MainWindow::findDialogFinished(int result) {
    findCloseButtonClicked();
}

/**
 * @brief MainWindow::findCloseButtonClicked - Handles find/replace dialog disposal
 */
void MainWindow::findCloseButtonClicked() {
    for (QVector<SearchResult*>::Iterator s_it = searchResults->begin(); s_it != searchResults->end(); ++s_it) {
        delete *s_it;
    }
    searchResults->clear();
    findDialog->close();
    //Since deleting a QObject deletes all of its children as per QObject documentation, we only have to delete findDialog here.
    //Since we have WA_DeleteOnClose set for this dialog, DO NOT call delete findDialog.
}

/**
 * @brief MainWindow::findTextChanged - Updates the search query when the text is changed
 * @param text
 */
void MainWindow::findTextChanged(QString text) {
    qDebug() << "Find text:" << text;
    currentSearchQuery = text;
    queryUpdated = false;
}

/**
 * @brief MainWindow::findMatchCaseChanged - Called whenever the find dialog's match case checkbox is modified
 * @param value
 */
void MainWindow::findMatchCaseChanged(bool value) {
    qDebug() << "Case sensitive:" << value;
    queryMatchCase = value;
    queryUpdated = true;
}

/**
 * @brief MainWindow::replaceTextChanged - Updates the replacement text
 * @param text
 */
void MainWindow::replaceTextChanged(QString text) {
    qDebug() << "Replace text:" << text;
    currentReplacementText = text;
}

/**
 * @brief MainWindow::findButtonClicked - Opens find and replace dialog
 */
void MainWindow::findButtonClicked() {
    qDebug() << "Find button clicked";
    findDialog = new QDialog(this);
    QVBoxLayout *findLayout = new QVBoxLayout(findDialog);
    QHBoxLayout *findBoxLayout = new QHBoxLayout(findDialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout(findDialog);
    QHBoxLayout *caseLayout = new QHBoxLayout(findDialog);
    QPushButton *previousButton = new QPushButton();
    previousButton->setText("Find Previous");
    previousButton->setProperty("autoDefault", false);
    previousButton->setProperty("default", false);
    QPushButton *nextButton = new QPushButton();
    nextButton->setText("Find Next");
    nextButton->setProperty("autoDefault", false);
    nextButton->setProperty("default", false);
    QPushButton *closeButton = new QPushButton();
    closeButton->setProperty("autoDefault", false);
    closeButton->setProperty("default", false);
    closeButton->setText("Close");
    buttonLayout->addWidget(previousButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(closeButton);
    QLabel *findLabel = new QLabel();
    findLabel->setText("Text to Find:");
    findTextLineEdit = new QLineEdit(findDialog);
    replaceTextLineEdit = new QLineEdit(findDialog);
    findBoxLayout->addWidget(findLabel);
    findBoxLayout->addWidget(findTextLineEdit);
    replaceTextLineEdit->close();
    findLayout->addLayout(findBoxLayout);
    QLabel *caseLabel = new QLabel();
    caseLabel->setText("Match Case: ");
    QCheckBox *caseCheckBox = new QCheckBox();
    caseLayout->addWidget(caseLabel);
    caseLayout->addWidget(caseCheckBox);
    findBoxLayout->addLayout(caseLayout);
    findLayout->addLayout(buttonLayout);
    findDialog->setLayout(findLayout);
    findDialog->setAttribute(Qt::WA_DeleteOnClose);
    findDialog->setWindowTitle("Find");
    findDialog->setModal(false);
    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();

    connect(previousButton, SIGNAL(clicked()), this, SLOT(findPreviousButtonClicked()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(findNextButtonClicked()));
    connect(findTextLineEdit, SIGNAL(returnPressed()), this, SLOT(findNextButtonClicked()));
    connect(findTextLineEdit, SIGNAL(textChanged(QString)), this, SLOT(findTextChanged(QString)));
    connect(replaceTextLineEdit, SIGNAL(textChanged(QString)), this, SLOT(replaceTextChanged(QString)));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(findCloseButtonClicked()));
    connect(findDialog, SIGNAL(finished(int)), this, SLOT(findDialogFinished(int)));
    connect(caseCheckBox, SIGNAL(toggled(bool)), this, SLOT(findMatchCaseChanged(bool)));
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
 * @brief MainWindow::donateButtonClicked - Opens the donation page in the user's browser
 */
void MainWindow::donateButtonClicked() {
    QDesktopServices::openUrl(QUrl("https://mitchellaugustin.com/spiral/contribute.html#donate"));
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
    aboutSpiralBox.setIconPixmap(QPixmap(":/icons/spiral/logo.png").scaled(110, 100));
    aboutSpiralBox.setWindowTitle("About Spiral");
    aboutSpiralBox.setText("Spiral - https://mitchellaugustin.com/spiral/\n\nVersion: " + SPIRAL_VERSION + "\n\nBuild Date: " + BUILD_DATE + "\n\nAuthor: Mitchell Augustin\n\n" +
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
