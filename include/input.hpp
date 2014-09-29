#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>
#include <string>
#include <map>

//class SDL_Keycode;

class Engine;

class Input {
public:
    Input(Engine *eng);
    ~Input();

    void init();
    void tick(float dt);
    void stop();

private:
    void resetKeyPresses();
    void handleMovementKeys();
    Engine *engine;
    float sensitivity;

    std::map<int, bool> keyPresses;
};

#endif // INPUT_HPP