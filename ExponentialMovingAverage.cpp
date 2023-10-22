//
// Created by Admin on 2023/10/22.
//

#include "ExponentialMovingAverage.h"

ExponentialMovingAverage::ExponentialMovingAverage(double alpha, double ema, bool initialized) :
    alpha(alpha)
    , ema(ema)
    , initialized(initialized)
{

}

void ExponentialMovingAverage::setAlpha(double alpha) {
    ExponentialMovingAverage::alpha = alpha;
}

double ExponentialMovingAverage::filter(double rawValue) {
    if (!initialized) {
        initialized = true;
        ema = rawValue;
    }
    else {
        ema = alpha * rawValue + (1 - alpha) * ema;
    }
    return ema;
}

void ExponentialMovingAverage::reset() {
    ema = 0.0;
    initialized = true;
}

void ExponentialMovingAverage::resetAll() {
    alpha = 1.0;
    ema = 0.0;
    initialized = true;
}
