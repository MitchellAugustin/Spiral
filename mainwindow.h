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

#include "draglayout.h"
#include "textbox.h"
//#include "breakpad/client/linux/handler/exception_handler.h"
//#include "breakpad/client/linux/minidump_writer/minidump_writer.h"
//#include "breakpad/google_breakpad/common/minidump_exception_linux.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
