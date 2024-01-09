//
// Created by Henrik Ravnborg on 2023-12-29.
//

#include "Bollar.h"
#include <iostream>
Bollar::Bollar(float radius, float x, float y, sf::Vector2f velocity) {
    this->circle.setRadius(radius);
    this->circle.setPosition(x, y);
    this->speedVector.x = velocity.x;
    this->speedVector.y = velocity.y;
    this->mass = rand() % 10 + 1;
}

Bollar::Bollar() {
    this->circle = sf::CircleShape();
    this->circle.setRadius(10);
    this->circle.setPosition(50, 50);
    this->speedVector.x = 5;
    this->speedVector.y = 0;
    this->mass = 5;
    setRotatingSpeed();
    loadFont();
}

void Bollar::setRotatingSpeed() {
    if (thingtKeepinTrack == -1) {
        if (thingy > 1 ) {
            thingtKeepinTrack = 1;
        }else{
            this->speedVector.y = sin(thingy) * 3;
            thingy += 0.1;
        }
    }
    else if (thingtKeepinTrack == 1) {
        if (thingy < -1 ) {
            thingtKeepinTrack = -1;
        }else{
            this->speedVector.y = sin(thingy) * 3;
            thingy -= 0.1;
        }
    }

}

float Bollar::getRadius() {
    return this->circle.getRadius();
}

float Bollar::getXpos() {
    return this->circle.getPosition().x;
}

float Bollar::getYpos() {
    return this->circle.getPosition().y;
}

sf::Vector2f Bollar::getSpeed() {
    return this->speedVector;
}

void Bollar::setRadius(float radius) {
    this->circle.setRadius(radius);
}

void Bollar::setXpos(float x) {
    this->circle.setPosition(x, this->circle.getPosition().y);
}

void Bollar::setYpos(float y) {
    this->circle.setPosition(this->circle.getPosition().x, y);
}

void Bollar::setSpeed(sf::Vector2f speedVector) {
    this->speedVector.x = speedVector.x;
    this->speedVector.y = speedVector.y;
}

void Bollar::update() {
    this->circle.move(this->speedVector);
}

void Bollar::update(float x, float y) {
    this->circle.move(x, y);
}

void Bollar::applyGravity(float x, float y) {
    this->speedVector.x += x;
    this->speedVector.y += y;
}

void Bollar::draw(sf::RenderWindow &window) {
    window.draw(this->circle);
}

sf::CircleShape &Bollar::getCircle() {
    return this->circle;
}

void Bollar::checkEdges(float width, float height, float elasticity) {
    if (this->circle.getPosition().x + (this->circle.getRadius() * 2)> width) {
        this->circle.setPosition(width - (this->circle.getRadius()* 2), this->circle.getPosition().y);
        this->speedVector.x *= -elasticity;
    }
    if (this->circle.getPosition().x - this->circle.getRadius() < 0) {
        this->circle.setPosition(this->circle.getRadius(), this->circle.getPosition().y);
        this->speedVector.x *= -elasticity;
    }
    if (this->circle.getPosition().y + (this->circle.getRadius() * 2) > height) {
        this->circle.setPosition(this->circle.getPosition().x, height - (this->circle.getRadius()* 2));
        this->speedVector.y *= -elasticity;
    }
    if (this->circle.getPosition().y - this->circle.getRadius() < 0) {
        this->circle.setPosition(this->circle.getPosition().x, this->circle.getRadius());
        this->speedVector.y *= -elasticity;
    }
}

void Bollar::setAreaNR(sf::Vector2<int> areaNR) {
    this->areaNR = areaNR;
}

sf::Vector2<int> Bollar::getAreaNR() {
    return this->areaNR;
}

void Bollar::move(float x, float y, float windowWidth, float windowHeight) {
    if (this->circle.getPosition().x + x + (this->circle.getRadius() * 2) > windowWidth) {
        this->circle.move(0, y);
    }
    else if (this->circle.getPosition().x + x < 0) {
        this->circle.move(0, y);
    }
    else if (this->circle.getPosition().y + y + (this->circle.getRadius() * 2) > windowHeight) {
        this->circle.move(x, 0);
    }
    else if (this->circle.getPosition().y + y < 0) {
        this->circle.move(x, 0);
    }
    else {
        this->circle.move(x, y);
    }
}

void Bollar::setMass(float mass) {
    this->mass = mass;
}

float Bollar::getMass() {
    return this->mass;
}

void Bollar::applyElasticity(float elasticity) {
    this->speedVector.x *= elasticity;
    this->speedVector.y *= elasticity;
}

void Bollar::drawBollPos(sf::RenderWindow &window) {
    this->text.setString(std::to_string(this->areaNR.y) + " " + std::to_string(this->areaNR.x));
    this->text.setPosition(this->circle.getPosition().x + circle.getRadius() * 2, this->circle.getPosition().y);
    window.draw(this->text);
}

void Bollar::loadFont() {
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial Unicode.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }
    this->text.setFont(font);
    this->text.setCharacterSize(20);
    this->text.setFillColor(sf::Color::White);
}




