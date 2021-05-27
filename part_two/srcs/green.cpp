#include <opencv2/opencv.hpp>

int main() {
  auto image = cv::imread("images/split.png");
  cv::imshow("Image", image);

  cv::Mat hsv;
  cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
  cv::imshow("HSV", hsv);
  cv::Mat channels[3];
  cv::split(hsv, channels);
  cv::imshow("H", channels[0]);
  cv::imshow("S", channels[1]);
  cv::imshow("V", channels[2]);

  auto factor = channels[2].clone().setTo(50);
  cv::Mat newImage = channels[2] + factor;
  cv::imshow("New V", newImage);
  channels[2] = newImage;
  cv::Mat result;
  cv::merge(channels, 3, hsv);
  cv::cvtColor(hsv, result, cv::COLOR_HSV2BGR);
  cv::imshow("Result", result);

  cv::waitKey();
  cv::destroyAllWindows();

  return 0;
}
