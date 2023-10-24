//
// Created by Admin on 2023/10/23.
//

#ifndef NANOBREATHCONTROLLERUSBDRIVER_LINEEDITMOUSEBUTTONDBLCLICKFILTER_H
#define NANOBREATHCONTROLLERUSBDRIVER_LINEEDITMOUSEBUTTONDBLCLICKFILTER_H

#include <QLineEdit>
#include <QMouseEvent>
#include <utility>

class LineEditMouseButtonDblClickFilter : public QObject {
    Q_OBJECT
private:
    QLineEdit* lineEdit;
    std::function<void()> onMouseButtonDblClick;
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
public:
    LineEditMouseButtonDblClickFilter(QLineEdit* lineEdit, std::function<void()> onMouseButtonDblClick);
};


#endif //NANOBREATHCONTROLLERUSBDRIVER_LINEEDITMOUSEBUTTONDBLCLICKFILTER_H
