#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "letter.h"
#include <iostream>
#include <random>
#include <fstream>
#include <windows.h>
#include <QFile>
#include <QTextStream>
#include <string>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<QPushButton*> buttons = {
        ui->pushButton, ui->pushButton_2, ui->pushButton_3,
        ui->pushButton_4, ui->pushButton_5, ui->pushButton_6,
        ui->pushButton_7, ui->pushButton_8, ui->pushButton_9,
        ui->pushButton_10, ui->pushButton_11, ui->pushButton_12,
        ui->pushButton_13, ui->pushButton_14, ui->pushButton_15,
        ui->pushButton_16, ui->pushButton_17, ui->pushButton_18,
        ui->pushButton_19, ui->pushButton_20, ui->pushButton_21,
        ui->pushButton_22, ui->pushButton_23, ui->pushButton_24,
        ui->pushButton_25, ui->pushButton_26, ui->pushButton_27,
    };

    connect(ui->acceptLetters, &QPushButton::clicked, [this](bool) {this->onAcceptPressed();});

    vector<int> values = {100000,100000,-100,-100,-50,-50,50000,50000,10000,10000,5000,5000,2000,2000,
                      1000,1000,500,500,200,200,100,100,0,0,500,500,666};
    random_device rng;
    mt19937 g(rng());
    shuffle(values.begin(), values.end(), g);

    for(int val : values){
        cout << val << endl;
    }


    QFile file(":/m/pytania.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Nie można otworzyć pliku!";
    }
    else {
        int i = 1;
        QTextStream in(&file);
        QString  line;
        for (QPushButton* button : buttons) {
            button->setText(QString::number(i));
            button->setStyleSheet("background-image: url(:/m/letter.png); background-repeat: no-repeat; background-position: center; font-size: 15px; text-align: bottom;");

            line = in.readLine();
            Letter *letter = new Letter(button, values[i],  line.toStdString());
            connect(button, &QPushButton::clicked, [this, letter](bool) {this->onAnswerPressed(letter);});
            i++;
            this->letters.push_back(letter);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAnswerPressed(Letter* letter)
{
    letter->handler();
    cout << letter->get_value() << endl;
}

void MainWindow::onAcceptPressed()
{
    if(Letter::pick_amount == 5){
        textLabelDisplay("Udalo ci sie wybrac disu");
        init_picked_letters();
        ui->stackedWidget->setCurrentIndex(1);
        setQuestion();

    }else if(Letter::pick_amount < 5){
        textLabelDisplay("Trzeba wybrac dokladnie 5 kopert!");

    }
}

void MainWindow::textLabelDisplay(string name)
{
    ui->textLabel->setText(QString::fromStdString(name));
    ui->textLabel->setWordWrap(true);
}

void MainWindow::init_picked_letters()
{
    for(Letter* letter: letters){
        if(letter->getIs_pressed()){
            picked_letters.push_back(letter);
        }
    }
}

void MainWindow::setQuestion(){
    ui->question->setText(QString::fromStdString(picked_letters[questionIndex]->getQuestion().getContent()));
    string line = picked_letters[questionIndex]->getQuestion().getContent() + " \n" +
    + "A: " + picked_letters[questionIndex]->getQuestion().answer_map["A"] + " \n" +
    + "B: " + picked_letters[questionIndex]->getQuestion().answer_map["B"] + " \n" +
    + "C: " + picked_letters[questionIndex]->getQuestion().answer_map["C"] + " \n" +
    + "D: " + picked_letters[questionIndex]->getQuestion().answer_map["D"] + " \n" ;
    ui->question->setText(QString::fromStdString(line));
    ui->question->setWordWrap(true);
    questionIndex++;
    if(questionIndex == 5){
        questionIndex = 0;
    }
}



void MainWindow::isCorrectAnswer(Letter* letter, string buttonLetter){
    if(letter->getQuestion().getCorrect_answer() == buttonLetter){
        letter->setWin();
    }
    setQuestion();
}

void MainWindow::on_pushButton_A_clicked()
{
    isCorrectAnswer(picked_letters[questionIndex - 1], "A");
}

void MainWindow::on_pushButton_B_clicked()
{
    isCorrectAnswer(picked_letters[questionIndex - 1], "B");
}

void MainWindow::on_pushButton_C_clicked()
{
    isCorrectAnswer(picked_letters[questionIndex - 1], "C");
}

void MainWindow::on_pushButton_D_clicked()
{
    isCorrectAnswer(picked_letters[questionIndex - 1], "D");
}

