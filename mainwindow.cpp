#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "letter.h"
#include <iostream>
#include <random>
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

    vector<Letter*> letters;
    vector<int> values = {100000,100000,-100,-100,-50,-50,50000,50000,10000,10000,5000,5000,2000,2000,
                      1000,1000,500,500,200,200,100,100,0,0,500,500,666};
    random_device rng;
    mt19937 g(rng());
    shuffle(values.begin(), values.end(), g);

    for(int val : values){
        cout << val << endl;
    }

    int i = 1;
    for (QPushButton* button : buttons) {
        //button->setText(QString::number(i));
        Letter *letter = new Letter(button, values[i]);
        connect(button, &QPushButton::clicked, [this, letter](bool) {this->onAnswerPressed(letter);});
        i++;
        letters.push_back(letter);
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
        cout << "udalo ci sie wybrac disu" << endl;
    }
}
