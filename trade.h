#ifndef TRADE_H
#define TRADE_H

#include "letter.h"
#include "qlabel.h"
#include <vector>

class Trade
{
private:
    std::vector<Letter*> host_Letters;
    int propability();
    std::vector<Letter*> player_Letters;
    int target_value;
    bool is_offert_accepted(std::vector<int> letter_to_sell, int price_for_letters, int price);
    int trade_price;
    std::vector<std::vector<int>> offerts_history;

public:
    int player_money = 0;
    std::map<int, int> index_to_letter_number_map;
    Trade();
    bool wasTradeAccepted = false;
    bool player_offer(std::vector<int> letter_to_sell, int price_for_letters);
    bool chosenLetter[6] = {false, false, false, false, false, false};
    void start_trade();
    void set_player_letters(std::vector<Letter*> player_Letters);
    void set_trade_price(int price);
    void init_letters_map();
    void move_letters_to_host(std::vector<int> letter_to_sell, int trade_price);
    std::vector<Letter *> getPlayer_Letters() const;
    std::vector<Letter *> getHost_Letters() const;
    int getPlayer_money() const;
    std::vector<std::vector<int>> take_offer();
    //std::vector<std::vector<int>> take_new_offer();
    void generujKombinacje();
    std::vector<int> letter_give;
    std::vector<int> letter_take;
    std::vector<int> money_for_trade;
};

#endif // LETTER_H
