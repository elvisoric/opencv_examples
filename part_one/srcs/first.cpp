#include <util.hpp>
#include <iostream>
int main() {
  auto original = cv::imread("images/coins.jpeg");

  cv::Mat gray;
  cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
  cv::imshow("Gray", gray);

  cv::Mat thresh, dilatedImage, erodedImage;
  cv::threshold(gray, thresh, 110, 255, cv::THRESH_BINARY_INV);
  cv::imshow("Threshold", thresh);

  auto kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
  cv::dilate(thresh, dilatedImage, kernel);
  cv::imshow("DilatedImage", dilatedImage);

  cv::erode(dilatedImage, erodedImage, kernel);
  cv::erode(erodedImage, erodedImage, kernel);
  cv::imshow("ErodedImage", erodedImage);

  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours(erodedImage, contours, hierarchy, cv::RETR_LIST,
                   cv::CHAIN_APPROX_SIMPLE);

  std::cout << "Number of coins: " << contours.size() << std::endl;
  cv::drawContours(original, contours, -1, cv::Scalar{0, 240, 0}, 5);
  for (auto& contour : contours) {
    auto moment = cv::moments(contour);
    auto x = static_cast<int>(moment.m10 / moment.m00);
    auto y = static_cast<int>(moment.m01 / moment.m00);

    cv::circle(original, cv::Point(x, y), 10, cv::Scalar{0, 0, 230}, -1);
  }

  cv::imshow("Original", original);

  cv::waitKey(0);
  cv::destroyAllWindows();
  return 0;
}
