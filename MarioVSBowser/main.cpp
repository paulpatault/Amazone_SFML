#include <iostream>
#include <time.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

const int MOVE = 2;

bool onPlateforme(Vector2f mario, Vector2f plateforme, float dyMario)
{
    return ((mario.y + 96 >= plateforme.y) && (mario.y + 96 <= plateforme.y + 57)
     && (mario.x >= plateforme.x) && (mario.x + 10 <= plateforme.x + 254)
     && ((dyMario > 0) or dyMario == -10));
}

int main() {
    srand(time(nullptr));

    sf::RenderWindow window(sf::VideoMode(800, 800), "PLATEFORME");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    sf::Texture t_mario_R, t_mario_L, t_bowser_R, t_bowser_L, t_plateforme, t_sol, t_fireball, t_arrow, t_dead, t_win, t_lose;
    {
        t_mario_R.loadFromFile("../data/mario_Right.png");
        t_mario_L.loadFromFile("../data/mario_Left.png");
        t_bowser_R.loadFromFile("../data/bowser_Right.png");
        t_bowser_L.loadFromFile("../data/bowser_Left.png");
        t_plateforme.loadFromFile("../data/plateforme.png");
        t_plateforme.setSmooth(true);
        t_sol.loadFromFile("../data/sol.png");
        t_fireball.loadFromFile("../data/fireball.png");
        t_arrow.loadFromFile("../data/arrow.png");
        t_win.loadFromFile("../data/youWin.png");
        t_lose.loadFromFile("../data/gameOver.png");
        t_dead.loadFromFile("../data/life/dead.png");
        t_dead.setSmooth(true);

    }
    sf::Texture t_life[14];  // 0:6 -> à l'endroit // 7:13 -> à l'envers
    {
        t_life[0].loadFromFile("../data/life/life0.png"); t_life[7].loadFromFile("../data/life/rlife0.png");
        t_life[1].loadFromFile("../data/life/life1.png"); t_life[8].loadFromFile("../data/life/rlife1.png");
        t_life[2].loadFromFile("../data/life/life2.png"); t_life[9].loadFromFile("../data/life/rlife2.png");
        t_life[3].loadFromFile("../data/life/life3.png"); t_life[10].loadFromFile("../data/life/rlife3.png");
        t_life[4].loadFromFile("../data/life/life4.png"); t_life[11].loadFromFile("../data/life/rlife4.png");
        t_life[5].loadFromFile("../data/life/life5.png"); t_life[12].loadFromFile("../data/life/rlife5.png");
        t_life[6].loadFromFile("../data/life/life6.png"); t_life[13].loadFromFile("../data/life/rlife6.png");
    }

    sf::Sprite mario;
    {
        mario.setTexture(t_mario_R);
        mario.scale(0.08,0.08);
        mario.setPosition(110,615);
    }
    sf::Sprite bowser;
    {
        bowser.setTexture(t_bowser_L);
        bowser.scale(0.4,0.4);
        bowser.setPosition(550,390);
    }
    sf::Sprite fireBall;
    {
        fireBall.setTexture(t_fireball);
        fireBall.setPosition(545, 400);
    }
    sf::Sprite plateforme;
    {
        plateforme.setTexture(t_plateforme);
        plateforme.scale(1.1,1.1);
        plateforme.setPosition(100,200);
    }
    sf::Sprite plateforme_2;
    {
        plateforme_2.setTexture(t_plateforme);
        plateforme_2.scale(1.1,1.1);
        plateforme_2.setPosition(500,500);
    }
    sf::Sprite sol;
    {
        sol.setTexture(t_sol);
    }
    sf::Sprite arrow;
    {
        arrow.setTexture(t_arrow);
        arrow.scale(0.3,0.3);
        arrow.setPosition(0,-100);
    }
    sf::Sprite life[2];
    {
        life[0].setTexture(t_life[0]);
        life[0].setPosition(50,10);
        life[0].scale(0.8,0.8);
        life[1].setTexture(t_life[7]);
        life[1].setPosition(500,10);
        life[1].scale(0.8,0.8);
    }
    sf::Sprite end;
    {
        //end.setTexture(t_win);
        end.setPosition(400,300);
        end.setOrigin(529/2, 346/2);
    }
    // variables that manage time
    sf::Clock clock;
    float timerPlateformes = 0;
    float timerAutoDep = 0;
    float timerRandomJump = 0;
    float timerFireBall = 0;
    float delay = 0.01, delayFireBall = 2.5, delayRandomJump = 1.68;

    Vector2f upDownPlat;
    {
        upDownPlat.x = 0;
        upDownPlat.y = MOVE;
    }

    float dyMario = 0, dyBowser = 0;
    float dyGravity = 0;
    bool marioJump = false, hit = false, bowserJump = false;


    int HP_MARIO = 7, HP_BOWSER = 7;

    while (window.isOpen()){

        ////// time for tick //////
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timerPlateformes += time;
        timerAutoDep += time;
        timerFireBall += time;
        timerRandomJump += time;

        Event event;
        ////// handle all events //////
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        ////// Tick PLATEFORME //////
        if(timerPlateformes > delay)
        {
            if(plateforme.getPosition().y > 580)
                upDownPlat.y = -MOVE;
            else if (plateforme.getPosition().y < 150)
                upDownPlat.y = MOVE;
            plateforme.move(upDownPlat);
            timerPlateformes = 0;
        }

        ////// Deplacement Psg auto //////

        if(timerRandomJump > delayRandomJump)  // Bowser jump
        {
            int uneChanceSur2 = rand() % 2;
            if(uneChanceSur2 == 1 and not bowserJump)
                bowserJump = true;
            timerRandomJump = 0;
        }
        if(bowserJump)
        {
            dyBowser += 0.2;
            bowser.move(0,dyBowser);
            if(bowser.getPosition().y >= 390){
                dyBowser = -10;
                bowserJump = false;
            }
        }

        // mario 'chute'
        if(timerAutoDep > delay)
        {
            if(onPlateforme(mario.getPosition(), plateforme.getPosition(), dyMario))  /// SI SUR LA PLATEFORME
            {
                dyMario = -10;
                marioJump = false;
                mario.setPosition(mario.getPosition().x, (float)(int)mario.getPosition().y);
                mario.move(upDownPlat);
            }
            else
            {
                if ((mario.getPosition().y < 605 and not marioJump))
                    // à décommenter pour la chute dans la lave
                    //or ((mario.getPosition().x <= 100 or mario.getPosition().x >= 700)
                    //      and  mario.getPosition().y < 680)
                    //)  /// SI CHUTE
                {
                    dyGravity += 0.16;
                    mario.move(0,dyGravity);
                }                                           // IDEM
                else if (mario.getPosition().y >= 605)// and mario.getPosition().x > 100 and mario.getPosition().x < 700)   /// SI AU SOL
                {
                    dyGravity = 0;
                }
            }
            timerAutoDep = 0;
        }

        ////// Mouvements Psg guidé //////
        if(Keyboard::isKeyPressed(Keyboard::Q) or Keyboard::isKeyPressed(Keyboard::Left)){
            /// SI MOVE LEFT POSSIBLE   // mario::78x96
            if(plateforme.getPosition().y > mario.getPosition().y and plateforme.getPosition().y + 57 < mario.getPosition().y
                and plateforme.getPosition().x + 231 < mario.getPosition().x
                    and plateforme.getPosition().x + 231 > mario.getPosition().x + 10)
            {
                cout << "ok" << endl;
            }
            else
            {
                mario.move(-3,0);
                mario.setTexture(t_mario_L);
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::D) or Keyboard::isKeyPressed(Keyboard::Right)){
            mario.move(3,0);
            mario.setTexture(t_mario_R);
        }
        if(Keyboard::isKeyPressed(Keyboard::Z) or Keyboard::isKeyPressed(Keyboard::Up)){
            marioJump = true;
        }
        if(Keyboard::isKeyPressed(Keyboard::Space)){
            hit = true;
        }

        if(marioJump){  // SAUT
            dyMario += 0.2;
            mario.move(0,dyMario);
            if(mario.getPosition().y >= 605){ // EN BAS
                dyMario = -10;
                marioJump = false;
            }
        }

        ////// Tick FIRE BALL //////
        if(timerFireBall > delayFireBall)
        {
            fireBall.setPosition(545, bowser.getPosition().y + 10);
            timerFireBall = 0;
        }
        fireBall.move(-6,0);

        ////// Hit //////
        if(hit)
        {
            arrow.setPosition(mario.getPosition().x,mario.getPosition().y);
            hit = false;
        }
        arrow.move(5,0);

        /// bowser is touch ///
        if(arrow.getPosition().x > bowser.getPosition().x && arrow.getPosition().x < bowser.getPosition().x + 126
           && arrow.getPosition().y > bowser.getPosition().y && arrow.getPosition().y < bowser.getPosition().y + 126)
        {
            HP_BOWSER -= 1;
            arrow.move(1000,0);
        }

        /// mario is touch ///
        if(fireBall.getPosition().x > mario.getPosition().x && fireBall.getPosition().x < mario.getPosition().x + 78
           && fireBall.getPosition().y > mario.getPosition().y && fireBall.getPosition().y < mario.getPosition().y + 96)
        {
            HP_MARIO -= 1;
            fireBall.move(-1000,0);
        }


        ////// Life //////
        if(HP_MARIO > 0){
            life[0].setTexture(t_life[7-HP_MARIO]);
        } else {
            life[0].setTexture(t_dead);
            end.setTexture(t_lose);
        }
        if(HP_BOWSER > 0) {
            life[1].setTexture(t_life[14 - HP_BOWSER]);
        } else {
            life[1].setTexture(t_dead);
            end.setTexture(t_win);
        }

        ////// Draw //////
        //window.clear(sf::Color(185,230,250));

        window.clear();
        window.draw(sol);
        window.draw(plateforme);
        window.draw(plateforme_2);
        window.draw(life[0]);
        window.draw(life[1]);
        window.draw(mario);
        window.draw(bowser);
        window.draw(arrow);
        window.draw(fireBall);
        window.draw(end);
        window.display();
    }

    return 0;
}