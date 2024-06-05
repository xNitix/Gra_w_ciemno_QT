#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "letter.h"
#include <iostream>
#include <random>
#include <windows.h>
#include <QFile>
#include <QTextStream>
#include <string>
#include <QTimer>
#include <QPainter>
#include "trade.h"

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
                      1000,1000,500,500,200,200,100,100,0,0,500,500,2000};
    random_device rng;
    mt19937 g(rng());
    shuffle(values.begin(), values.end(), g);

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
            Letter *letter = new Letter(button, values[i],  line.toStdString(), i);
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
}

void MainWindow::onAcceptPressed()
{
    if(Letter::pick_amount == 5){
        textLabelDisplay("Udalo ci sie wybrac! Pierwszy sukces!");
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
    ui->question->setText(QString::fromStdString(picked_letters[questionIndex%5]->getQuestion().getContent()));
    string line = picked_letters[questionIndex%5]->getQuestion().getContent() + " \n" +
    + "A: " + picked_letters[questionIndex%5]->getQuestion().answer_map["A"] + " \n" +
    + "B: " + picked_letters[questionIndex%5]->getQuestion().answer_map["B"] + " \n" +
    + "C: " + picked_letters[questionIndex%5]->getQuestion().answer_map["C"] + " \n" +
    + "D: " + picked_letters[questionIndex%5]->getQuestion().answer_map["D"] + " \n" ;
    ui->question->setText(QString::fromStdString(line));
    ui->question->setWordWrap(true);

    paintLettersColors();

    buttonsBlockade = false;
    questionIndex++;
}

void MainWindow::paintLettersColors(){
    lettersUI(picked_letters[(0 + questionIndex)%5], ui->picked_letter);
    lettersUI(picked_letters[(1 + questionIndex)%5], ui->letter1);
    lettersUI(picked_letters[(2 + questionIndex)%5], ui->letter2);
    lettersUI(picked_letters[(3 + questionIndex)%5], ui->letter3);
    lettersUI(picked_letters[(4 + questionIndex)%5], ui->letter4);
}

void MainWindow::lettersUI(Letter* letter, QLabel* qLabel){
    QPixmap originalPixmap;
    if(letter->getWin() == 1){
        originalPixmap = QPixmap(":/m/letter_green.png");
    } else if(letter->getWin() == 0) {
        originalPixmap = QPixmap(":/m/letter_red.png");
    } else {
        originalPixmap = QPixmap(":/m/letter.png");
    }
    QFont font;
    font.setPointSize(45);
    QPainter painter(&originalPixmap);
    painter.setFont(font);
    QRect textRect(0, 0, originalPixmap.width(), originalPixmap.height());
    string text = std::to_string(letter->getNr());
    painter.drawText(textRect, Qt::AlignCenter | Qt::AlignBottom, QString::fromStdString(text));
    qLabel->setPixmap(originalPixmap);
}



void MainWindow::isCorrectAnswer(Letter* letter, string buttonLetter){
    bool win = false;
    if(letter->getQuestion().getCorrect_answer() == buttonLetter){
        letter->setWin();
        win = true;
    } else {
        letter->setLose();
    }
    showAnswer(win, letter);
}

void MainWindow::showAnswer(bool win, Letter* letter){
    ui->question->setText(QString::fromStdString("Twoja odpowiedz jest: ..."));
    ui->question->setWordWrap(true);
    QTimer::singleShot(timeToWait, this, [this, win, letter]() {
        if(win){
            ui->question->setText(QString::fromStdString("Prawidlowa! Mozesz zachowac swoja koperte!"));
            ui->question->setWordWrap(true);
        } else {
            ui->question->setText(QString::fromStdString("Niepoprawna! Prawidlowa odpowiedz to: " +
                letter->getQuestion().getCorrect_answer()) + " - niestety tracisz swoja koperte!");
            ui->question->setWordWrap(true);
        }
        if(questionIndex < 5){
            QTimer::singleShot(timeToWait, this, [this, win, letter]() {
                setQuestion();
            });
        }
        else{
            paintLettersColors();
            QTimer::singleShot(timeToWait, this, [this, win, letter]() {
                int correct_answears = countGoodAnswers();
                if (correct_answears > 0){
                    ui->question->setText(QString::fromStdString("Koniec fazy pytan! \nUdalo ci sie zdobyc\n " + std::to_string(correct_answears) + "/5 kopert!"));
                    QTimer::singleShot(timeToWait, this, [this, win, letter]() {
                        prepareTradeScreen();
                    });

                }
                else{
                    ui->question->setText(QString::fromStdString("Koniec gry!\nPocwicz umiejetnosci c++!"));
                }

            });
        }

    });
}

int MainWindow::countGoodAnswers(){
    int res=0;
    for(Letter* letter : picked_letters){
        res += letter->getWin();
    }
    return res;
}

void MainWindow::prepareTradeScreen()
{
    ui->stackedWidget->setCurrentIndex(2);
    setLettersNumbers();

    ui->accept_trade->setVisible(false);
    ui->cancel_trade->setVisible(false);

    int lose_amount = 5 - countGoodAnswers();
    if(lose_amount > 0){
        lose_amount--;
        delete ui->my_letter_5;
        delete ui->butt5;
    }
    if(lose_amount > 0){
        lose_amount--;
        delete ui->my_letter_4;
        delete ui->butt4;
    }
    if(lose_amount > 0){
        lose_amount--;
        delete ui->my_letter_3;
        delete ui->butt3;
    }
    if(lose_amount > 0){
        lose_amount--;
        delete ui->my_letter_2;
        delete ui->butt2;
    }
    if(lose_amount > 0){
        lose_amount--;
        delete ui->my_letter_1;
        delete ui->butt1;
    }

    std::vector<Letter*> won_letters;
    for(Letter* letter : picked_letters){
        if(letter->getWin() == 1){
            won_letters.push_back(letter);
        }
    }
    this->trade->set_player_letters(won_letters);
    this->trade->init_letters_map();
}

void MainWindow::setLettersNumbers()
{
    std::vector<Letter*> won_letters;
    for(Letter* letter : picked_letters){
        if(letter->getWin() == 1){
            won_letters.push_back(letter);
        }
    }

    int index = 0;
    if(index < won_letters.size()){
        drawLettersUITradeMode(won_letters[index], ui->my_letter_1);
        index++;
    }
    if(index < won_letters.size()){
        drawLettersUITradeMode(won_letters[index], ui->my_letter_2);
        index++;
    }
    if(index < won_letters.size()){
        drawLettersUITradeMode(won_letters[index], ui->my_letter_3);
        index++;
    }
    if(index < won_letters.size()){
        drawLettersUITradeMode(won_letters[index], ui->my_letter_4);
        index++;
    }
    if(index < won_letters.size()){
        drawLettersUITradeMode(won_letters[index], ui->my_letter_5);
    }
    ui->money->setVisible(false);
}


void MainWindow::drawLettersUITradeMode(Letter* letter, QLabel* qLabel){
    QPixmap originalPixmap = QPixmap(":/m/letter_90.png");
    QFont font;
    font.setPointSize(45);
    QPainter painter(&originalPixmap);
    painter.setFont(font);
    QRect textRect(0, 0, originalPixmap.width(), originalPixmap.height());
    string text = std::to_string(letter->getNr());
    painter.drawText(textRect, Qt::AlignCenter | Qt::AlignBottom, QString::fromStdString(text));
    qLabel->setPixmap(originalPixmap);

}

void MainWindow::set_and_draw_player_money(){
    QPixmap originalPixmap = QPixmap(":/m/block_of_money.png");
    QFont font;
    font.setPointSize(70);
    QPainter painter(&originalPixmap);
    painter.setFont(font);
    QRect textRect(0, 0, originalPixmap.width(), originalPixmap.height());
    string text = std::to_string(trade->player_money);
    painter.save(); // Zapisz bieżący stan malowania
    painter.translate(textRect.center()); // Przesuń punkt (0,0) do środka prostokąta
    painter.rotate(-90); // Obróć o 90 stopni w lewo
    painter.translate(-textRect.center()); // Przesuń punkt (0,0) z powrotem
    painter.drawText(textRect, Qt::AlignCenter, QString::fromStdString(text));
    painter.restore();
    ui->money->setPixmap(originalPixmap);
}

void MainWindow::prepareEndScreen()
{
    QFont font;
    font.setPointSize(38);
    ui->label_win->setFont(font);
    ui->label_win->setAlignment(Qt::AlignCenter);

    int res = trade->getPlayer_money();
    int decrease_100_multiplier = 1;
    double decrease_50_multiplier = 1;

    int player_letters_amount = trade->getPlayer_Letters().size();
    int letters_total_amount = player_letters_amount + trade->getHost_Letters().size();
    std::vector<int> letter_numbers;
    std::vector<int> letter_values;
    for(Letter* letter : trade->getPlayer_Letters()){
        if(letter->get_value() == -100){
            decrease_100_multiplier = 0;
        }
        else if(letter->get_value() == -100){
            decrease_50_multiplier /= 2;
        }
        else{
            res += letter->get_value();
        }

        letter_numbers.push_back(letter->getNr());
        letter_values.push_back(letter->get_value());
    }
    for(Letter* letter : trade->getHost_Letters()){
        letter_numbers.push_back(letter->getNr());
        letter_values.push_back(letter->get_value());
    }

    int index = 0;
    bool is_green = true;
    if(player_letters_amount >= 0){
        player_letters_amount--;
        if(player_letters_amount < 0){
            is_green = false;
        }
    }
    drawEndLetter(letter_values[index], letter_numbers[index], ui->last_letter1, is_green);
    index++;
    if(index > letters_total_amount){delete ui->last_letter1;}

    if(player_letters_amount >= 0){
        player_letters_amount--;
        if(player_letters_amount < 0){
            is_green = false;
        }
    }
    drawEndLetter(letter_values[index], letter_numbers[index], ui->last_letter2, is_green);
    index++;
    if(index > letters_total_amount){delete ui->last_letter2;}

    if(player_letters_amount >= 0){
        player_letters_amount--;
        if(player_letters_amount < 0){
            is_green = false;
        }
    }
    drawEndLetter(letter_values[index], letter_numbers[index], ui->last_letter3, is_green);
    index++;
    if(index > letters_total_amount){delete ui->last_letter3;}

    if(player_letters_amount >= 0){
        player_letters_amount--;
        if(player_letters_amount < 0){
            is_green = false;
        }
    }
    drawEndLetter(letter_values[index], letter_numbers[index], ui->last_letter4, is_green);
    index++;
    if(index > letters_total_amount){delete ui->last_letter4;}

    if(player_letters_amount >= 0){
        player_letters_amount--;
        if(player_letters_amount < 0){
            is_green = false;
        }
    }
    drawEndLetter(letter_values[index], letter_numbers[index], ui->last_letter5, is_green);
    index++;
    if(index > letters_total_amount){delete ui->last_letter5;}

    cout << "RES PRZED DEBUFEM: " << res << endl;
    res = res * decrease_100_multiplier * decrease_50_multiplier;
    cout << "RES PO DEBUFIE: " << res << endl;
    string result = "Wygrałeś łącznie: " + to_string(res) + " zł!";
    ui->label_win->setText(QString::fromStdString(result));
}
void MainWindow::drawEndLetter(int value, int nr, QLabel* qLabel, bool is_green) {
    QPixmap originalPixmap = QPixmap(":/end_letter_green.png");
    if (!is_green) {
        originalPixmap = QPixmap(":/end_letter_red.png");
    }

    // Dodajemy tekst na środku u góry obrazka
    QFont font;
    font.setPointSize(35);
    QPainter painter(&originalPixmap);
    painter.setFont(font);

    // Ustawienia dla tekstu na górze
    QRect topTextRect(0, 0, originalPixmap.width(), originalPixmap.height() / 2);
    QString topText = QString::fromStdString(std::to_string(value));
    painter.drawText(topTextRect, Qt::AlignCenter | Qt::AlignTop, topText);

    // Ustawienia dla tekstu na dole
    QRect bottomTextRect(0, originalPixmap.height() / 2, originalPixmap.width(), originalPixmap.height() / 2);
    QString bottomText = QString::fromStdString(std::to_string(nr));
    painter.drawText(bottomTextRect, Qt::AlignCenter | Qt::AlignBottom, bottomText);

    qLabel->setPixmap(originalPixmap);
}

void MainWindow::on_pushButton_A_clicked()
{
    if(!buttonsBlockade){
        buttonsBlockade = true;
        isCorrectAnswer(picked_letters[questionIndex - 1], "A");
    }
}

void MainWindow::on_pushButton_B_clicked()
{
    if(!buttonsBlockade){
        buttonsBlockade = true;
        isCorrectAnswer(picked_letters[questionIndex - 1], "B");
    }
}

void MainWindow::on_pushButton_C_clicked()
{
    if(!buttonsBlockade){
        buttonsBlockade = true;
        isCorrectAnswer(picked_letters[questionIndex - 1], "C");
    }
}

void MainWindow::on_pushButton_D_clicked()
{
    if(!buttonsBlockade){
        buttonsBlockade = true;
        isCorrectAnswer(picked_letters[questionIndex - 1], "D");
    }
}


void MainWindow::on_pushButton_end_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    prepareEndScreen();
}


