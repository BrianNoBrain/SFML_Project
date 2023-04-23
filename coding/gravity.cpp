#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cmath>
#include <sys/timeb.h>
#include <windows.h>

using namespace std;

const float G = 6.67430E-11;
const float SUN_M = 1.98847E30;
const float EARTH_M = 5.972E24;
const float MOON_M = 7.34767309E22;
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

double timeSpeed = 7200.0;
double scale = 1000*1000;
double restitution = 0.5;

sf::Clock Clock;
double deltatime = Clock.getElapsedTime().asSeconds();

void CaculateAllGravity();
void AddAllVelocity(double dt);
void AddAllPosition(double dt);
void DetectWallBound();
struct planet;

vector<planet *> planet_manager;

struct planet: public sf::CircleShape{
    double mass;
    sf::Vector2<double> velocity;
    sf::Vector2<double> acceleration;

    planet(){
        mass = 1E25;

        velocity.x = 0.0;
        velocity.y = 0.0;

        acceleration.x = 0.0;
        acceleration.y = 0.0;

        setRadius(50.f);
        setFillColor(sf::Color::Green);
    }

    planet(float r, float m, sf::Vector2f pos, sf::Color color){
        setRadius(r);
        mass = m;
        setPosition(pos.x, pos.y);
        setFillColor(color);
        setOrigin(r, r);
        planet_manager.push_back(this);
        cout<<planet_manager.size()<<"\n";
    }
};

int main(){
    freopen("log.txt", "w", stdout);
    struct timeb timeSeed;
	ftime(&timeSeed);
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Gravity Test");
    planet planet1((float)10.0, 6E20, sf::Vector2f (SCREEN_WIDTH/2-350,SCREEN_HEIGHT/2), sf::Color (255,0,0));
    planet planet2((float)10.0, 5E20, sf::Vector2f (SCREEN_WIDTH/2-100,SCREEN_HEIGHT/2-200), sf::Color (0,255,0));
    planet planet3((float)10.0, 8E20, sf::Vector2f (SCREEN_WIDTH/2-50,SCREEN_HEIGHT/2+200), sf::Color (0,0,255));
    planet planet4((float)10.0, 10E20, sf::Vector2f (SCREEN_WIDTH/2-250,SCREEN_HEIGHT/2-100), sf::Color (0,100,100));
    planet planet5((float)20.0, 50E20, sf::Vector2f (SCREEN_WIDTH/2,SCREEN_HEIGHT/2), sf::Color (255,255,0));
    planet planet6((float)10.0, 2E20, sf::Vector2f (SCREEN_WIDTH/2+100,SCREEN_HEIGHT/2+300), sf::Color (0,255,255));
    planet planet7((float)10.0, 15E20, sf::Vector2f (SCREEN_WIDTH/2+250,SCREEN_HEIGHT/2+100), sf::Color (255,0,255));
    planet planet8((float)20.0, 20E20, sf::Vector2f (SCREEN_WIDTH/2+200,SCREEN_HEIGHT/2+200), sf::Color (255,255,255));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //cout<<"==================\n";
        CaculateAllGravity();
        AddAllVelocity(deltatime);
        AddAllPosition(deltatime);
        DetectWallBound();
        //cout<<"==================\n";

        deltatime = Clock.getElapsedTime().asSeconds();
        Clock.restart();

        window.clear();
        
        window.draw(planet1);
        window.draw(planet2);
        window.draw(planet3);
        window.draw(planet4);
        window.draw(planet5);
        window.draw(planet6);
        window.draw(planet7);
        window.draw(planet8);
        
        window.display();

        //Sleep(1000);
    }
    return 0;
}


