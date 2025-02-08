#include "plotting/Plotter.hpp"

void Plotter::updatePlot(const std::vector<double>& time_series,
                         const std::vector<double>& temp_values,
                         const std::vector<double>& kalman_values,
                         const std::vector<double>& anomaly_times,
                         const std::vector<double>& anomaly_values) {
    plt::clf();

    plt::named_plot("Raw Temperature", time_series, temp_values, "r-");
    plt::named_plot("Kalman Filter", time_series, kalman_values, "b-");

    // Plot anomalies as red circles
    if (!anomaly_times.empty()) {
        plt::scatter(anomaly_times, anomaly_values, 100, {{"color", "red"}, {"marker", "o"}, {"label", "Anomalies"}});
    }

    plt::xlabel("Time");
    plt::ylabel("Temperature (°C)");
    plt::title("Real-Time Temperature Monitoring");

    plt::legend();
    plt::grid(true);

    // Save the figure instead of showing it
    plt::save("temperature_plot.png");
}
