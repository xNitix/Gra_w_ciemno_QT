#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "letter.h"
#include "qlabel.h"
#include <QMainWindow>
#include "trade.h"

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
    void setTradeCeter(Trade trade);


public slots:
    void onAnswerPressed(Letter* letter);
    void onAcceptPressed();
    void textLabelDisplay(std::string text);
    void on_pushButton_B_clicked();
    void on_pushButton_A_clicked();
    void on_pushButton_C_clicked();
    void on_pushButton_D_clicked();
    void on_pushButton_end_clicked();

private slots:


    void on_butt1_pressed();

    void on_butt2_pressed();

    void on_butt3_pressed();

    void on_butt4_pressed();

    void on_butt5_pressed();

    void on_pushButton_sell_clicked();

    void on_butt3_released();

    void on_butt1_released();

    void on_butt2_released();

    void on_butt4_released();

    void on_butt5_released();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_textChanged(const QString &arg1);

    void on_spinBox_editingFinished();

    void on_pushButton_offer_clicked();


private:
    int timeToWait = 50;
    Trade* trade = new Trade();
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
    void prepareTradeScreen();
    void setLettersNumbers();
    void drawLettersUITradeMode(Letter* letter, QLabel* qLabel);
    void prepareEndScreen();
    void drawEndLetter(int value, int nr, QLabel* qLabel, bool is_green);
    void moveLetterToHostOnScreen(std::vector<Letter*> host_before_trade_letters);
    void set_and_draw_player_money();
    bool is_new_offert = true;

};
#endif // MAINWINDOW_H