void CaculateAllGravity(){
    //cout<<"----------------\n";
    int siz = planet_manager.size();
    for(int i=0;i<siz;i++){
        planet_manager[i]->acceleration.x = 0.0;
        planet_manager[i]->acceleration.y = 0.0;
        for(int j=0;j<siz;j++){
            sf::Vector2<double> distance;
            double distance_magnitude;
            double a;
            distance = sf::Vector2<double>(planet_manager[j]->getPosition()-planet_manager[i]->getPosition());
            distance = distance*scale;
            distance_magnitude = sqrt(distance.x*distance.x+distance.y*distance.y);
            if(distance_magnitude>=10.0*scale){
                a = G * planet_manager[j]->mass / (distance_magnitude*distance_magnitude);
            }else{
                a = G * planet_manager[j]->mass / (10.0*scale*10.0*scale);
            }
            /*
            if(distance_magnitude>=10.0*scale){
                if(planet_manager[i]->mass>0){
                    a = G * planet_manager[j]->mass / (distance_magnitude*distance_magnitude);
                }else{
                    a = -1 * G * planet_manager[j]->mass / (distance_magnitude*distance_magnitude);
                }
            }else{
                if(planet_manager[i]->mass>0){
                    a = G * planet_manager[j]->mass / (10.0*scale*10.0*scale);
                }else{
                    a = -1 * G * planet_manager[j]->mass / (10.0*scale*10.0*scale);
                }
            }
            */
            if(distance_magnitude!=0){
                planet_manager[i]->acceleration.x += a/distance_magnitude*distance.x;
                planet_manager[i]->acceleration.y += a/distance_magnitude*distance.y;
            }
            //cout<<"distance: "<<distance.x<<" "<<distance.y<<"\n";
            //cout<<"distance magnitude: "<<distance_magnitude<<"\n";
            //cout<<"a: "<<a<<"\n";
        }
        //cout<<"acceleration: "<<planet_manager[i]->acceleration.x<<" "<<planet_manager[i]->acceleration.y<<"\n";
    }
    //cout<<"----------------\n";
    return;
};

void AddAllVelocity(double dt){
    //cout<<"----------------\n";
    int siz = planet_manager.size();
    for(int i=0;i<siz;i++){
        planet_manager[i]->velocity.x += planet_manager[i]->acceleration.x * dt * timeSpeed;
        planet_manager[i]->velocity.y += planet_manager[i]->acceleration.y * dt * timeSpeed;
        //cout<<"velocity: "<<planet_manager[i]->velocity.x<<" "<<planet_manager[i]->velocity.y<<"\n";
    }
    //cout<<"----------------\n";
    return;
};

void AddAllPosition(double dt){
    int siz = planet_manager.size();
    for(int i=0;i<siz;i++){
        planet_manager[i]->setPosition(planet_manager[i]->getPosition().x+planet_manager[i]->velocity.x*dt*timeSpeed, planet_manager[i]->getPosition().y+planet_manager[i]->velocity.y*dt*timeSpeed);
    }
    return;
};

void DetectWallBound(){
    int siz = planet_manager.size();
    for(int i=0;i<siz;i++){
        double r = planet_manager[i]->getRadius();
        if(planet_manager[i]->getPosition().x>SCREEN_WIDTH-r){
            planet_manager[i]->setPosition(SCREEN_WIDTH-r, planet_manager[i]->getPosition().y);
            planet_manager[i]->velocity.x *= -1*restitution;
        }
        if(planet_manager[i]->getPosition().x<r){
            planet_manager[i]->setPosition(r, planet_manager[i]->getPosition().y);
            planet_manager[i]->velocity.x *= -1*restitution;
        }
        if(planet_manager[i]->getPosition().y>SCREEN_HEIGHT-r){
            planet_manager[i]->setPosition(planet_manager[i]->getPosition().x, SCREEN_HEIGHT-r);
            planet_manager[i]->velocity.y *= -1*restitution;
        }
        if(planet_manager[i]->getPosition().y<r){
            planet_manager[i]->setPosition(planet_manager[i]->getPosition().x, r);
            planet_manager[i]->velocity.y *= -1*restitution;
        }
    }
    return;
}