//
// Created by paul patault on 03/10/2019.
//
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
using namespace std;
using namespace sf;
#include "Pool.h"

Pool::Pool()
{
    for(auto & i : this->tab){  // <=> for(int i = 0; i < TAILLE; i++){
        for(auto & j : i){      // <=> for(int j = 0; j < TAILLE; j++){
            j = 0;              // <=> t[i][j] = 0
        }
    }

}

void Pool::draw(RenderWindow &window, int TAILLE)
{
    RectangleShape rectangle_fond(sf::Vector2f(width, height));
    {
        rectangle_fond.setFillColor(Color::White);
        window.draw(rectangle_fond);
    }

    for(int i = 0; i < TAILLE; i++){
        RectangleShape line(sf::Vector2f(1, height));
        {
            line.setFillColor(Color::Black);
            line.setPosition((float)i * width/TAILLE, 0);
            window.draw(line);
        }
        RectangleShape line2(sf::Vector2f(height, 1));
        {
            line2.setFillColor(Color::Black);
            line2.setPosition(0, (float)i * width/TAILLE);
            window.draw(line2);
        }
        for(int j = 0; j < TAILLE; j++){
            if(this->tab[i][j] == 1){
                RectangleShape cell(sf::Vector2f(width/TAILLE, height/TAILLE));
                {
                    cell.setFillColor(Color::Black);
                    cell.setPosition((float)i * width/TAILLE, (float)j * width/TAILLE);
                    window.draw(cell);
                }
            }
        }
    }
    float y_rec_font_et_pause = (float)height/((float)TAILLE/2);
    RectangleShape rectangle_font_et_pause(sf::Vector2f(width, y_rec_font_et_pause));
    {
        rectangle_fond.setFillColor(Color::White);
        window.draw(rectangle_font_et_pause);
    }
}

void Pool::update(int TAILLE) {
    int copy[TAILLE][TAILLE];

    Vector2i parcours;
    for(parcours.x = 0; parcours.x < TAILLE; parcours.x++)
    {
        for(parcours.y = 0; parcours.y < TAILLE; parcours.y++)
        {
            int vois = nbVoisins(parcours, TAILLE);
            if(this->tab[parcours.x][parcours.y] == 1)
            {
                if(vois != 2 and vois != 3){
                    copy[parcours.x][parcours.y] = 0;
                } else {
                    copy[parcours.x][parcours.y] = 1;
                }
            }
            else
            {
                if(vois == 3){
                    copy[parcours.x][parcours.y] = 1;
                } else {
                    copy[parcours.x][parcours.y] = 0;
                }
            }
        }
    }
    for(int i = 0; i < TAILLE; i++){
        for(int j = 0; j < TAILLE; j++)
            this->tab[i][j] = copy[i][j];
    }
}

int Pool::nbVoisins(Vector2i p, int TAILLE) {
    int voisins = 0;

    if(p.x == 0 and p.y == 0)
    {
        if(this->tab[p.x + 1][p.y] == 1) voisins++;
        if(this->tab[p.x][p.y + 1] == 1) voisins++;
        if(this->tab[p.x + 1][p.y + 1] == 1) voisins++;
    }
    else if(p.x == 0)
    {
        if(this->tab[p.x][p.y - 1] == 1) voisins++;
        if(this->tab[p.x][p.y + 1] == 1) voisins++;
        if(this->tab[p.x + 1][p.y - 1] == 1) voisins++;
        if(this->tab[p.x + 1][p.y] == 1) voisins++;
        if(this->tab[p.x + 1][p.y + 1] == 1) voisins++;
    }
    else if(p.y == 0)
    {
        if(this->tab[p.x - 1][p.y] == 1) voisins++;
        if(this->tab[p.x + 1][p.y] == 1) voisins++;
        if(this->tab[p.x - 1][p.y + 1] == 1) voisins++;
        if(this->tab[p.x][p.y + 1] == 1) voisins++;
        if(this->tab[p.x + 1][p.y + 1] == 1) voisins++;
    }
    else if(p.x == TAILLE-1 and p.y == TAILLE-1)
    {
        if(this->tab[p.x - 1][p.y] == 1) voisins++;
        if(this->tab[p.x][p.y - 1] == 1) voisins++;
        if(this->tab[p.x + 1][p.y - 1] == 1) voisins++;
    }
    else if(p.x == TAILLE-1)
    {
        if(this->tab[p.x][p.y - 1] == 1) voisins++;
        if(this->tab[p.x][p.y + 1] == 1) voisins++;
        if(this->tab[p.x - 1][p.y - 1] == 1) voisins++;
        if(this->tab[p.x - 1][p.y] == 1) voisins++;
        if(this->tab[p.x - 1][p.y + 1] == 1) voisins++;
    }
    else if(p.y == TAILLE-1)
    {
        if(this->tab[p.x - 1][p.y] == 1) voisins++;
        if(this->tab[p.x + 1][p.y] == 1) voisins++;
        if(this->tab[p.x - 1][p.y - 1] == 1) voisins++;
        if(this->tab[p.x][p.y - 1] == 1) voisins++;
        if(this->tab[p.x + 1][p.y - 1] == 1) voisins++;
    } else {
        if(this->tab[p.x - 1][p.y - 1] == 1) voisins++;
        if(this->tab[p.x - 1][p.y] == 1) voisins++;
        if(this->tab[p.x - 1][p.y + 1] == 1) voisins++;
        if(this->tab[p.x][p.y - 1] == 1) voisins++;
        if(this->tab[p.x][p.y + 1] == 1) voisins++;
        if(this->tab[p.x + 1][p.y - 1] == 1) voisins++;
        if(this->tab[p.x + 1][p.y] == 1) voisins++;
        if(this->tab[p.x + 1][p.y + 1] == 1) voisins++;
    }

    return voisins;
}

void Pool::clic(Vector2i p) {
    if(this->tab[p.x][p.y] == 1){
        this->tab[p.x][p.y] = 0;
    } else {
        this->tab[p.x][p.y] = 1;
    }
}