void MainWindow::on_butt1_pressed()
{
    trade->chosenLetter[1] = !trade->chosenLetter[1];
}


void MainWindow::on_butt2_pressed()
{
    trade->chosenLetter[2] = !trade->chosenLetter[2];
}


void MainWindow::on_butt3_pressed()
{
    trade->chosenLetter[3] = !trade->chosenLetter[3];
}


void MainWindow::on_butt4_pressed()
{
    trade->chosenLetter[4] = !trade->chosenLetter[4];
}


void MainWindow::on_butt5_pressed()
{
    trade->chosenLetter[5] = !trade->chosenLetter[5];
}


void MainWindow::on_pushButton_sell_clicked()
{
    vector<Letter*> host_before_trade_letters = trade->getHost_Letters();
    trade->set_trade_price(ui->spinBox->value());
    trade->start_trade();
    if(trade->wasTradeAccepted){
        moveLetterToHostOnScreen(host_before_trade_letters);
        if(trade->player_money!=0){
            ui->money->setVisible(true);
            set_and_draw_player_money();
        } else {
            ui->money->setVisible(false);
        }
    }
    check_to_end_game();
}

void MainWindow::check_to_end_game(){
    if(trade->getPlayer_Letters().size() == 0){
        ui->stackedWidget->setCurrentIndex(3);
        prepareEndScreen();
    }
}

