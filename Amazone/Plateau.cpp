//
// Created by paul patault on 03/10/2019.
//
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
using namespace std;
using namespace sf;
#include "Plateau.h"

Plateau::Plateau()
{
    for(auto & i : this->transcription){  // <=> for(int i = 0; i < TAILLE; i++){
        for(auto & j : i){                // <=> for(int j = 0; j < TAILLE; j++){
            j = vide;
        }
    }
}

void Plateau::actualise(RenderWindow& w)
{
    w.clear();

    float widthDec = width / 10;
    float heightDec = height / 10;
    float lambda = widthDec / 50;
    double radius = 18 * lambda;

    Color c_vert = Color(111,153,102 );
    Color c_beige = Color(206,213,198);
    Color c_jaune = Color(195,195,0,160);
    Color c_violet = Color(185,0,195,110);
    Color c_red = Color(255, 100,0);

    RectangleShape rectangle(sf::Vector2f(widthDec, heightDec));
    RectangleShape rectangle_fond(sf::Vector2f(width, height));
    {
        rectangle_fond.setFillColor(Color(111,153,102 ));
        w.draw(rectangle_fond);
    }

    CircleShape cercleB(radius, 5);
    {
        cercleB.setFillColor(Color::White);
        cercleB.setOutlineThickness(1);
        cercleB.setOutlineColor(Color::Black);
    }
    CircleShape cercleN(radius, 6);
    {
        cercleN.setFillColor(Color::Black);
    }

    for(int i = 0; i < TAILLE; i++)
    {
        for(int j = 0; j < TAILLE; j++)
        {
            if((i + j) % 2)
            {
                rectangle.setFillColor(c_beige) ;
                rectangle.setPosition(i * widthDec,j * heightDec);
                w.draw(rectangle);
            }
            switch (this->transcription[i][j]){
                case amazB:
                {
                    cercleB.setPosition(i * widthDec + 7,j * heightDec + 7);
                    w.draw(cercleB);
                    break;
                }
                case amazN:
                {
                    cercleN.setPosition(i * widthDec + 7,j * heightDec + 7);
                    w.draw(cercleN);
                    break;
                }
                case block:
                {
                    rectangle.setFillColor(c_red);
                    rectangle.setPosition(i * widthDec,j * heightDec);
                    w.draw(rectangle);
                    break;
                }
                case possibleJ:
                {
                    rectangle.setFillColor(c_jaune);
                    rectangle.setPosition(i * widthDec,j * heightDec);
                    w.draw(rectangle);
                    break;
                }
                case possibleV:
                {
                    rectangle.setFillColor(c_violet);
                    rectangle.setPosition(i * widthDec,j * heightDec);
                    w.draw(rectangle);
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void Plateau::initialise()
{
    this->transcription[0][3] = amazN;
    this->transcription[3][0] = amazN;
    this->transcription[6][0] = amazN;
    this->transcription[9][3] = amazN;

    this->transcription[0][6] = amazB;
    this->transcription[9][6] = amazB;
    this->transcription[3][9] = amazB;
    this->transcription[6][9] = amazB;
}

void Plateau::majTab(Position deb, Position fin) {
    this->transcription[fin.x][fin.y] = getType(deb);
    this->transcription[deb.x][deb.y] = vide;
}

void Plateau::fleche(Position p) {
    this->transcription[p.x][p.y] = block;
}

bool Plateau::videvide(int a, int b)
{
    if(this->transcription[a][b] == amazB)
        return false;
    if(this->transcription[a][b] == amazN)
        return false;
    if(this->transcription[a][b] == block)
        return false;
    return true;
}

bool Plateau::d_poss(Position deb, Position fin) {
    if ((deb.y == fin.y) and (deb.x == fin.x)) return false;

    if (deb.y == fin.y)  /// MEME LIGNE ///
    {
        if (deb.x < fin.x)
        {
            for (int i = deb.x + 1; i <= fin.x; ++i) {
                if (not this->videvide(i, deb.y)) return false;
            }
        }
        else if (deb.x > fin.x)
        {
            for (int i = fin.x ; i < deb.x; ++i) {
                if (not this->videvide(i, deb.y)) return false;
            }
        }
    }
    else if (deb.x == fin.x) /// MEME COLONNE ///
    {
        if (deb.y < fin.y)
        {
            for (int i = deb.y + 1; i <= fin.y; ++i) {
                if (not this->videvide(deb.x, i)) return false;
            }
        }
        else if (deb.y > fin.y)
        {
            for (int i = fin.y ; i < deb.y; ++i) {
                if (not this->videvide(deb.x, i)) return false;
            }
        }
    }
    else if (fin.x + fin.y == deb.x + deb.y)  /// BAS GAUCHE / HAUT DROITE ///
    {
        if (fin.y > deb.y)
        {
            for (int i = 1; i <= (fin.y - deb.y); ++i) {
                if (not this->videvide(deb.x - i,deb.y + i)) return false;
            }
        }
        else if (fin.y < deb.y)
        {
            for (int i = 0; i < (deb.y - fin.y); ++i) {
                if (not this->videvide(fin.x - i,fin.y + i)) return false;
            }
        }
    }
    else if (fin.y - deb.y == fin.x - deb.x)
    {
        if (fin.y > deb.y)
        {
            for (int i = 1; i <= (fin.y - deb.y); ++i) {
                if (not this->videvide(deb.x + i, deb.y + i)) return false;
            }
        }
        else if (fin.y < deb.y)
        {
            for (int i = 0; i < (deb.y - fin.y); i++) {
                if (not this->videvide(fin.x + i, fin.y + i)) return false;
            }
        }
    }
    else { return false; }
    return true;
}

void Plateau::afficheDPoss(Position k, int mode)
{
    cancelDposs();
    Position p;
    for(p.x = 0; p.x < TAILLE; p.x ++)
    {
        for(p.y = 0; p.y < TAILLE; p.y ++)
        {
            if(d_poss(k,p))
            {
                if(mode == 1)
                    this->transcription[p.x][p.y] = possibleJ;
                else if(mode == 2)
                    this->transcription[p.x][p.y] = possibleV;
            }
        }
    }
}

Type Plateau::getType(Position p) {
    return this->transcription[p.x][p.y];
}

void Plateau::cancelDposs()
{
    Position p;
    for(p.x = 0; p.x < TAILLE; p.x ++)
    {
        for(p.y = 0; p.y < TAILLE; p.y ++)
        {
            if(this->transcription[p.x][p.y] == possibleJ or this->transcription[p.x][p.y] == possibleV)
            {
                this->transcription[p.x][p.y] = vide;
            }
        }
    }
}

bool Plateau::changeAmaz(Player j, Position p) {
    if(j == Blanc){
        return this->transcription[p.x][p.y] == amazB;
    }
    return this->transcription[p.x][p.y] == amazN;
}

int Plateau::nb_depPos(Player joueur) {

    Position p[4], p2;
    int rg(0), res(0);

    // Localisation des amazones d'une couleur donnée, on met leurs coordonnées dans p[]
    for(int i = 0; i < TAILLE; i++){
        for(int j = 0; j < TAILLE; j++){
            if(this->transcription[i][j] == amazB and joueur == Blanc)
            {
                p[rg].x = i;
                p[rg].y = j;
                rg++;
            }
            else if(this->transcription[i][j] == amazN and joueur == Noir)
            {
                p[rg].x = i;
                p[rg].y = j;
                rg++;
            }
        }
    }

    // Compteur des cases accessibles au total
    for(rg = 0; rg < 4; rg++)
    {
        for(p2.x = 0; p2.x < TAILLE; p2.x++)
        {
            for(p2.y = 0; p2.y < TAILLE; p2.y++)
            {
                if(d_poss(p[rg], p2)) res++;
            }
        }
    }

    return res;
}