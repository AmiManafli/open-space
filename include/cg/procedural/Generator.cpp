#include "Generator.h"

double gaussianProbability(double x, double mean, double sigma) {
    return glm::exp(-0.5 * pow((x - mean) / sigma, 2)) / (sigma * glm::sqrt(2 * PI));
}

double normalized(SeedType value) {
    return (1.0 + value / static_cast<double>(MAX_SEED)) / 2.0;
}

double gaussian(SeedType value, double min, double max, double mean, double sigma) {
    std::default_random_engine generator(value);
    std::normal_distribution<double> distribution(mean, sigma);
    return distribution(generator);
}

double limit(SeedType value, double min, double max) {
    return min + (max - min) * normalized(value);
}
