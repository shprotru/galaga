#ifndef MODEL_H
#define MODEL_H

#include <SDL2/SDL.h>
#include "../assets.h"

namespace MODEL {
    enum Mstate : uint8_t {
        initialized = 0,
        alive,
        dying
    };

    enum Mdirection : uint8_t {
        up = 0,
        right,
        down,
        left
    };

    class IModel
    {
    public:
        virtual void move() = 0;
        virtual void render() = 0;
        virtual void setInitPosition( uint16_t x, uint16_t y ) = 0;
    };

    class PModel
    {
    protected:
        Mstate state;
        double angle; // угол, под которым отображаем объект
        static const uint8_t amountOfDirections = 4; // длинна enum'а Mdirection
        std::array<int16_t, amountOfDirections> ms; // скорость перемещения
        double movementDirection; // направление перемещения(задаётся через угол)
        SDL_Rect position; // местоположение объекта
        uint16_t halfWidth; // половина ширины отображаемого спрайта
        uint16_t halfHeight; // половина высоты отображаемого спрайта
        SDL_Renderer* gRenderer;
        SDL_RendererFlip flip;
    public:
        PModel();
        ~PModel();
    };
}

#endif // MODEL_H
