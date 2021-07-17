#include "model.h"
#include <iostream>

namespace MODEL {

    PModel::PModel()
    {
        angle = 0.0;
        for ( auto it = ms.begin(); it != ms.end(); ++it ) {
            *it = 0;
        }
    }

    PModel::~PModel()
    {

    }
}
