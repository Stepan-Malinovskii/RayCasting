#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>
#include <thread>
#define M_PI 3.14159265358979323846

class Map
{
private:
public:
    std::vector<sf::Shape*> Figure;
    Map()
    {
        LoadConture();
        LoadFigure();
    }

    void LoadConture()
    {
        auto* line1 = new sf::RectangleShape(sf::Vector2f(500, 10));
        line1->setFillColor(sf::Color::Red);
        line1->setPosition(sf::Vector2f(30, 30));

        auto* line2 = new sf::RectangleShape(*line1);
        line2->setRotation(90);

        auto* line3 = new sf::RectangleShape(*line2);
        line3->setPosition(sf::Vector2f(530, 40));
        auto* line4 = new sf::RectangleShape(*line1);
        line4->setPosition(sf::Vector2f(20, 530));

        Figure.push_back(line1);
        Figure.push_back(line2);
        Figure.push_back(line3);
        Figure.push_back(line4);
    }

    void LoadFigure()
    {
        auto* f1 = new sf::RectangleShape(sf::Vector2f(100, 100));
        f1->setPosition(sf::Vector2f(100, 100));
        f1->setFillColor(sf::Color::Magenta);
        Figure.push_back(f1);
    }
};

class Player
{
private:
    float rotationSpeed = 4, moveSpeed = 150;
    Map* nowMap;

    void SetBody()
    {
        Body->setOrigin(Body->getRadius(), Body->getRadius());
        Body->setPosition(Position);
        Body->setFillColor(sf::Color::Red);
    }
public:
    float Angle, Deth = 300;
    sf::Vector2f Position;
    sf::CircleShape* Body = new sf::CircleShape(10, 10);
    const float Fov = M_PI / 3;

    Player(float angle = 0, float posX = 0 , float posY = 0, Map* newMap = nullptr) : 
        Angle{ angle }, Position{ sf::Vector2f(posX,posY)}, nowMap{ newMap } 
    {
        SetBody();
    }

    void ResetMap(Map* newMap)
    {
        nowMap = newMap;
    }

    void MakeStep(float direction)
    {
        Position.x += cos(Angle) * moveSpeed * direction;
        Position.y += sin(Angle) * moveSpeed * direction;
        
        for (auto i : nowMap->Figure)
        {
            if (i->getGlobalBounds().contains(Position))
            {
                Position.x -= cos(Angle) * moveSpeed * direction;
                Position.y -= sin(Angle) * moveSpeed * direction;
                
                break;
            }
        }

        Body->setPosition(Position);
    }

    void MakeRotation(float directions)
    {
        Angle += directions * rotationSpeed;
    }
};

class RayCasting
{
private:
    Player* player;
    Map* nowPlace;
    std::vector <float>* masRays;
    int RayCount = 320;

    float rayX, rayY, dist;
    bool hitWall;
    sf::Vector2f test;

    float Ray(float rayAngle)
    {
        rayX = std::cos(rayAngle);
        rayY = std::sin(rayAngle);

        dist = 0;
        hitWall = false;

        while (dist < player->Deth && !hitWall)
        {
            test = sf::Vector2f((player->Position.x + rayX * dist), (player->Position.y + rayY * dist));

            if (test.x >= (player->Deth + player->Position.x) || test.y >= (player->Deth + player->Position.y))
            {
                hitWall = true;
                dist = player->Deth;
            }
            else
            {
                for (auto fg : nowPlace->Figure)
                {
                    if (fg->getGlobalBounds().contains(test))
                    {
                        hitWall = true;
                        break;
                    }
                }
            }

            dist += 1;
        }
        return  dist ;
    }
public:
    RayCasting(Player* pl, Map* nmp, std::vector <float> * rays) : player{ pl }, nowPlace{ nmp }, masRays{ rays } {}

    void CastRay()
    {
        masRays->clear();
        float angle;
        for (int cnt = 0; cnt < RayCount; cnt++)
        {
            angle = player->Angle - player->Fov / 2 + player->Fov * cnt / float(1960 / 2.0);
            masRays->push_back(Ray(angle) * cos(angle - player->Angle));
        }
    }
    
    ~RayCasting() {}
};

class GameManager
{
private:
    sf::RenderWindow* window;
    sf::Event event;

    std::chrono::steady_clock::time_point timerB, timerA;
    float deltaTime = 0;

    sf::Vertex line[2];
    std::vector <float>* rays;

    int nowLocation = 0;
    std::vector<Map*>* maps;

    Map* map1;
    Map* nowMap;
    Player* player;
    RayCasting* rayCasting;

    sf::Font font;
    sf::Text text;
public:
    GameManager(sf::RenderWindow* _window) : window{ _window }
    {
        map1 = new Map();
        maps = new std::vector<Map*>();
        maps->push_back(map1);
        nowMap = (*maps)[nowLocation];

        player = new Player(0, 300, 300, (*maps)[nowLocation]);
        rays = new std::vector <float>();
        rayCasting = new RayCasting(player, ((*maps)[nowLocation]), rays);

        font.loadFromFile("CyrilicOld.TTF");
        text = sf::Text("", font, 20);
        text.setFillColor(sf::Color::Green);
        text.setPosition(100, 100);        
    }

    void Processer()
    {
        timerB = std::chrono::steady_clock::now();
        
        rayCasting->CastRay();
        Draw();
        CheckInput();

        timerA = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration<float>(timerA - timerB).count();
    }

    void Draw()
    {
        window->clear();
        
        window->draw(*player->Body);
        float rayAngle = player->Angle;
        float rayX = std::cos(rayAngle) * player->Deth;
        float rayY = std::sin(rayAngle) * player->Deth;
        sf::Vertex line[] =
        {
            player->Position,
            sf::Vector2f(player->Position.x + rayX, player->Position.y + rayY)
        };
        window->draw(line, 2, sf::Lines);

        float blockH = window->getSize().y;
        float sizeX = window->getSize().x / 320;
        sf::RectangleShape sh(sf::Vector2f(sizeX, 100));
        float cel;
        int xpos = 0;
        
        for (auto i : *rays)
        {
            cel =  blockH * (1- i/player->Deth);
            sh.setSize(sf::Vector2f(sizeX, cel));
            sh.setPosition(sf::Vector2f(xpos,window->getSize().y/2 - sh.getSize().y/2));
            xpos+= sizeX;
            
            window->draw(sh);
        }

        text.setString(std::to_string(1.0 / deltaTime));
        window->draw(text);
        window->display();
    }

    void CheckInput()
    {
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
        }

        if (GetAsyncKeyState('A'))
        {
            player->MakeRotation(-1 * deltaTime);
        }
        else if (GetAsyncKeyState('D'))
        {
            player->MakeRotation(1 * deltaTime);
        }
        if (GetAsyncKeyState('W'))
        {
            player->MakeStep(1 * deltaTime);
        }
        else if (GetAsyncKeyState('S'))
        {
            player->MakeStep(-1 * deltaTime);
        }
    }
};

int main()
{
    sf::ContextSettings setings;
    setings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(320, 200), "My window", sf::Style::Fullscreen, setings);
    
    GameManager* gameManager = new GameManager(&window);

    while (window.isOpen())
    {
        gameManager->Processer();
    }

    return 0;
}