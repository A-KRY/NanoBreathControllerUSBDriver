//
// Created by Admin on 2023/10/22.
//

#ifndef NANOBREATHCONTROLLERUSBDRIVER_EXPONENTIALMOVINGAVERAGE_H
#define NANOBREATHCONTROLLERUSBDRIVER_EXPONENTIALMOVINGAVERAGE_H

class ExponentialMovingAverage {
private:
    double alpha;
    double ema;
    bool initialized;
public:
    explicit ExponentialMovingAverage(double alpha = 1.0, double ema = 0.0, bool initialized = true);

    /**
     * @brief   <p>设置 EMA 的 alpha 值</p>
     * <p>Set the alpha of EMA.</p>
     * @param   alpha a parameter of EMA
     * @author  A-KRY
     * @date    2023/10/22 19:28
     */
    void setAlpha(double alpha);

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
    void reset();

    /**
     * @brief   <p>重置所有参数</p>
     * <p>Reset all parameters</p>
     * @author  A-KRY
     * @date    2023/10/22 19:26
     */
    void resetAll();
};


#endif //NANOBREATHCONTROLLERUSBDRIVER_EXPONENTIALMOVINGAVERAGE_H