void MainWindow::moveLetterToHostOnScreen(std::vector<Letter*> host_before_trade_letters){
    std::vector<Letter*> host_letters = trade->getHost_Letters();
    for(Letter* letter : host_letters){
        bool to_be_changed = true;
        for(Letter* letter2 : host_before_trade_letters){
            if(letter->getNr() == letter2->getNr()){
                to_be_changed = false;
            }
        }
        if(to_be_changed){
            for (const auto& pair : trade->index_to_letter_number_map) {
                if (pair.second == letter->getNr()) {
                    int offset = 430;
                    if(pair.first == 1){
                        ui->my_letter_1->move(ui->my_letter_1->x() + offset, ui->my_letter_1->y());
                        ui->butt1->setVisible(false);
                        trade->chosenLetter[1] = false;
                        //ui->butt1->move(ui->butt1->x() + offset, ui->butt1->y());
                    }
                    if(pair.first == 2){
                        ui->my_letter_2->move(ui->my_letter_2->x() + offset, ui->my_letter_2->y());
                        ui->butt2->setVisible(false);
                        trade->chosenLetter[2] = false;
                        //ui->butt2->move(ui->butt2->x() + offset, ui->butt2->y());
                    }
                    if(pair.first == 3){
                        ui->my_letter_3->move(ui->my_letter_3->x() + offset, ui->my_letter_3->y());
                        ui->butt3->setVisible(false);
                        trade->chosenLetter[3] = false;
                        //ui->butt3->move(ui->butt3->x() + offset, ui->butt3->y());
                    }
                    if(pair.first == 4){
                        ui->my_letter_4->move(ui->my_letter_4->x() + offset, ui->my_letter_4->y());
                        ui->butt4->setVisible(false);
                        trade->chosenLetter[4] = false;
                        //ui->butt4->move(ui->butt4->x() + offset, ui->butt4->y());
                    }
                    if(pair.first == 5){
                        ui->my_letter_5->move(ui->my_letter_5->x() + offset, ui->my_letter_5->y());
                        trade->chosenLetter[5] = false;
                        ui->butt5->setVisible(false);
                        //ui->butt5->move(ui->butt5->x() + offset, ui->butt5->y());
                    }
                }
            }
        }
    }
}

