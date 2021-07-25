#ifndef MODEL_H
#define MODEL_H

#include <SDL2/SDL.h>
#include "../assets.h"
#include "../config.h"

namespace MODEL {
    enum Mstate : uint8_t {
        initialized = 0, // объект инициализирован
        appearance,      // анимация появления
        alive,           // модель во время жизни
        dying            // гибель модели
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
        virtual void integrate( SDL_Rect &pos, double time, double timeDelta ) = 0;
        virtual void render( SDL_Rect pos ) = 0;
        virtual void setInitPosition( uint16_t screenW, uint16_t screenH ) = 0;
    };

    class Basic
    {
    protected:
        double angle; // угол, под которым отображаем объект
        SDL_Renderer* gRenderer;
        SDL_RendererFlip flip;
        long accumForStep; // остаток времени от дельты при движении
        static const uint8_t amountOfDirections = 4; // длинна enum'а Mdirection
        std::array<int16_t, amountOfDirections> ms; // скорость перемещения
        double movementDirection; // направление перемещения(задаётся через угол)
    public:
        Basic();
        ~Basic();
    };

    class PModel
    {
    protected:
        Mstate state;
        SDL_Rect position; // местоположение объекта
        uint16_t halfWidth; // половина ширины видимого спрайта
        uint16_t halfHeight; // половина высоты видимого спрайта
    public:
        PModel();
        ~PModel();
    };
}

#endif // MODEL_H
