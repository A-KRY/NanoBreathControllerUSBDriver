//
// Created by Admin on 2023/10/23.
//

#ifndef NANOBREATHCONTROLLERUSBDRIVER_SLIDERCTRLLEFTCLICKFILTER_H
#define NANOBREATHCONTROLLERUSBDRIVER_SLIDERCTRLLEFTCLICKFILTER_H

#include <QSlider>
#include <QMouseEvent>
#include <QKeyEvent>

class SliderCtrlLeftClickFilter : public QObject {
    Q_OBJECT
private:
    QSlider* slider;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

public:
    explicit SliderCtrlLeftClickFilter(QSlider* slider) : slider(slider) {

    }
};


#endif //NANOBREATHCONTROLLERUSBDRIVER_SLIDERCTRLLEFTCLICKFILTER_H