void MainWindow::moveLetterToPlayerOnScreen(std::vector<Letter*> player_before_trade_letters){
    std::vector<Letter*> player_letters = trade->getPlayer_Letters();
    for(Letter* letter : player_letters){
        bool to_be_changed = true;
        for(Letter* letter2 : player_before_trade_letters){
            if(letter->getNr() == letter2->getNr()){
                to_be_changed = false;
            }
        }
        if(to_be_changed){
            for (const auto& pair : trade->index_to_letter_number_map) {
                if (pair.second == letter->getNr()) {
                    int offset = -430;
                    if(pair.first == 1){
                        ui->my_letter_1->move(ui->my_letter_1->x() + offset, ui->my_letter_1->y());
                        ui->butt1->setVisible(true);
                        trade->chosenLetter[1] = false;
                        //ui->butt1->move(ui->butt1->x() + offset, ui->butt1->y());
                    }
                    if(pair.first == 2){
                        ui->my_letter_2->move(ui->my_letter_2->x() + offset, ui->my_letter_2->y());
                        ui->butt2->setVisible(true);
                        trade->chosenLetter[2] = false;
                        //ui->butt2->move(ui->butt2->x() + offset, ui->butt2->y());
                    }
                    if(pair.first == 3){
                        ui->my_letter_3->move(ui->my_letter_3->x() + offset, ui->my_letter_3->y());
                        ui->butt3->setVisible(true);
                        trade->chosenLetter[3] = false;
                        //ui->butt3->move(ui->butt3->x() + offset, ui->butt3->y());
                    }
                    if(pair.first == 4){
                        ui->my_letter_4->move(ui->my_letter_4->x() + offset, ui->my_letter_4->y());
                        ui->butt4->setVisible(true);
                        trade->chosenLetter[4] = false;
                        //ui->butt4->move(ui->butt4->x() + offset, ui->butt4->y());
                    }
                    if(pair.first == 5){
                        ui->my_letter_5->move(ui->my_letter_5->x() + offset, ui->my_letter_5->y());
                        ui->butt5->setVisible(true);
                        trade->chosenLetter[5] = false;
                        //ui->butt5->move(ui->butt5->x() + offset, ui->butt5->y());
                    }
                }
            }
        }
    }
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    trade->set_trade_price(ui->spinBox->value());
}

