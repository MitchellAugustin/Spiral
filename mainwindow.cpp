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

    QTabWidget *tabWidget = new QTabWidget(this);
    DragLayout *customDragLayout = new DragLayout(tabWidget);


    QScrollArea *scrollArea = new QScrollArea(this);
    //Note: The customDragLayout has to be resized for it to be clickable within the scrollArea for some reason.
    //TODO figure out how to make this automatic
    customDragLayout->resize(DEFAULT_TAB_SIZE, DEFAULT_TAB_SIZE);
    scrollArea->setWidget(customDragLayout);

    //Adds a new tab to the tabWidget containing the dragLayout
    tabWidget->addTab(scrollArea, "DragLayoutTab");

    ui->verticalLayout->addWidget(tabWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

