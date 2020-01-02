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
    DragLayout* customDragLayout = new DragLayout(this);

    //Adds a new tab to the tabWidget containing the dragLayout
    tabWidget->addTab(customDragLayout, "DragLayoutTab");

    ui->verticalLayout->addWidget(tabWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

