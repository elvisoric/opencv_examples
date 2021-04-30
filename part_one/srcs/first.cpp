#include <util.hpp>
#include <iostream>
int main() {
  cv::Mat image = cv::imread("images/number_zero.jpg");
  image.at<cv::Vec3b>(0, 0) = cv::Vec3b{0, 0, 250};
  image(cv::Range(2, 4), cv::Range(4, 9)).setTo(cv::Scalar(0, 0, 250));

  cv::imshow("Image", image);

  auto grayImage = cv::imread("images/number_zero.jpg", cv::IMREAD_GRAYSCALE);
  grayImage.at<uchar>(0, 0) = 123;
  grayImage.at<uchar>(2, 9) = 123;

  cv::imshow("Image gray", grayImage);

  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}
