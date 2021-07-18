#include "model.h"

namespace MODEL {
    Basic::Basic()
    {
        angle = 0.0;
        for ( auto it = ms.begin(); it != ms.end(); ++it ) {
            *it = 0;
        }
    }

    Basic::~Basic()
    {

    }
}
