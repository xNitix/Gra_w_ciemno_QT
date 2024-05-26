#include "trade.h"
#include <random>
#include "letter.h"
#include <iostream>

int Trade::propability()
{
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(10000, 2000);
    int number = distribution(generator);
    if (number < 0) number = 0;
    if (number > 100000) number = 100000;
    return number;
}

bool Trade::is_offert_accepted(int price)
{
    double difference = abs(price - target_value);
    double sigma = 1.0;
    double mu = 0.0;

    // Obliczamy wartość funkcji gęstości prawdopodobieństwa
    double pdfValue = (1.0 / (sigma * sqrt(2.0 * M_PI))) * exp(-0.5 * pow((difference - mu) / sigma, 2));

    // Generujemy losową liczbę z zakresu 0.0 - 1.0
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double randomValue = distribution(generator);

    // Jeśli wylosowana liczba jest mniejsza od wartości PDF, zwracamy true
    if (randomValue < pdfValue) {
        return true;
    } else {
        return false;
    }

}

Trade::Trade()
{

}

bool Trade::player_offer(std::vector<int> letter_to_sell, int price_for_letters)
{
    int sum = 0;
    int minus100 = 1;
    float minus50 = 1;
    for(Letter* letter : player_Letters){
        for(int nr : letter_to_sell){
            if(letter->getNr() != nr){
                if(letter->get_value() ==  -100){
                    minus100 = 0;
                } else if(letter->get_value() ==  -50){
                    minus50 = 0.5;
                } else {
                    sum += letter->get_value();
                }
            }
        }
    }
    return is_offert_accepted( (sum)*(minus100)*(minus50) + price_for_letters );
}

void Trade::print_picked_letters()
{
    std::cout << "letter1: " + std::to_string(letter1) +
    " letter2: " + std::to_string(letter2) +
    " letter3: " + std::to_string(letter3) +
    " letter4: " + std::to_string(letter4) +
    " letter5: " + std::to_string(letter5) << std::endl;
    std::cout << "trade price: " + std::to_string(trade_price) << std::endl;
}

void Trade::set_player_letters(std::vector<Letter *> player_Letters)
{
    this->player_Letters = player_Letters;
}

void Trade::set_trade_price(int price)
{
    this->trade_price = price;
}

