#include <util.hpp>
#include <iostream>
int main() {
  auto original = cv::imread("images/coins.jpeg");

  cv::Mat gray;
  cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);

  cv::Mat thresh, dilatedImage, erodedImage;
  cv::threshold(gray, thresh, 110, 255, cv::THRESH_BINARY_INV);

  auto kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
  cv::dilate(thresh, dilatedImage, kernel);

  cv::erode(dilatedImage, erodedImage, kernel);
  cv::erode(erodedImage, erodedImage, kernel);

  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours(erodedImage, contours, hierarchy, cv::RETR_LIST,
                   cv::CHAIN_APPROX_SIMPLE);

  std::cout << "Number of coins: " << contours.size() << std::endl;
  cv::drawContours(original, contours, -1, cv::Scalar{0, 240, 0}, 5);

  cv::imshow("Original", original);

  cv::waitKey(0);
  cv::destroyAllWindows();
  return 0;
}
