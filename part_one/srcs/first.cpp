#include <util.hpp>
#include <iostream>
int main() {
  cv::Mat image = cv::imread("images/number_zero.jpg");
  std::cout << image << std::endl;
  std::cout << biagile::parseType(image.type()) << std::endl;

  std::cout << image.cols << std::endl;
  std::cout << image.rows << std::endl;

  std::cout << "\n\n\n";

  auto grayImage = cv::imread("images/number_zero.jpg", cv::IMREAD_GRAYSCALE);
  std::cout << grayImage << std::endl;
  std::cout << biagile::parseType(grayImage.type()) << std::endl;

  std::cout << grayImage.cols << std::endl;
  std::cout << grayImage.rows << std::endl;

  // RGB
  // OpenCV BGR
  cv::Mat customImage{5, 7, CV_8UC3, cv::Scalar{20, 100, 45}};
  std::cout << customImage << std::endl;
  std::cout << customImage.size() << std::endl;

  auto clone = image.clone();
  std::cout << clone << std::endl;

  return 0;
}
