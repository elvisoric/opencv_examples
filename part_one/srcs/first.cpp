#include <util.hpp>
#include <iostream>
int main() {
  cv::Mat image = cv::imread("images/split.png");
  cv::imshow("Image", image);

  cv::Mat channels[4];
  cv::split(image, channels);

  // BGR
  cv::imshow("Blue", channels[0]);
  cv::imshow("Green", channels[1]);
  cv::imshow("Red", channels[2]);

  cv::Mat all[3];
  all[0] = channels[2];
  all[1] = channels[1];
  all[2] = channels[0];

  cv::Mat rgb;
  cv::merge(all, 3, rgb);
  cv::imshow("rgb", rgb);

  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}
