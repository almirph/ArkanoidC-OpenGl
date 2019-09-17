#include <iostream>
#include "Ponto.h"

using namespace std;

Ponto::Ponto(float x, float y) {
    this->x = x;
    this->y = y;
}

Ponto::Ponto() { }

Ponto::~Ponto(){}

float Ponto::getX() {
    return this->x;
}

float Ponto::getY() {
    return this->y;
}

void Ponto::setX(float x) {
    this->x = x;
}

void Ponto::setY(float y) {
    this->y = y;
}
