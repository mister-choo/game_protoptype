#pragma once

#include <array>
#include <vector>
template <class T> using Matrix = std::vector<std::vector<T>>;

class PerlinNoise {
  std::vector<int> p;
  double fade(double t);
  double lerp(double t, double a, double b);
  double grad(int hash, double x, double y, double z);
  double insertionSort_Middle(std::vector<double> a);
  double Common_Middle(std::vector<double> a);
  void Common_Filter(int N);
  void Median_Filter(int N);

public:
  Matrix<double> M;
  PerlinNoise();
  PerlinNoise(unsigned int seed);
  double noise(double x, double y, double z);
  void fill_matrix(double z, int width, int height);
};
