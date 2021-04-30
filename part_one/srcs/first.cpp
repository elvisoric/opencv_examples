#include <util.hpp>
#include <iostream>
int main() {
  cv::Mat image = cv::imread("images/color.jpg");
  cv::imshow("Image", image);

  auto crop = image(cv::Range(350, 650), cv::Range(300, 500));
  cv::imshow("Image crop", crop);

  cv::imwrite("crop.jpg", crop);

  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  cv::imshow("gray", gray);

  cv::imwrite("gray.jpg", gray);

  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}
