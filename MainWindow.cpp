//
// Created by Henrik Ravnborg on 2023-12-29.
//

#include "MainWindow.h"
#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
using namespace std;
MainWindow::MainWindow(){
    window.create(sf::VideoMode(width, height), "SFML works!");
}

void MainWindow::run() {
    sf::Thread thread1(&MainWindow::threadFunc1, this);
    sf::Thread thread2(&MainWindow::threadFunc2, this);
    sf::Thread thread3(&MainWindow::threadFunc3, this);

    setup();
    while (window.isOpen())
    {
        if (fpsClock.getElapsedTime().asSeconds() >= 0.2){
            cout << "FPS: " << fps * 5 << endl;
            fps = 0;
            fpsClock.restart();
        }
        window.clear();
        sf:: Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Escape){
                    if (paused) {
                        paused = false;
                        tickRateClock.restart();
                        tickRate = 128;
                    }
                    else {
                        paused = true;
                        tickRate = 0;
                    }
                }
            }
        }
        if (tickRateClock.getElapsedTime().asMilliseconds() >= int32_t(1000 / tickRate) && tickRate > 0) {
            if (addBallsClock.getElapsedTime().asMilliseconds() >= 150 && !paused){
                addBollar();

                addBallsClock.restart();
            }
            addBallsToMap();
            thread1.launch();
            thread2.launch();
            thread3.launch();
            applyGravity();
            checkEdges();
            updateBollar();
            tickRateClock.restart();
        }
        //drawBollArea();
        drawAreas();
        drawBollar();
        fps++;
        totalfps++;
        window.display();
    }
}

void MainWindow::threadFunc1() {
    checkCollision0X_2X();
}

void MainWindow::threadFunc2() {
    checkCollision2X_4X();
}

void MainWindow::threadFunc3() {
    checkCollision4X_7X();
}

void MainWindow::setup() {
    addVectorsToMap();
}

void MainWindow::addBollar() {
    auto *boll = new Bollar();
    bollar.push_back(boll);
    cout << bollar.size() << endl;

}

void MainWindow::drawBollar() {
    for (auto &boll : bollar) {
        boll->draw(window);
    }
}

void MainWindow::updateBollar() {
    for (auto &boll : bollar) {
        boll->update();
    }
}

void MainWindow::applyGravity() {
    for (auto &boll : bollar) {
        boll->applyGravity(0, gravityY / tickRate);
    }
}

void MainWindow::checkEdges() {
    for (auto &boll : bollar) {
        boll->checkEdges(width, height, elasticity);
    }
}

void MainWindow::addBallsToMap() {
    clearMap();
    for (auto &boll : bollar){
        int bolxarea = int(boll->getXpos() / (width / 8));
        int bolyarea = int(boll->getYpos() / (height / 8));
        if (bolxarea > 7){
            bolxarea = 7;
        }
        if (bolyarea > 7){
            bolyarea = 7;
        }
        if (bolxarea < 0){
            bolxarea = 0;
        }
        if (bolyarea < 0){
            bolyarea = 0;
        }
        boll->setAreaNR(sf::Vector2<int>(bolxarea, bolyarea));
        map[bolyarea][bolxarea].push_back(boll);
    }
}



void MainWindow::addVectorsToMap() {
    for (int i = 0; i < 8; i++){
        vector<vector<Bollar*>> temp;
        for (int j = 0; j < 8; j++){
            vector<Bollar*> temp2;
            temp2.reserve(10000);
            temp.push_back(temp2);
        }
        map.push_back(temp);
    }
}

void MainWindow::clearMap() {
    for (auto &i : map) {
        for (auto &j : i) {
            j.clear();
        }
    }
}

void MainWindow::checkCollision0X_2X() {

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 3; j++){
            for (auto &boll : map[i][j]) {
                if (boll == nullptr){
                    break;
                }
                for (auto &boll2 : map[i][j]) {
                    if (boll != boll2){
                        //calculate distance between balls
                        auto distance = float(sqrt(pow(boll->getXpos() - boll2->getXpos(), 2) + pow(boll->getYpos() - boll2->getYpos(), 2)));
                        //if distance is smaller than the sum of the radiuses, they intersect
                        if (distance < boll->getRadius() + boll2->getRadius()){
                            //if they intersect, fix clumping
                            fixClumping(*boll, *boll2, distance);
                            //calculate new speed
                            physicsCollision(*boll, *boll2);
                        }
                    }
                }
            }
        }
    }


    /*
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 2; j++){
            for (int k = 0; k < map[i][j].size(); k++){
                for (int l = 0; l < map[i][j].size(); l++){
                    if (map[i][j][k] && map[i][j][l]){
                        if (map[i][j][k]->getXpos() != map[i][j][l]->getXpos() && map[i][j][k]->getYpos() != map[i][j][l]->getYpos()){
                            if (map[i][j][k]->getCircle().getGlobalBounds().intersects(map[i][j][l]->getCircle().getGlobalBounds())){
                                fixClumping(map[i][j][k], map[i][j][l]);
                            }
                        }
                    }
                }
            }
        }
    }
     */
}

