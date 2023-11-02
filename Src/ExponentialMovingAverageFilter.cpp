//
// Created by Admin on 2023/10/22.
//

#include "../Inc/ExponentialMovingAverageFilter.h"

// Constants
const double ExponentialMovingAverageFilter::minAlpha = 0.001, ExponentialMovingAverageFilter::maxAlpha = 0.1;

// Methods
ExponentialMovingAverageFilter::ExponentialMovingAverageFilter(double alpha) :
    alpha(alpha)
    , ema(0.0)
    , isReset(true)
{

}

void ExponentialMovingAverageFilter::setAlpha(double newAlpha) {
    ExponentialMovingAverageFilter::alpha = newAlpha;
}

double ExponentialMovingAverageFilter::next(double value) {
    if (isReset) {
        isReset = false;
        ema = value;
    }
    else {
        ema = alpha * value + (1 - alpha) * ema;
    }
    return ema;
}

[[maybe_unused]] void ExponentialMovingAverageFilter::reset() {
    isReset = true;
}

[[maybe_unused]] void ExponentialMovingAverageFilter::resetAll() {
    alpha = maxAlpha;
    isReset = true;
}

[[maybe_unused]] double ExponentialMovingAverageFilter::MinAlpha() {
    return minAlpha;
}

[[maybe_unused]] double ExponentialMovingAverageFilter::MaxAlpha() {
    return maxAlpha;
}

[[maybe_unused]] double ExponentialMovingAverageFilter::getAlpha() const {
    return alpha;
}
