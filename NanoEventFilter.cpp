//
// Created by Admin on 2023/10/24.
//

#include "NanoEventFilter.h"

NanoEventFilter::NanoEventFilter(
        const std::function<bool(QObject *, QEvent *)> &callback , QObject *objPtr)
    : callback(callback)
    , objPtr(objPtr)
{

}

bool NanoEventFilter::eventFilter(QObject *watched, QEvent *event) {
    callback(watched, event);
    return QObject::eventFilter(watched, event);
}

void NanoEventFilter::setCallback(const std::function<bool(QObject *, QEvent *)> &callbackMethod) {
    NanoEventFilter::callback = callbackMethod;
}

void NanoEventFilter::setObjPtr(QObject *qObject) {
    NanoEventFilter::objPtr = qObject;
}
