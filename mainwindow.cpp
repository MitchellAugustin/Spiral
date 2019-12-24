#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Moves the fontComboBox declared in the UI editor to the toolbar since it can't be done from the editor directly
    ui->toolBar->addWidget(ui->fontComboBox);
    ui->toolBar->addWidget(ui->fontSizeSpinner);
    ui->browserToolBar->addWidget(ui->notebooksListView);
    ui->browserToolBar->addWidget(ui->sectionsListView);

    //Creates a new textEdit and adds it to the vertical layout.
    QTextEdit* textEdit = new QTextEdit(this);
    DragWidget* dragWidget = new DragWidget(nullptr, textEdit);

    ui->verticalLayout->addWidget(dragWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

