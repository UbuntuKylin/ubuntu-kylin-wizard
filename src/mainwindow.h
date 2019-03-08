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
    void on_installButton_clicked();

private:
    void initTableView();
    void doInstall();
    void startProgram();

    Ui::MainWindow *ui;
    AppModel       *appmodel;
    QStringList     install_list;
};

#endif // MAINWINDOW_H
