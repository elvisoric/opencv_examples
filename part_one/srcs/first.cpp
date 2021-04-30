#include <util.hpp>
#include <iostream>
int main() {
  cv::Mat image = cv::imread("images/number_zero.jpg");
  image.at<cv::Vec3b>(0, 0) = cv::Vec3b{99, 99, 99};
  image(cv::Range(2, 4), cv::Range(4, 9)).setTo(cv::Scalar(99, 99, 99));
  std::cout << image << std::endl;

  std::cout << "\n\n\n";

  auto grayImage = cv::imread("images/number_zero.jpg", cv::IMREAD_GRAYSCALE);
  std::cout << static_cast<int>(grayImage.at<uchar>(0, 0)) << std::endl;
  grayImage.at<uchar>(0, 0) = 123;
  grayImage.at<uchar>(2, 9) = 123;
  std::cout << grayImage << std::endl;

  return 0;
}
