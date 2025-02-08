#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <vector>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

class Plotter {
public:
    static void updatePlot(const std::vector<double>& time_series,
                           const std::vector<double>& temp_values,
                           const std::vector<double>& kalman_values,
                           const std::vector<double>& anomaly_times,
                           const std::vector<double>& anomaly_values);
};

#endif // PLOTTER_HPP
