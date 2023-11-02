//
// Created by Admin on 2023/10/22.
//

#ifndef NANOBREATHCONTROLLERUSBDRIVER_EXPONENTIALMOVINGAVERAGE_H
#define NANOBREATHCONTROLLERUSBDRIVER_EXPONENTIALMOVINGAVERAGE_H

class ExponentialMovingAverage {
private:
    /**
     * @brief   <p>EMA 滤波器的 alpha 参数</p>
     * <p>The alpha parameter of EMA.</p>
     * @author  A-KRY
     * @date    2023/10/22 19:34
     */
    double alpha;

    /**
     * @brief   <p>指示 EMA 是否刚刚初始化或被重置</p>
     * <p>Shows if the EMA is initialized or reset.</p>
     * @author  A-KRY
     * @date    2023/10/22 19:36
     */
    bool isReset;

    /**
     * @brief   <p>最新的 EMA 值</p>
     * <p>The latest EMA value.</p>
     * @author  A-KRY
     * @date    2023/10/22 19:35
     */
    double ema;

    static const double minAlpha, maxAlpha;

public:
    explicit ExponentialMovingAverage(double alpha = 1.0);

    [[maybe_unused]] static double MinAlpha();

    [[maybe_unused]] static double MaxAlpha();

    /**
     * @brief   <p>设置 EMA 的 newAlpha 值</p>
     * <p>Set the newAlpha of EMA.</p>
     * @param   newAlpha a parameter of EMA
     * @author  A-KRY
     * @date    2023/10/22 19:28
     */
    void setAlpha(double newAlpha);

    /**
     * @brief   <p>过滤输入值</p>
     * <p>Filter the raw value.</p>
     * @param   rawValue New value about to filter.
     * @return  Value filtered by EMA.
     * @author  A-KRY
     * @date    2023/10/22 19:26
     */
    double filter(double rawValue);
    
    /**
     * @brief   <p>重置 EMA 值，alpha 值不变</p>
     * <p>Reset EMA value, alpha reserved.</p>
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


#endif //NANOBREATHCONTROLLERUSBDRIVER_EXPONENTIALMOVINGAVERAGE_H