void MainWindow::checkCollision2X_4X() {
    for (int i = 0; i < 8; i++){
        for (int j = 3; j < 5; j++){
            for (auto &boll : map[i][j]) {
                for (auto &boll2: map[i][j]) {
                    if (boll != boll2) {
                        //calculate distance between balls
                        auto distance = float(sqrt(pow(boll->getXpos() - boll2->getXpos(), 2) +
                                              pow(boll->getYpos() - boll2->getYpos(), 2)));
                        //if distance is smaller than the sum of the radiuses, they intersect
                        if (distance < boll->getRadius() + boll2->getRadius()) {
                            //fix clumping
                            fixClumping(*boll, *boll2, distance);
                            //calculate new speed
                            physicsCollision(*boll, *boll2);
                        }
                    }
                }
            }
        }
    }




    /*
    for (int i = 0; i < 8; i++){
        for (int j = 2; j < 4; j++){
            for (int k = 0; k < map[i][j].size(); k++){
                for (int l = 0; l < map[i][j].size(); l++){
                    if (map[i][j][k] && map[i][j][l]){
                        if (map[i][j][k]->getXpos() != map[i][j][l]->getXpos() && map[i][j][k]->getYpos() != map[i][j][l]->getYpos()){
                            if (map[i][j][k]->getCircle().getGlobalBounds().intersects(map[i][j][l]->getCircle().getGlobalBounds())){
                                fixClumping(map[i][j][k], map[i][j][l]);
                            }
                        }
                    }
                }
            }
        }
    }
     */
}

void MainWindow::checkCollision4X_7X() {

    for (int i = 0; i < 8; i++) {
        for (int j = 5; j < 8; j++) {
           for (auto &boll : map[i][j]) {
               for (auto &boll2 : map[i][j]) {
                   if (boll != boll2){
                       //calculate distance between balls
                       auto distance = float(sqrt(pow(boll->getXpos() - boll2->getXpos(), 2) + pow(boll->getYpos() - boll2->getYpos(), 2)));
                       //if distance is smaller than the sum of the radiuses, they intersect
                       if (distance < boll->getRadius() + boll2->getRadius()) {
                           //fix clumping
                           fixClumping(*boll, *boll2, distance);
                           //calculate new speed
                           physicsCollision(*boll, *boll2);
                       }
                   }
               }
           }
        }
    }

}

void MainWindow::fixClumping(Bollar &boll, Bollar &boll2, float distance) {
    float overlap = 0.5 * (distance - boll.getRadius() - boll2.getRadius());
    float angle = atan2(boll2.getYpos() - boll.getYpos(), boll2.getXpos() - boll.getXpos());
    float targetX = overlap * cos(angle);
    float targetY = overlap * sin(angle);
    for (int i = 0; i < 8; i++){
        boll2.move(-targetX / 16, -targetY / 16, width, height);
        boll.move(targetX / 16, targetY / 16, width, height);
        fixClumping2(boll);
        fixClumping2(boll2);
    }


}

void MainWindow::fixClumping2(Bollar &boll) {
    int bolxarea = boll.getAreaNR().x;
    int bolyarea = boll.getAreaNR().y;

    for (auto & i : map[bolyarea][bolxarea]){
        if (&boll != i){
            //calculate distance between balls
            auto distance = float(sqrt(pow(boll.getXpos() - i->getXpos(), 2) + pow(boll.getYpos() - i->getYpos(), 2)));
            //if distance is smaller than the sum of the radiuses, they intersect
            if (distance < boll.getRadius() + i->getRadius()) {
                //fix clumping
                if (boll.getCircle().getGlobalBounds().intersects(i->getCircle().getGlobalBounds())){
                    auto overlap = float(0.5 * (distance - boll.getRadius() - i->getRadius()));
                    float angle = atan2(i->getYpos() - boll.getYpos(), i->getXpos() - boll.getXpos());
                    float targetX = overlap * cos(angle);
                    float targetY = overlap * sin(angle);
                        boll.move(targetX, targetY, width, height);
                        i->move(-targetX, -targetY, width, height);
                    physicsCollision(boll, *i);

                }
            }
        }
    }


    /*
    for_each(map[boll->getAreaNR().y][boll->getAreaNR().x].begin(), map[boll->getAreaNR().y][boll->getAreaNR().x].end(), [&](Bollar *boll2){
        if (boll2 != nullptr){
            if (boll != boll2){
                if (boll->getCircle().getGlobalBounds().intersects(boll2->getCircle().getGlobalBounds())){
                    float distance = sqrt(pow(boll->getXpos() - boll2->getXpos(), 2) + pow(boll->getYpos() - boll2->getYpos(), 2));
                    float overlap = 0.5 * (distance - boll->getRadius() - boll2->getRadius());
                    float angle = atan2(boll2->getYpos() - boll->getYpos(), boll2->getXpos() - boll->getXpos());
                    float targetX = overlap * cos(angle);
                    float targetY = overlap * sin(angle);
                    boll->move(targetX / 16, targetY / 16);
                    boll2->move(-targetX / 16, -targetY / 16);
                }
            }
        }
    });
     */

}

