#include "letter.h"
#include <iostream>

int Letter::pick_amount = 0;

Letter::Letter(QPushButton *button, int value)
{
    this->button = button;
    this->value = value;
    this->is_pressed = false;
}

void Letter::changeIcon()
{
    if(is_pressed){
        this->button->setStyleSheet("background-image: url(:/m/letter_picked.png); background-repeat: no-repeat; background-position: center; font-size: 15px; text-align: bottom;");
    } else {
        this->button->setStyleSheet("background-image: url(:/m/letter.png); background-repeat: no-repeat; background-position: center; font-size: 15px; text-align: bottom;");
    }
}

void Letter::handler()
{
    if(pick_amount < 5 && !is_pressed){
        is_pressed = !is_pressed;
        pick_amount++;
        changeIcon();
    } else if(is_pressed){
        is_pressed = !is_pressed;
        pick_amount--;
        changeIcon();
    }
}

int Letter::get_value()
{
    std::cout << "jd " << button->text().toStdString() << " jd" <<  std::endl;
    return this->value;
}
