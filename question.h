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
    std::map<std::string, std::string> answer_map;
    std::string getNumber() const;
    std::string getContent() const;
    std::string getCorrect_answer() const;

private:
    std::string number, content, correct_answer;
    void parse_data(std::string line);
    std::vector<std::string> split(const std::string& str, char delimiter);

};

#endif // QUESTION_H