void MainWindow::physicsCollision(Bollar &boll, Bollar &boll2) {

    float angle = atan2(boll.getYpos() - boll2.getYpos(), boll.getYpos() - boll2.getXpos());
    float speed1 = float(sqrt(pow(boll.getSpeed().x, 2) + pow(boll.getSpeed().y, 2)));
    float speed2 = float(sqrt(pow(boll2.getSpeed().x, 2) + pow(boll2.getSpeed().y, 2)));
    float direction1 = atan2(boll.getSpeed().y, boll.getSpeed().x);
    float direction2 = atan2(boll2.getSpeed().y, boll2.getSpeed().x);

    float new_xspeed_1 = speed1 * cos(direction1 - angle);
    float new_yspeed_1 = speed1 * sin(direction1 - angle);
    float new_xspeed_2 = speed2 * cos(direction2 - angle);
    float new_yspeed_2 = speed2 * sin(direction2 - angle);
    float final_xspeed_1 = ((boll.getMass() - boll2.getMass()) * new_xspeed_1 + (boll2.getMass() + boll2.getMass()) * new_xspeed_2) / (boll.getMass() + boll2.getMass());
    float final_xspeed_2 = ((boll.getMass() + boll.getMass()) * new_xspeed_1 + (boll2.getMass() - boll.getMass()) * new_xspeed_2) / (boll.getMass() + boll2.getMass());
    float final_yspeed_1 = new_yspeed_1;
    float final_yspeed_2 = new_yspeed_2;

    boll.setSpeed(sf::Vector2f(float(cos(angle) * final_xspeed_1 + cos(angle + M_PI / 2) * final_yspeed_1),
                                float(sin(angle) * final_xspeed_1 + sin(angle + M_PI / 2) * final_yspeed_1)));
    boll2.setSpeed(sf::Vector2f(float(cos(angle) * final_xspeed_2 + cos(angle + M_PI / 2) * final_yspeed_2),
                                 float(sin(angle) * final_xspeed_2 + sin(angle + M_PI / 2) * final_yspeed_2)));

    boll.applyElasticity(0.9999);
    boll2.applyElasticity(0.9999);




}

vector<int> MainWindow::getAreasInThirdOfWidth(int x) {
    //window width = width
    //width / 3
    //if int = 1, return first third
    //if int = 2, return second third
    //if int = 3, return third third of areas in width
    if (int x = 1){

    }


}

void MainWindow::drawAreas() {
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(width / 8, height / 8));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1);
    sf::Text text;
    sf::Font font;
    font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Unicode.ttf");
text.setFont(font);
text.setCharacterSize(24);
text.setFillColor(sf::Color::Red);
text.setStyle(sf::Text::Bold);

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            rectangle.setPosition(i * (width / 8), j * (height / 8));
            text.setPosition(i * (width / 8), j * (height / 8));
            text.setString(to_string(j ) + " " + to_string(i ));
            window.draw(text);
            window.draw(rectangle);
        }
    }
}

void MainWindow::drawArea(int x, int y) {
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(width / 8, height / 8));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(x * (width / 8), y * (height / 8));
    window.draw(rectangle);
}

void MainWindow::drawBollArea() {

    for (auto &boll : bollar){
        boll->drawBollPos(window);
    }

    /*
    sf::Text text;
    sf::Font font;
    font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Unicode.ttf");
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    for (auto &boll : bollar){
        text.setPosition(boll->getXpos(), boll->getYpos());
        text.setString(to_string(boll->getAreaNR().y) + " " + to_string(boll->getAreaNR().x));
        text.move(boll->getCircle().getRadius() * 2, 0);
        window.draw(text);
    }
     */
}

vector<Bollar*> MainWindow::getFiveClosestBalls(Bollar *boll) {

}

