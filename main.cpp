#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Plateau.h"
using namespace std;
using namespace sf;

Position getPosMouse(Event event, RenderWindow& w){
    Position p;
    Vector2u fenetreSize = w.getSize();
    p.x = event.mouseButton.x / (float)(fenetreSize.x/10);
    p.y = event.mouseButton.y / (float)(fenetreSize.y/10);
    return p;
}
Player actuTour(Player p){
    if(p == Noir){
        return Blanc;
    }
    return Noir;
}

int main(){

    // création de la fenetre
    RenderWindow window(VideoMode(width, height), "AMAZONE");
    window.setVerticalSyncEnabled(true);

    // création du plateau & damier
    Plateau P = Plateau();
    P.initialise();
    P.actualise(window);

    window.display();

    // déclaration des variables de jeu
    Player tour = Blanc;
    Position mouse, save;
    int state(0);  // état du tour := selection / deplacement / fleche

    // boucle infinie
    while (window.isOpen()){

        Event event;
        // boucle d'évènement qui tourne en continue
        while(window.pollEvent(event))
        {
            if (event.type == Event::Closed) window.close();  // pour quitter ( + q ou croix rouge)

            if(event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    mouse = getPosMouse(event, window);  // récupère dans mouse les coordonnées dans le plateau du clic
                }
            }
        }

        switch (state)
        {
            case 0: // choix amazone
            {
                if((tour == Blanc and P.getType(mouse) == amazB) or (tour == Noir and P.getType(mouse) == amazN))
                {
                    state++;
                    save = mouse;
                    P.afficheDPoss(save,1);
                    P.actualise(window);
                    window.display();
                }
                break;
            }
            case 1: // deplacement de l'amazone
            {
                if(P.changeAmaz(tour, mouse)){  // choix d'une autre amazone
                    save = mouse;
                    P.afficheDPoss(save,1);
                    P.actualise(window);
                    window.display();
                }
                else if(P.d_poss(save, mouse)){ // déplacement
                    P.majTab(save, mouse);
                    state++;
                    save = mouse;
                    P.afficheDPoss(save,2);
                    P.actualise(window);
                    window.display();
                }
                break;
            }
            case 2: // fleche
            {
                if(P.d_poss(save, mouse)){
                    P.fleche(mouse);
                    P.cancelDposs();
                    P.actualise(window);
                    window.display();
                    state++;
                }
                break;
            }
            case 3: // change joueur
            {
                if(P.nb_depPos(tour) == 0) exit(10); // dernier joueur
                else if(P.nb_depPos(actuTour(tour)) == 0) exit(20); // prochain joueur
                P.actualise(window);
                window.display();
                tour = actuTour(tour);
                state = 0;
                break;
            }
            default:
                break;
        }
    }
    return 0;
}
