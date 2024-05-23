#include "question.h"
#include <sstream>
#include <iostream>>

Question::Question() {}

void Question::parse_data(std::string line)
{
    std::vector<std::string> result = split(line, ';');
    number = result.at(0);
    content = result.at(1);
    for(int i = 2; i <= 8; i += 2 ){
        answer_map[result.at(i)] = result.at(i+1);
    }
    correct_answer = result.at(10);
}

std::vector<std::string> Question::split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
