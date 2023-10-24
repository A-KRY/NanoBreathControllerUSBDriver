//
// Created by Admin on 2023/10/23.
//

#ifndef NANOBREATHCONTROLLERUSBDRIVER_LINEEDITCLICKOUTSIDEFILTER_H
#define NANOBREATHCONTROLLERUSBDRIVER_LINEEDITCLICKOUTSIDEFILTER_H

#include <QMouseEvent>
#include <QLineEdit>
#include "../driverwindow.h"

/**
 * @note This event filter should be installed on the parent of a QLineEdit
 */
class LineEditClickOutsideFilter : public QObject {
    Q_OBJECT
private:
//    QLineEdit* lineEdit;
    std::function<void()> callback;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

public:
    LineEditClickOutsideFilter(QLineEdit* lineEdit,/* QWidget* parent, */std::function<void()> callback);
};


#endif //NANOBREATHCONTROLLERUSBDRIVER_LINEEDITCLICKOUTSIDEFILTER_H
