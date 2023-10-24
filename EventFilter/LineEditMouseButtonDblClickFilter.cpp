//
// Created by Admin on 2023/10/23.
//

#include "LineEditMouseButtonDblClickFilter.h"

LineEditMouseButtonDblClickFilter::LineEditMouseButtonDblClickFilter(QLineEdit* lineEdit, std::function<void()> onMouseButtonDblClick) :
    lineEdit(lineEdit)
    , onMouseButtonDblClick(std::move(onMouseButtonDblClick))
{

}

bool LineEditMouseButtonDblClickFilter::eventFilter(QObject *watched, QEvent *event) {
    if (watched == lineEdit and event->type() == QEvent::MouseButtonDblClick) {
        onMouseButtonDblClick();
        return true;
    }
    return QObject::eventFilter(watched, event);
}
