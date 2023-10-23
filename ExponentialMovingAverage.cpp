//
// Created by Admin on 2023/10/22.
//

#include "ExponentialMovingAverage.h"

// Constants
const double ExponentialMovingAverage::minAlpha = 0.1, ExponentialMovingAverage::maxAlpha = 1.0;

// Methods
ExponentialMovingAverage::ExponentialMovingAverage(double alpha) :
    alpha(alpha)
    , ema(0.0)
    , isReset(true)
{

}

void ExponentialMovingAverage::setAlpha(double newAlpha) {
    ExponentialMovingAverage::alpha = newAlpha;
}

double ExponentialMovingAverage::filter(double rawValue) {
    if (isReset) {
        isReset = false;
        ema = rawValue;
    }
    else {
        ema = alpha * rawValue + (1 - alpha) * ema;
    }
    return ema;
}

void ExponentialMovingAverage::reset() {
    isReset = true;
}

void ExponentialMovingAverage::resetAll() {
    alpha = 1.0;
    isReset = true;
}

double ExponentialMovingAverage::MinAlpha() {
    return minAlpha;
}

double ExponentialMovingAverage::MaxAlpha() {
    return maxAlpha;
}
