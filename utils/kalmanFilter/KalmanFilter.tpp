#ifndef KALMAN_FILTER_TPP
#define KALMAN_FILTER_TPP

#include "KalmanFilter.hpp"

namespace kalman {

// Default constructor implementation.
template <typename T>
KalmanFilter<T>::KalmanFilter()
    : estimate_(0), errorCov_(1), processNoise_(0.01), measurementNoise_(1) {}

// Parameterized constructor implementation.
template <typename T>
KalmanFilter<T>::KalmanFilter(T initialEstimate, T initialErrorCov, T processNoise, T measurementNoise)
    : estimate_(initialEstimate),
      errorCov_(initialErrorCov),
      processNoise_(processNoise),
      measurementNoise_(measurementNoise) {}

// The update() method applies a simple Kalman filter update to incorporate a new measurement.
template <typename T>
void KalmanFilter<T>::update(T measurement) {
    T predictedEstimate = estimate_;
    T predictedErrorCov = errorCov_ + processNoise_;
    T kalmanGain = predictedErrorCov / (predictedErrorCov + measurementNoise_);
    estimate_ = predictedEstimate + kalmanGain * (measurement - predictedEstimate);
    errorCov_ = (1 - kalmanGain) * predictedErrorCov;
}

// Returns the current estimated state.
template <typename T>
T KalmanFilter<T>::getEstimate() const {
    return estimate_;
}

} // namespace kalman

#endif // KALMAN_FILTER_TPP
