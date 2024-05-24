#ifndef LETTER_H
#define LETTER_H
#include "qpushbutton.h"
#include "question.h"

class Letter
{
public:
    static int pick_amount;


private:
    int value;
    int is_pressed;
    QPushButton* button;
    Question question;
    int isWon = -1;
    int nr;

public:
    Letter(QPushButton* button, int value, const std::string& data_to_question, int nr);
    void handler();
    int get_value();
    int getIs_pressed() const;
    void setWin();
    void setLose();
    Question getQuestion() const;
    int getWin() const;
    int getNr() const;

private:
    void changeIcon();

};

#endif // LETTER_H
