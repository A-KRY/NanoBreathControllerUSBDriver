//
// Created by Admin on 2023/10/24.
//

#include "../Inc/NanoEventFilter.h"

NanoEventFilter::NanoEventFilter(
        const std::function<bool(QObject *, QEvent *)> &callback)
    : callback(callback)
{

}

bool NanoEventFilter::eventFilter(NEF_PARAMS) {
    return callback(watched, event);
}

void NanoEventFilter::setCallback(const std::function<bool(QObject *, QEvent *)> &callbackMethod) {
    NanoEventFilter::callback = callbackMethod;
}
