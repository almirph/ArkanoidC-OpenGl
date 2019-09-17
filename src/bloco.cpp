#include <iostream>
#include "Bloco.h"

using namespace std;

Bloco::Bloco(Ponto p1, Ponto p2, Ponto p3, Ponto p4) {
    this->p1 = new Ponto();
    this->p1->setX(p1.getX());
    this->p1->setY(p1.getY());

    this->p2 = new Ponto();
    this->p2->setX(p2.getX());
    this->p2->setY(p2.getY());

    this->p3 = new Ponto();
    this->p3->setX(p3.getX());
    this->p3->setY(p3.getY());

    this->p4 = new Ponto();
    this->p4->setX(p4.getX());
    this->p4->setY(p4.getY());

    this->exibe = true;
}

Bloco::Bloco(){
    this->exibe = true;

    this->p1 = NULL;
    this->p2 = NULL;
    this->p3 = NULL;
    this->p4 = NULL;
}

Bloco::~Bloco() {
    delete this->p1;
    delete this->p2;
    delete this->p3;
    delete this->p4;
}

Ponto* Bloco::getP1() {
    return this->p1;
}

Ponto* Bloco::getP2() {
    return this->p2;
}

Ponto* Bloco::getP3() {
    return this->p3;
}

Ponto* Bloco::getP4() {
    return this->p4;
}

void Bloco::setP1(Ponto p1) {
    if(this->p1 == NULL) {
        this->p1 = new Ponto();
    }
    this->p1->setX(p1.getX());
    this->p1->setY(p1.getY());
}

void Bloco::setP2(Ponto p2) {
    if(this->p2 == NULL) {
        this->p2 = new Ponto();
    }
    this->p2->setX(p2.getX());
    this->p2->setY(p2.getY());
}

void Bloco::setP3(Ponto p3) {
    if(this->p3 == NULL) {
        this->p3 = new Ponto();
    }
    this->p3->setX(p3.getX());
    this->p3->setY(p3.getY());
}

void Bloco::setP4(Ponto p4) {
    if(this->p4 == NULL) {
        this->p4 = new Ponto();
    }
    this->p4->setX(p4.getX());
    this->p4->setY(p4.getY());
}

void Bloco::setExibe(bool exibe) {
    this->exibe = exibe;
}

bool Bloco::getExibe() {
    return this->exibe;
}
