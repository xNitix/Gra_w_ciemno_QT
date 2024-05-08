#include "letter.h"

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
        QIcon newIcon(":/m/letter_picked.png");
        this->button->setIcon(newIcon);
    } else {
        QIcon newIcon(":/m/letter.png");
        this->button->setIcon(newIcon);
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
    return this->value;
}
