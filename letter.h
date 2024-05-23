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

public:
    Letter(QPushButton* button, int value, const std::string& data_to_question);
    void handler();
    int get_value();
    int getIs_pressed() const;

private:
    void changeIcon();

};

#endif // LETTER_H
