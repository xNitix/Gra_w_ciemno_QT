#ifndef TRADE_H
#define TRADE_H

#include "letter.h"
#include "qlabel.h"
#include <vector>

class Trade
{
private:
    int player_money;
    std::vector<Letter*> host_Letters;
    int propability();
    std::vector<Letter*> player_Letters;
    int target_value;
    bool is_offert_accepted(int price);
    int trade_price;
    //bool letter1, letter2, letter3, letter4, letter5;

public:
    Trade();
    bool player_offer(std::vector<int> letter_to_sell, int price_for_letters);
    bool letter1 = false, letter2 = false, letter3 = false, letter4 = false, letter5 = false;
    void print_picked_letters();
    void set_player_letters(std::vector<Letter*> player_Letters);
    void set_trade_price(int price);
};

#endif // LETTER_H
