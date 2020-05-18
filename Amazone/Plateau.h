//
// Created by paul patault on 03/10/2019.
//
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
#ifndef AMAZSFML_PLATEAU_H
#define AMAZSFML_PLATEAU_H

enum Type{amazB, amazN, block, vide, possibleJ, possibleV};
enum Player {Blanc, Noir};
struct Position{
    int x;
    int y;
};

static const int width = 500;
static const int height = 500;

class Plateau {

    private:
        static const int TAILLE = 10;
        Type transcription[TAILLE][TAILLE];

    public:
        Plateau();
        bool changeAmaz(Player, Position);
        void initialise();
        bool d_poss(Position, Position);
        int nb_depPos(Player);
        void actualise(RenderWindow&);
        void fleche(Position);
        void afficheDPoss(Position, int);
        void majTab(Position, Position);
        void cancelDposs();
        Type getType(Position);
        bool videvide(int a, int b);
};

#endif //AMAZSFML_PLATEAU_H
