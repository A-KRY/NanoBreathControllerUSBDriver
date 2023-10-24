//
// Created by Admin on 2023/10/24.
//

#ifndef NANOBREATHCONTROLLERUSBDRIVER_NANOEVENTFILTER_H
#define NANOBREATHCONTROLLERUSBDRIVER_NANOEVENTFILTER_H

#include <QObject>
#include <QEvent>

class NanoEventFilter : public QObject {
    Q_OBJECT
private:
    std::function<bool(QObject*, QEvent*)> callback;
    QObject* objPtr;
public:
    /**
     * @brief   <p>构造函数</p>
     * <p>Constructor.</p>
     * @param callback <p>回调函数，在eventFilter()内被调用。</p>
     * <p>Callback method which is called inside eventFilter().</p>
     * @param objPtr <p>可能用得到的指针，可指向QObject的任意公共子类。</p>
     * <p>Pointer maybe used, which can point to any public subclass of QObject.</p>
     * @author  A-KRY
     * @date    2023/10/24 9:57
     */
    explicit NanoEventFilter(const std::function<bool(QObject *, QEvent *)> &callback = nullptr, QObject *objPtr = nullptr);

    /**
     * @brief   <p>只做一件事，调用 <strong><i>callback(watched, event);</i></strong></p>
     * <p>Only do one thing, calls <strong><i>callback(watched, event);</i></strong></p>
     * @return  return QObject::eventFilter(watched, event);
     * @author  A-KRY
     * @date    2023/10/24 10:02
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief   <p>成员 <strong><i>callback</i></strong> 的 setter</p>
     * <p>Setter of member <strong><i>callback</i></strong></p>
     * @param   callbackMethod 顾名思义 | Just as its name implies.
     * @author  A-KRY
     * @date    2023/10/24 10:05
     */
    void setCallback(const std::function<bool(QObject *, QEvent *)> &callbackMethod);

    /**
     * @brief   <p>成员 <strong><i>objPtr</i></strong> 的 setter</p>
     * <p>Setter of member <strong><i>objPtr</i></strong></p>
     * @param   callbackMethod 顾名思义 | Just as its name implies.
     * @author  A-KRY
     * @date    2023/10/24 10:05
     */
    void setObjPtr(QObject *qObject);

};


#endif //NANOBREATHCONTROLLERUSBDRIVER_NANOEVENTFILTER_H
