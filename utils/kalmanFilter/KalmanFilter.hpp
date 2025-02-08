#ifndef KALMAN_FILTER_HPP
#define KALMAN_FILTER_HPP

namespace kalman {

template <typename T>
class KalmanFilter {
public:
    /**
     * @brief Default constructor.
     * Initializes the filter with default values.
     *   - initial estimate: 0
     *   - initial error covariance: 1
     *   - process noise: 0.01
     *   - measurement noise: 1
     */
    KalmanFilter();

    /**
     * @brief Parameterized constructor.
     *
     * @param initialEstimate Initial state estimate.
     * @param initialErrorCov Initial error covariance.
     * @param processNoise    Process noise.
     * @param measurementNoise Measurement noise.
     */
    KalmanFilter(T initialEstimate, T initialErrorCov, T processNoise, T measurementNoise);

    /**
     * @brief Updates the filter with a new measurement.
     *
     * @param measurement The new measurement value.
     */
    void update(T measurement);

    /**
     * @brief Retrieves the current state estimate.
     *
     * @return T The current estimated state.
     */
    T getEstimate() const;

    // Optionally, add setters/getters to adjust process or measurement noise, or reset the filter.

private:
    T estimate_;
    T errorCov_;
    T processNoise_;
    T measurementNoise_;
};

} // namespace kalman

#include "KalmanFilter.tpp"

#endif // KALMAN_FILTER_HPP
