//
// Created by paul patault on 03/10/2019.
//
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
#ifndef AMAZSFML_PLATEAU_H
#define AMAZSFML_PLATEAU_H

static const int width = 800;
static const int height = 800;
static const int t = 40;

class Pool {

    private:
        int tab[t][t];
        int nbVoisins(Vector2i, int);

    public:
        Pool();
        void draw(RenderWindow&, int);
        void update(int);
        void clic(Vector2i);

};

#endif //AMAZSFML_PLATEAU_H
