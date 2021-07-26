#include "gun.h"

namespace GUN {
    gun::gun(
            double *angle,
            Gtype t
    ) : lastShotTime( 0 ) {
        anglePtr = angle;
        type = t;
    }

    gun::~gun() {

    }

    bool gun::fire( Uint32 currentTime,
                    int directionX,
                    int directionY ) {
        if( currentTime - lastShotTime < 800 )
            return false;

        lastShotTime = currentTime;
        return true;
    }
}
