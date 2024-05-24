#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "letter.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void onAnswerPressed(Letter* letter);
    void onAcceptPressed();
    void textLabelDisplay(std::string text);

private slots:
    void on_pushButton_B_clicked();
    void on_pushButton_A_clicked();
    void on_pushButton_C_clicked();
    void on_pushButton_D_clicked();

private:
    void init_picked_letters();
    std::vector<Letter*> letters;
    std::vector<Letter*> picked_letters;
    Ui::MainWindow *ui;
    void setQuestion();
    void isCorrectAnswer(Letter* letter, std::string buttonLetter);
    int questionIndex = 0;
};
#endif // MAINWINDOW_H
