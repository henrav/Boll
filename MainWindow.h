//
// Created by Henrik Ravnborg on 2023-12-29.
//

#ifndef UNTITLED5_MAINWINDOW_H
#define UNTITLED5_MAINWINDOW_H

#include <SFML/Window/Event.hpp>
#include <iostream>
#include "Bollar.h"
using namespace std;

class MainWindow {
private:
    sf::RenderWindow window;
    vector<Bollar*> bollar;
    float width = 1920;
    float height = 1010;
    sf::Clock tickRateClock;
    float tickRate = 128;
    float ballElasticity = 0.8;
    float gravityX = 0;
    float gravityY = 9.82;
    sf::Vector2f gravity = sf::Vector2f(gravityX, gravityY);
    float elasticity = 0.7;
    vector<vector<vector<Bollar*>>> map;
    sf::Clock addBallsClock;
    bool paused = false;
    int64_t fps = 0;
    sf::Clock fpsClock;
    int64_t totalfps = 0;
    sf::Clock totalfpsClock;


public:
    MainWindow();
    void run();
    void setup();
    void drawBollar();
    void updateBollar();
    void applyGravity();
    void checkEdges();
    void addBallsToMap();
    void addVectorsToMap();
    void clearMap();
    void checkCollision0X_2X();
    void checkCollision2X_4X();
    void checkCollision4X_7X();
    static void physicsCollision(Bollar &boll, Bollar &boll2);
    void fixClumping(Bollar &boll, Bollar &boll2, float distance);
    void fixClumping2(Bollar &boll);
    void addBollar();
    static vector<int> getAreasInThirdOfWidth(int x);
    void drawAreas();
    void drawArea(int x, int y);
    void drawBollArea();
    void threadFunc1();
    void threadFunc2();
    void threadFunc3();
    void errorAreaNR();
    vector<Bollar*> getFiveClosestBalls(Bollar *boll);


};


#endif //UNTITLED5_MAINWINDOW_H
