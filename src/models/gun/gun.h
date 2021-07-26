#ifndef GUN_H
#define GUN_H

#include <SDL2/SDL.h>

namespace GUN{
    enum Gtype {
        monster,
        player
    };

    class gun {
        Gtype type; // кому принадлежит оружие?
        double *anglePtr;
        Uint32 lastShotTime;
    public:
        gun(double *angle, Gtype t);
        ~gun();

        bool fire( Uint32 currentTime, // текущее время
                int directionX, // координаты места вылета пули
                int directionY
        );
    };
}

#endif // GUN_H
