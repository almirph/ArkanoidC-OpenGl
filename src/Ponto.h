#ifndef PONTO_H_INCLUDED
#define PONTO_H_INCLUDED
#include <iostream>

using namespace std;

class Ponto {
    private:
        float x;
        float y;
    public:
        Ponto(float x, float y);
        Ponto();
        ~Ponto();
        float getX();
        float getY();
        void setX(float x);
        void setY(float y);
};

#endif // PONTO_H_INCLUDED
