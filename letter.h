#ifndef LETTER_H
#define LETTER_H
#include "qpushbutton.h"

class Letter
{
public:
    static int pick_amount;

private:
    int value;
    int is_pressed;
    QPushButton* button;

public:
    Letter(QPushButton* button, int value);
    void handler();
    int get_value();

private:
    void changeIcon();

};

#endif // LETTER_H
