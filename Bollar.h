//
// Created by Henrik Ravnborg on 2023-12-29.
//

#ifndef UNTITLED5_BOLLAR_H
#define UNTITLED5_BOLLAR_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "SFML/Graphics/CircleShape.hpp"
static float thingy = -2;
static int thingtKeepinTrack = -1;
class Bollar {
private:
    sf::CircleShape circle;
    sf::Vector2f speedVector;
    sf::Vector2<int> areaNR;
    std::string areaNRstring;
    sf::Text text;
    sf::Font font;
    float mass;
public:
    Bollar(float radius, float x, float y, sf::Vector2f speedVector);
    Bollar();
    float getRadius();
    float getXpos();
    float getYpos();
    sf::Vector2f getSpeed();
    void setRadius(float radius);
    void setXpos(float x);
    void setYpos(float y);
    void setSpeed(sf::Vector2f speedVector);
    void update();
    void update(float x, float y);
    void applyGravity(float x, float y);
    void draw(sf::RenderWindow &window);
    sf::CircleShape &getCircle();
    void checkEdges(float width, float height, float elasticity);
    void setAreaNR(sf::Vector2<int> areaNR);
    sf::Vector2<int> getAreaNR();
    void move(float x, float y, float windowWidth, float windowHeight);
    void setRotatingSpeed();
    void setMass(float mass);
    float getMass();
    void applyElasticity(float elasticity);
    void drawBollPos(sf::RenderWindow &window);
    void loadFont();





};


#endif //UNTITLED5_BOLLAR_H
