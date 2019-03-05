#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class AppModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_closeButton_clicked();

private:
    Ui::MainWindow *ui;
    AppModel       *appmodel;
};

#endif // MAINWINDOW_H
