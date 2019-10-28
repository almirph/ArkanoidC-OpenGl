#ifndef BLOCO_H_INCLUDED
#define BLOCO_H_INCLUDED

#include <iostream>
#include "Ponto.h"

using namespace std;

class Bloco {
    private:
        Ponto* p1;
        Ponto* p2;
        Ponto* p3;
        Ponto* p4;
        bool exibe;

    public:
        Bloco(Ponto p1, Ponto p2, Ponto p3, Ponto p4);
        Bloco();
        ~Bloco();
        Ponto* getP1();
        Ponto* getP2();
        Ponto* getP3();
        Ponto* getP4();
        bool getExibe();
        void setP1(Ponto p1);
        void setP2(Ponto p2);
        void setP3(Ponto p3);
        void setP4(Ponto p4);
        void setExibe(bool exibe);

        float color1;
        float color2;
        float color3;
};


#endif // BLOCO_H_INCLUDED
