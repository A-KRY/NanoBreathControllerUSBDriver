//
// Created by Admin on 2023/10/22.
//

#ifndef NANOBREATHCONTROLLERUSBDRIVER_EXPONENTIALMOVINGAVERAGEFILTER_H
#define NANOBREATHCONTROLLERUSBDRIVER_EXPONENTIALMOVINGAVERAGEFILTER_H

#include <QObject>

class ExponentialMovingAverageFilter : public QObject {
    Q_OBJECT
private:
    /**
     * @brief   <p>smoothFilter 滤波器的 alpha 参数</p>
     * <p>The alpha parameter of smoothFilter.</p>
     * @author  A-KRY
     * @date    2023/10/22 19:34
     */
    double alpha;

    /**
     * @brief   <p>指示 smoothFilter 是否刚刚初始化或被重置</p>
     * <p>Shows if the smoothFilter is initialized or reset.</p>
     * @author  A-KRY
     * @date    2023/10/22 19:36
     */
    bool isReset;

    /**
     * @brief   <p>最新的 smoothFilter 值</p>
     * <p>The latest smoothFilter value.</p>
     * @author  A-KRY
     * @date    2023/10/22 19:35
     */
    double ema;

    static const double minAlpha, maxAlpha;

public:
    explicit ExponentialMovingAverageFilter(double alpha = minAlpha);

    [[maybe_unused]] static double MinAlpha();

    [[maybe_unused]] static double MaxAlpha();

    [[maybe_unused]] [[nodiscard]] double getAlpha() const;

    /**
     * @brief   <p>设置 smoothFilter 的 newAlpha 值</p>
     * <p>Set the newAlpha of smoothFilter.</p>
     * @param   newAlpha a parameter of smoothFilter
     * @author  A-KRY
     * @date    2023/10/22 19:28
     */
    void setAlpha(double newAlpha);

    /**
     * @brief   <p>过滤输入值</p>
     * <p>Filter the raw value.</p>
     * @param   value New value about to next.
     * @return  Value filtered by smoothFilter.
     * @author  A-KRY
     * @date    2023/10/22 19:26
     */
    double next(double value);
    
    /**
     * @brief   <p>重置 smoothFilter 值，alpha 值不变</p>
     * <p>Reset smoothFilter value, alpha reserved.</p>
     * @author  A-KRY
     * @date    2023/10/22 19:24
     */
    [[maybe_unused]] void reset();

    /**
     * @brief   <p>重置所有参数</p>
     * <p>Reset all parameters</p>
     * @author  A-KRY
     * @date    2023/10/22 19:26
     */
    [[maybe_unused]] void resetAll();
};


#endif //NANOBREATHCONTROLLERUSBDRIVER_EXPONENTIALMOVINGAVERAGEFILTER_H
