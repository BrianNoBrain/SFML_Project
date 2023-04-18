#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/timeb.h>

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

float circle_r = (float)SCREEN_HEIGHT/30;
float circle_x[2] = {0};
float circle_v[2];
float circle_v_magnitude;
float circle_v_angle;
float circle_maxv = 5;
float circle_minv = 10;
int color[3]={0,255,0};
float rectangle_width = (float)SCREEN_WIDTH/50;
float rectangle_height = (float)SCREEN_HEIGHT/5;
float rectangle_speed = (float)SCREEN_HEIGHT*1.5;
float rectangle_wall_distance = (float)SCREEN_WIDTH/15;
int status = 0;

sf::Clock Clock;
float deltatime = Clock.getElapsedTime().asSeconds();

int main()
{

    struct timeb timeSeed;
	ftime(&timeSeed);
	srand(timeSeed.time * 1000 + timeSeed.millitm);
    circle_v_angle = rand()%360;
    circle_v_magnitude = circle_minv+rand()%(int)(circle_maxv-circle_minv);
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pong Game");

    sf::CircleShape circle(circle_r);
    circle.setOrigin(circle_r, circle_r);
    circle.setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    circle.rotate(45.f);

    sf::RectangleShape rectangle1(sf::Vector2f(rectangle_width, rectangle_height));
    rectangle1.setOrigin(rectangle_width/2, rectangle_height/2);
    rectangle1.setPosition(rectangle_wall_distance, SCREEN_HEIGHT/2);
    sf::RectangleShape rectangle2(sf::Vector2f(rectangle_width, rectangle_height));
    rectangle2.setOrigin(rectangle_width/2, rectangle_height/2);
    rectangle2.setPosition(SCREEN_WIDTH-rectangle_wall_distance, SCREEN_HEIGHT/2);

    Sleep(1000);

    while (window.isOpen())
    {
        // if window close
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // set ball color
        circle.setFillColor(sf::Color(color[0],color[1],color[2]));
        /*color[0]++; //color[1]++; //color[2]++;
        if(color[0]>255){
            color[0]=0; color[1]=0; color[2]=0;
        }*/

        // input detection and player rectangle movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            rectangle1.move(0.0f, rectangle_speed*(-1)*deltatime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            rectangle1.move(0.0f, rectangle_speed*deltatime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            rectangle2.move(0.0f, rectangle_speed*(-1)*deltatime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            rectangle2.move(0.0f, rectangle_speed*deltatime);
        }

        // ball movement and rebound angle
        circle_v[0] = circle_v_magnitude*cos(circle_v_angle*PI/180); circle_v[1] = circle_v_magnitude*sin(circle_v_angle*PI/180);
        circle.move(circle_v[0], circle_v[1]);
        if((circle.getPosition().x-rectangle1.getPosition().x)<(circle_r+rectangle_width/2) && fabs(circle.getPosition().y-rectangle1.getPosition().y)<=(double)rectangle_height/2){
            if(circle_v_angle>180) circle_v_angle=540-circle_v_angle;
            else circle_v_angle=180-circle_v_angle;
            circle.setPosition(rectangle1.getPosition().x+circle_r+rectangle_width/2, circle.getPosition().y);
        }
        if((rectangle2.getPosition().x-circle.getPosition().x)<(circle_r+rectangle_width/2) && fabs(circle.getPosition().y-rectangle2.getPosition().y)<=(double)rectangle_height/2){
            if(circle_v_angle>180) circle_v_angle=540-circle_v_angle;
            else circle_v_angle=180-circle_v_angle;
            circle.setPosition(rectangle2.getPosition().x-circle_r-rectangle_width/2, circle.getPosition().y);
        }
        if(circle.getPosition().y<circle_r){
            circle_v_angle=360-circle_v_angle;
            circle.setPosition(circle.getPosition().x, circle_r);
        }
        if(circle.getPosition().y>(SCREEN_HEIGHT-circle_r)){
            circle_v_angle=360-circle_v_angle;
            circle.setPosition(circle.getPosition().x, SCREEN_HEIGHT-circle_r);
        }
        if(circle_v_angle>360) circle_v_angle-=360;
        if(circle_v_angle<0) circle_v_angle+=360;
        circle_v_magnitude+=0.01;

        // player rectangle out of bound detection
        if(rectangle1.getPosition().y<rectangle_height/2) rectangle1.setPosition(rectangle1.getPosition().x, rectangle_height/2);
        if(rectangle1.getPosition().y>SCREEN_HEIGHT-rectangle_height/2) rectangle1.setPosition(rectangle1.getPosition().x, SCREEN_HEIGHT-rectangle_height/2);
        if(rectangle2.getPosition().y<rectangle_height/2) rectangle2.setPosition(rectangle2.getPosition().x, rectangle_height/2);
        if(rectangle2.getPosition().y>SCREEN_HEIGHT-rectangle_height/2) rectangle2.setPosition(rectangle2.getPosition().x, SCREEN_HEIGHT-rectangle_height/2);
        
        // winner detection
        if(circle.getPosition().x<circle_r) status=1;
        if(circle.getPosition().x>SCREEN_WIDTH-circle_r) status=2;

        // frame time
        deltatime = Clock.getElapsedTime().asSeconds();
        Clock.restart();

        // draw objects
        window.clear();
        if(status==0) window.draw(circle);
        window.draw(rectangle1);
        window.draw(rectangle2);
        window.display();

        // UWU have a nice sleep
        Sleep(10);
    }

    return 0;
}































// It took me 3 hours to figure out how to setup sfml, finally it WORK!!!!!!!!!!!!!!!!!!!
