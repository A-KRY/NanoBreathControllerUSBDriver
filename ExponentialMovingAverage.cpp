//
// Created by Admin on 2023/10/22.
//

#include "ExponentialMovingAverage.h"

ExponentialMovingAverage::ExponentialMovingAverage(double alpha, double ema, bool isReset) :
        alpha(alpha)
    , ema(ema)
    , isReset(isReset)
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
