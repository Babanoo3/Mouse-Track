#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <filesystem>

using namespace std;
using namespace sf;

Color getDotColor(float sensitivity){
    sensitivity = std::clamp(sensitivity,0.0f,10.0f);
    if (sensitivity <= 3.3f){
        float t = sensitivity / 3.3f;
        int blue = static_cast<int>(255 * (1 - t));
        return Color(255,255, blue);
    } else if (sensitivity <= 6.6f){
        float t = (sensitivity - 3.3f) / 3.3f;
        int green = static_cast<int>(255 -t * (255 - 165));
        return Color(255,green,0);
    } else {
        float t = (sensitivity - 6.6f) / 3.4f;
        int green = static_cast<int>(165 * (1 - t));
        return Color(255,green,0);
    }
}
int main()
{
    //Open window
    RenderWindow window(VideoMode(1200,800), "SFML Window");
    window.setMouseCursorVisible(false);

    bool menuMode = true;
    bool menuActive = false;
    bool inGear = false;
    float sensitivity = 0.0f;
    float gearMin = 0.0f;
    float gearMax = 0.0f;
    float engSpeed = 0.0f;


    //Center dot 
    Vector2f dotPos(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    CircleShape dot(5);
    dot.setFillColor(Color::White);
    dot.setOrigin(5,5);
    dot.setPosition(dotPos);

    //Center mouse
    Vector2i windowCenter(window.getSize().x /2, window.getSize().y /2);
    Mouse::setPosition(windowCenter,window);

    //Create menu overlay
    RectangleShape menuOverlay(Vector2f(window.getSize()));
    menuOverlay.setFillColor(Color(255,255,255,150));
    //Create menu text
    Font font;
    if (!font.loadFromFile("../assets/Jersey10-Regular.ttf")) {
        cerr<<"Error loading font!"<<endl;
        cerr<<filesystem::current_path()<<endl;
        return -1;
    }
    Text menuText;
    menuText.setFont(font);
    menuText.setString("Main Menu");
    menuText.setCharacterSize(125);
    menuText.setFillColor(Color::Black);
    menuText.setPosition(window.getSize().x /2, (window.getSize().y / 2) - 250);
    FloatRect textRect = menuText.getLocalBounds();
    menuText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    Text infoText;
    infoText.setFont(font);
    infoText.setString("Press Tab to start\nRight click decreases sensitivtiy\nLeft click increases sensitivity\nPress ESC to close");
    infoText.setCharacterSize(40);
    infoText.setFillColor(Color::Black);
    infoText.setPosition(window.getSize().x /2, (window.getSize().y / 2) - 125);
    FloatRect infoRect = infoText.getLocalBounds();
    infoText.setOrigin(infoRect.left + infoRect.width / 2.0f, infoRect.top + infoRect.height / 2.0f);

    while (window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
            }
            if (event.type == Event::KeyPressed){
            switch(event.key.code){
                case Keyboard::Num0:
                    inGear = false;
                    cout<<"Neutral"<<endl;
                    engSpeed = 0.0;
                    gearMax = 0.0;
                    break;
                case Keyboard::Num1:
                    inGear = true;
                    cout<<"1st gear"<<endl;
                    engSpeed = 0.0001;
                    gearMax = 2.5;
                    break;
                case Keyboard::Num2:
                    inGear = true; 
                    cout<<"2nd gear"<<endl;
                    engSpeed = 0.0002;
                    gearMax = 5;
                    break;
                case Keyboard::Num3:
                    inGear = true; 
                    cout<<"3rd gear"<<endl;
                    engSpeed = 0.0003;
                    gearMax = 10;
                    break;
                case Keyboard::Num4:
                    inGear = true; 
                    cout<<"4th gear"<<endl;
                    engSpeed = 0.0004;
                    gearMax = 25;
                    break;
                case Keyboard::Num5:
                    inGear = true; 
                    cout<<"5th gear"<<endl;
                    engSpeed = 0.0005;
                    gearMax = 50;
                    break;
                default:
                    cerr<<"No gear has been selected";
            }
        }

        }
        //Close window
        if (Keyboard::isKeyPressed(Keyboard::Escape)){
            window.close();
        }
        window.clear(Color::Black);
        //Open menu
        if (Keyboard::isKeyPressed(Keyboard::Tab)){
            if (!menuActive){
                menuMode = !menuMode;
                if (!menuMode){
                    Mouse::setPosition(windowCenter,window);
                }
                menuActive = true;
            } 
        } else {
            menuActive = false;
        }

        //If menu open display menu, if menu closed run game as normal
        if (menuMode){
        window.setMouseCursorVisible(true);
        window.draw(menuOverlay);
        window.draw(menuText);
        window.draw(infoText);
        } else {
        //If left click speed up, if right click slow down
        if (Mouse::isButtonPressed(Mouse::Left)){
            if (sensitivity >= 0 && sensitivity < gearMax){
                sensitivity +=engSpeed;
            } 
            if (sensitivity >= gearMax){
                sensitivity = gearMax;
            }
        } 
        if (Mouse::isButtonPressed(Mouse::Right)){
            if (sensitivity > 0 && sensitivity <= gearMax){
                sensitivity -=engSpeed;
            }
            if (sensitivity <=0){
                sensitivity = 0;
            }
        } 

        window.setMouseCursorVisible(false);
        Vector2i mousePos = Mouse::getPosition(window);
        Vector2f delta = Vector2f(mousePos - windowCenter);
        cout<<"Mouse sensitivity = "<<sensitivity<<endl;
        dotPos += delta * sensitivity;
        dot.setPosition(dotPos);
        //Ensure dot cannot leave window
        if (dotPos.x < 0) dotPos.x = 0;
        if (dotPos.y < 0) dotPos.y = 0;    
        if (dotPos.x > window.getSize().x) dotPos.x = window.getSize().x;
        if (dotPos.y > window.getSize().y) dotPos.y = window.getSize().y;
        dot.setFillColor(getDotColor(sensitivity));
        Mouse::setPosition(windowCenter, window); 
        window.draw(dot);
        }
        window.display();
    }
    return 0;
}

