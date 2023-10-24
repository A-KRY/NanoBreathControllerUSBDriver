//
// Created by Admin on 2023/10/23.
//

#include "LineEditClickOutsideFilter.h"


#include <utility>

LineEditClickOutsideFilter::LineEditClickOutsideFilter(QLineEdit *lineEdit, QWidget *parent, std::function<void()> callback) :
    QObject(parent)
//    , lineEdit(lineEdit)
    , callback(std::move(callback))
{

}

bool LineEditClickOutsideFilter::eventFilter(QObject *watched, QEvent *event) {
    auto driverWindow = qobject_cast<DriverWindow*>(watched);
    if (driverWindow and event->type() == QEvent::MouseButtonPress) {
        auto lineEdit = driverWindow->findChild<QLineEdit*>("smoothnessLineEdit");
        if (lineEdit and lineEdit->hasFocus() and !lineEdit->rect().contains(lineEdit->mapFromGlobal(QCursor::pos()))) {
            callback();
        }
    }
    return QObject::eventFilter(watched, event);
}


