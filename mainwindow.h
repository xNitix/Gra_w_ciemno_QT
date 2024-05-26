#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "letter.h"
#include "qlabel.h"
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
    void on_pushButton_B_clicked();
    void on_pushButton_A_clicked();
    void on_pushButton_C_clicked();
    void on_pushButton_D_clicked();

private:
    int timeToWait = 500;

    void init_picked_letters();
    std::vector<Letter*> letters;
    std::vector<Letter*> picked_letters;
    Ui::MainWindow *ui;
    void setQuestion();
    void isCorrectAnswer(Letter* letter, std::string buttonLetter);
    void showAnswer(bool win, Letter* letter);
    void connectAnswersButtons();
    int questionIndex = 0;
    bool buttonsBlockade = false;
    void lettersUI(Letter* letter, QLabel* qLabel);
    void paintLettersColors();
    int countGoodAnswers();
};
#endif // MAINWINDOW_H
