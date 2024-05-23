#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <map>
#include <vector>

class Question
{
public:
    Question();
    Question(const std::string& data_to_split){parse_data(data_to_split);}

private:
    std::string number, content, correct_answer;
    std::map<std::string, std::string> answer_map;
    void parse_data(std::string line);
    std::vector<std::string> split(const std::string& str, char delimiter);
};

#endif // QUESTION_H