void MainWindow::on_spinBox_textChanged(const QString &arg1){}
void MainWindow::on_spinBox_editingFinished(){}
void MainWindow::on_butt1_released(){}
void MainWindow::on_butt2_released(){}
void MainWindow::on_butt3_released(){}
void MainWindow::on_butt4_released(){}
void MainWindow::on_butt5_released(){}
void MainWindow::on_pushButton_offer_clicked()
{
    trade->generujKombinacje();
    for (int element : trade->letter_give) {
        std::cout << element << " ";
    }
    cout << "co to jest za droznya " << endl;

    for (int element : trade->letter_take) {
        std::cout << element << " ";
    }
    cout << "co sledziem jebie tak ? " << endl;
    std::cout << trade->money_for_trade;
    std::cout << std::endl;


    ui->pushButton_sell->setDisabled(true);
    ui->pushButton_offer->setDisabled(true);
    ui->accept_trade->setVisible(true);
    ui->cancel_trade->setVisible(true);

    check_to_end_game();
}


void MainWindow::on_accept_trade_clicked()
{
    vector<Letter*> host_before_trade_letters = trade->getHost_Letters();
    vector<Letter*> player_before_trade_letters = trade->getPlayer_Letters();
    trade->acceptTrade();
    moveLetterToHostOnScreen(host_before_trade_letters);
    moveLetterToPlayerOnScreen(player_before_trade_letters);
    if(trade->player_money!=0){
        ui->money->setVisible(true);
        set_and_draw_player_money();
    } else {
        ui->money->setVisible(false);
    }

    check_to_end_game();
    ui->pushButton_sell->setDisabled(false);
    ui->pushButton_offer->setDisabled(false);
    ui->accept_trade->setVisible(false);
    ui->cancel_trade->setVisible(false);
}


void MainWindow::on_cancel_trade_clicked()
{
    trade->cancelTrade();
    ui->pushButton_sell->setDisabled(false);
    ui->pushButton_offer->setDisabled(false);
    ui->accept_trade->setVisible(false);
    ui->cancel_trade->setVisible(false);
}

