#include "trade.h"
#include <random>
#include "letter.h"
#include <iostream>

using namespace std;

std::vector<Letter *> Trade::getHost_Letters() const
{
    return host_Letters;
}

int Trade::propability()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(10000, 8000);

    double randomNumber = distribution(gen);

    int boundedRandomNumber = std::max(0, std::min(100000, static_cast<int>(randomNumber)));

    return boundedRandomNumber;
}

std::vector<Letter *> Trade::getPlayer_Letters() const
{
    return player_Letters;
}

bool Trade::is_offert_accepted(int price)
{
    std::cout << "TARGET: " << target_value << "  ILE MAMY W TEORII PO ofercie: " << price << std::endl;
    double difference = std::abs(price - target_value);

    if (difference < 2500) {
        // Szansa 95%
        return (rand() % 100) < 95;
    } else if (difference >= 2500 && difference < 5000) {
        // Szansa 70%
        return (rand() % 100) < 70;
    } else if (difference >= 5000 && difference < 10000) {
        // Szansa 40%
        return (rand() % 100) < 40;
    } else if (difference >= 10000 && difference < 15000) {
        // Szansa 10%
        return (rand() % 100) < 10;
    } else if (difference >= 15000 && difference < 25000) {
        // Szansa 3%
        return (rand() % 100) < 3;
    } else {
        // Dla większych różnic szansa 1%
        return (rand() % 100) < 1;
    }
}
Trade::Trade()
{
    target_value = propability();
    cout << "WYLOSOWALEM: "<< target_value << endl;
}

bool Trade::player_offer(std::vector<int> letter_to_sell, int price_for_letters)
{
    int sum = 0;
    int minus100 = 1;
    float minus50 = 1;
    for(Letter* letter : player_Letters){
        int player_letter_nr = letter->getNr();
        bool som = false;
        for(int nr : letter_to_sell){
            if(nr == player_letter_nr){
                som = true;
            }
        }
        if(! som){
            if(letter->get_value() ==  -100){
                minus100 = 0;
            } else if(letter->get_value() ==  -50){
                minus50 = 0.5;
            } else {
                sum += letter->get_value();
            }
        }
    }
    cout << "SUMA PO SUMIE: "  << sum << endl;
    return is_offert_accepted( (sum + price_for_letters)*(minus100)*(minus50));
}

void Trade::start_trade()
{
    wasTradeAccepted = false;
    std::vector<int> letter_to_sell;
    /*
    std::cout << "letter1: " + std::to_string(chosenLetter[1]) +
    " letter2: " + std::to_string(chosenLetter[2]) +
    " letter3: " + std::to_string(chosenLetter[3]) +
    " letter4: " + std::to_string(chosenLetter[4]) +
    " letter5: " + std::to_string(chosenLetter[5]) << std::endl; */
    for(Letter* letter : player_Letters){
        cout << letter->get_value() << "  |  ";
    }
    cout << endl;
    std::cout << "trade price: " + std::to_string(trade_price) << std::endl;

    for(int i=1; i <= player_Letters.size(); i++){
        if(chosenLetter[i]){
            letter_to_sell.push_back(index_to_letter_number_map[i]);
        }
    }
    if(letter_to_sell.size() == 0){
        return;
    }
    cout << "TEST OFERTY" << endl;
    bool result = player_offer(letter_to_sell, trade_price);
    if(result){
        cout << "Transakcja powiodla sie!" << endl;
        wasTradeAccepted = true;
        move_letters_to_host(letter_to_sell, trade_price);
    }
}

void Trade::move_letters_to_host(std::vector<int> letter_to_sell, int trade_price){
    std::vector<Letter*> new_player_Letters;
    for(Letter* letter : player_Letters){
        bool was_added = false;
        for(int nr : letter_to_sell){
            if(letter->getNr() == nr){
                host_Letters.push_back(letter);
                was_added = true;
            }
        }
        if(!was_added){
            new_player_Letters.push_back(letter);
        }
    }
    player_Letters = new_player_Letters;


}

void Trade::set_player_letters(std::vector<Letter *> player_Letters)
{
    this->player_Letters = player_Letters;
}

void Trade::init_letters_map(){
    int index = 1;
    for(Letter* letter : player_Letters){
        index_to_letter_number_map[index] = letter->getNr();
        index++;
    }

}

void Trade::set_trade_price(int price)
{
    this->trade_price = price;
}

