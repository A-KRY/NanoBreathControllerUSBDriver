//
// Created by Admin on 2023/10/23.
//

#include "SliderCtrlLeftClickFilter.h"

bool SliderCtrlLeftClickFilter::eventFilter(QObject *watched, QEvent *event) {
    if (watched == slider and event->type() == QEvent::MouseButtonPress) {
        auto* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if (mouseEvent and (mouseEvent->button() == Qt::LeftButton)
            and (mouseEvent->modifiers() == Qt::ControlModifier)) {
            slider->setValue((slider->maximum()+slider->minimum())>>1);
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}
