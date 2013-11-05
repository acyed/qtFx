#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_regionBox_currentIndexChanged(int index);

    void on_colorBox_currentIndexChanged(int index);

    void on_applyButton_clicked();

private:
    Ui::MainWindow *ui;
    int colorIndex;
    int regionIndex;
};

#endif // MAINWINDOW_H
