#include <fmt/format.h>
#include <opencv2/opencv.hpp>

int main() {
  fmt::print("Hello world!\n");
  fmt::print("OpenCV version: {}\n", CV_VERSION);
  return 0;
}
