//
// Created by Admin on 2023/10/24.
//

#ifndef NANOBREATHCONTROLLERUSBDRIVER_NANOEVENTFILTER_H
#define NANOBREATHCONTROLLERUSBDRIVER_NANOEVENTFILTER_H

#include <QObject>
#include <QEvent>

#define NEF_PARAMS QObject *watched, QEvent *event
#define NEF_RETURN return QObject::eventFilter(watched, event);

class NanoEventFilter : public QObject {
    Q_OBJECT
private:
    std::function<bool(QObject*, QEvent*)> callback;
public:
    /**
     * @brief    <br><p>构造函数</p>
     * <p>Constructor.</p>
     * @param callback <p>回调函数，在eventFilter()内被调用。</p>
     * <p>Callback method which is the only thing called inside eventFilter().</p>
     * @author  A-KRY
     * @date    2023/10/24 9:57
     */
    explicit NanoEventFilter(const std::function<bool(QObject *, QEvent *)> &callback = nullptr);

    /**
     * @brief    <br><p>只做一件事，调用 <strong><i>callback();</i></strong></p>
     * <p>Only do one thing, calls <strong><i>callback();</i></strong></p>
     * @return  return callback(watched, event);
     * @author  A-KRY
     * @date    2023/10/24 10:02
     */
    bool eventFilter(QObject *watched, QEvent *event) override;

    /**
     * @brief    <br><p>成员 <strong><i>callback</i></strong> 的 setter。</p>
     * <p>Setter of member <strong><i>callback</i></strong>.</p>
     * @param   callbackMethod <p>在这里重载 <strong><i>eventFilter()</strong></i></p>
     * <p>Override <strong><i>eventFilter()</strong></i> here.</p>
     * @author  A-KRY
     * @date    2023/10/24 10:05
     */
    void setCallback(const std::function<bool(QObject *, QEvent *)> &callbackMethod);

};


#endif //NANOBREATHCONTROLLERUSBDRIVER_NANOEVENTFILTER_H
