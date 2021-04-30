#include <util.hpp>
#include <iostream>
int main() {
  auto image = cv::imread("images/threshold.png", cv::IMREAD_GRAYSCALE);
  cv::imshow("Image", image);

  cv::Mat thresh, threshInv;
  cv::threshold(image, thresh, 120, 255, cv::THRESH_BINARY);
  cv::imshow("threshold", thresh);

  cv::threshold(image, threshInv, 120, 255, cv::THRESH_BINARY_INV);
  cv::imshow("threshold inv", threshInv);

  cv::waitKey(0);
  cv::destroyAllWindows();
  return 0;
}
