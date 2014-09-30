#include "engine.hpp"
#include "input.hpp"
#include "graphics.hpp"
#include "camera.hpp"
#include "water.hpp"

#include <SDL.h>

#include <iostream>
#include <algorithm>

Input::Input(Engine *eng)
    : engine(eng)
{
    sensitivity = engine->getOptions().camera_sensitivity;
}

Input::~Input()
{
}

void Input::init()
{
    SDL_SetRelativeMouseMode(SDL_TRUE);

    resetKeyPresses();

    #ifdef __APPLE__
    int w, h;
    engine->graphics->getWindowSize(w,h);
    auto window = engine->graphics->getRenderWindow();
    SDL_WarpMouseInWindow(window, w/2, h/2);
    #endif
}

void Input::tick(float dt)
{
    SDL_Event event;
    float x,y;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                if(event.key.keysym.mod == KMOD_LSHIFT || event.key.keysym.mod == KMOD_RSHIFT)
                    keyPresses[KMOD_LSHIFT] = true;//engine->graphics->camera->applyBoost(10.0f);

                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        engine->stop();
                    break;


                    case SDLK_w:
                        keyPresses[SDLK_w] = true;
                        //engine->graphics->camera->moveForward();
                    break;

                    case SDLK_s:
                        keyPresses[SDLK_s] = true;
                        //engine->graphics->camera->moveBackward();
                    break;

                    case SDLK_d:
                        keyPresses[SDLK_d] = true;
                        //engine->graphics->camera->moveRight();
                    break;

                    case SDLK_a:
                        keyPresses[SDLK_a] = true;
                        //engine->graphics->camera->moveLeft();
                    break;

                    case SDLK_r:
                        keyPresses[SDLK_r] = true;
                        //engine->graphics->camera->moveUp();
                    break;

                    case SDLK_f:
                        keyPresses[SDLK_f] = true;
                        //engine->graphics->camera->moveDown();
                    break;

                    case SDLK_LEFT:
                        //engine->graphics->water->lightAngle -= 0.1f;
                        keyPresses[SDLK_LEFT] = true;

                    break;

                    case SDLK_RIGHT:
                        //engine->graphics->water->lightAngle += 0.1f;
                        keyPresses[SDLK_RIGHT] = true;

                    break;
                }

            break;

            case SDL_KEYUP:
                if(!(event.key.keysym.mod == KMOD_LSHIFT || event.key.keysym.mod == KMOD_RSHIFT))
                    keyPresses[KMOD_LSHIFT] = false;

                switch(event.key.keysym.sym) {
                    case SDLK_w:
                        keyPresses[SDLK_w] = false;
                        //engine->graphics->camera->moveForward();
                    break;

                    case SDLK_s:
                        keyPresses[SDLK_s] = false;
                        //engine->graphics->camera->moveBackward();
                    break;

                    case SDLK_d:
                        keyPresses[SDLK_d] = false;
                        //engine->graphics->camera->moveRight();
                    break;

                    case SDLK_a:
                        keyPresses[SDLK_a] = false;
                        //engine->graphics->camera->moveLeft();
                    break;

                    case SDLK_r:
                        keyPresses[SDLK_r] = false;
                        //engine->graphics->camera->moveUp();
                    break;

                    case SDLK_f:
                        keyPresses[SDLK_f] = false;
                        //engine->graphics->camera->moveDown();
                    break;

                    case SDLK_LEFT:
                        keyPresses[SDLK_LEFT] = false;
                    break;

                    case SDLK_RIGHT:
                        keyPresses[SDLK_RIGHT] = false;
                    break;
                }
            break;

            case SDL_QUIT:
                engine->stop();
            break;

            case SDL_MOUSEMOTION:
                x = (float)event.motion.xrel * sensitivity;
                y = (float)event.motion.yrel * sensitivity;

                engine->graphics->camera->rotate(x,y);
            break;

            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    engine->graphics->windowResized();
                }
            break;
        }
    }

    handleMovementKeys();
    //resetKeyPresses();
}

void Input::stop()
{

}

void Input::resetKeyPresses()
{
    keyPresses[SDLK_w] = false;
    keyPresses[SDLK_a] = false;
    keyPresses[SDLK_s] = false;
    keyPresses[SDLK_d] = false;
    keyPresses[SDLK_r] = false;
    keyPresses[SDLK_f] = false;

    keyPresses[KMOD_LSHIFT] = false;

    keyPresses[SDLK_LEFT] = false;
    keyPresses[SDLK_RIGHT] = false;
}

void Input::handleMovementKeys()
{
    if(keyPresses[KMOD_LSHIFT])
        engine->graphics->camera->applyBoost(5.0f);

    else
        engine->graphics->camera->applyBoost(1.0f);

    if(keyPresses[SDLK_w])
        engine->graphics->camera->moveForward();

    if(keyPresses[SDLK_s])
        engine->graphics->camera->moveBackward();

    if(keyPresses[SDLK_d])
        engine->graphics->camera->moveRight();

    if(keyPresses[SDLK_a])
        engine->graphics->camera->moveLeft();

    if(keyPresses[SDLK_r])
        engine->graphics->camera->moveUp();

    if(keyPresses[SDLK_f])
        engine->graphics->camera->moveDown();

    if(keyPresses[SDLK_LEFT])
        engine->graphics->water->lightAngle -= 0.1f;

    if(keyPresses[SDLK_RIGHT])
        engine->graphics->water->lightAngle += 0.1f;
}