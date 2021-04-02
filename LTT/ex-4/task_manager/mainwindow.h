#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_pushButton_clicked();

    void Graph_Show();


    void on_pushButton_kill_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_run_new_clicked();

private:
    Ui::MainWindow *ui;
    double time[120]={0};
    double cpu[120]={0};
    double mem[120]={0};
    double swap[120]={0};
};
#endif // MAINWINDOW_H
