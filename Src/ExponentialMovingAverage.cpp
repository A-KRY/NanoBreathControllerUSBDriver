//
// Created by Admin on 2023/10/22.
//

#include "../Inc/ExponentialMovingAverage.h"

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

[[maybe_unused]] void ExponentialMovingAverage::reset() {
    isReset = true;
}

[[maybe_unused]] void ExponentialMovingAverage::resetAll() {
    alpha = 1.0;
    isReset = true;
}

[[maybe_unused]] double ExponentialMovingAverage::MinAlpha() {
    return minAlpha;
}

[[maybe_unused]] double ExponentialMovingAverage::MaxAlpha() {
    return maxAlpha;
}
