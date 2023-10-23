//
// Created by Admin on 2023/10/23.
//

#ifndef NANOBREATHCONTROLLERUSBDRIVER_SLIDEREVENTFILTER_H
#define NANOBREATHCONTROLLERUSBDRIVER_SLIDEREVENTFILTER_H

#include <QSlider>
#include <QMouseEvent>
#include <QKeyEvent>

class SliderEventFilter : public QObject {
    Q_OBJECT
private:
    QSlider* slider;

protected:

    bool eventFilter(QObject *watched, QEvent *event) override {
        if (watched == slider) {
            if (event->type() == QEvent::MouseButtonPress) {
                auto* mouseEvent = dynamic_cast<QMouseEvent*>(event);
                if (mouseEvent and (mouseEvent->button() == Qt::LeftButton)
                and (mouseEvent->modifiers() == Qt::ControlModifier)) {
                    slider->setValue((slider->maximum()+slider->minimum())>>1);
                    return true;
                }
            }
        }
        return QObject::eventFilter(watched, event);
    }

public:
    SliderEventFilter(QSlider* slider) : slider(slider) {

    }
};


#endif //NANOBREATHCONTROLLERUSBDRIVER_SLIDEREVENTFILTER_H
