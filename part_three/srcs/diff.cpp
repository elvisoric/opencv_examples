#include <opencv2/opencv.hpp>

int main() {
  auto first = cv::imread("images/first.png");
  auto second = cv::imread("images/second.png");

  cv::imshow("first", first);
  cv::imshow("second", second);

  // cv::Mat diff1 = first - second;
  // cv::imshow("diff1", diff1);
  // cv::Mat diff2 = second - first;
  // cv::imshow("diff2", diff2);
  // auto diff = diff1 + diff2;
  cv::Mat diff;
  cv::absdiff(first, second, diff);
  cv::imshow("diff", diff);

  cv::Mat gray, mask;
  cv::cvtColor(diff, gray, cv::COLOR_BGR2GRAY);
  cv::imshow("gray", gray);

  cv::threshold(gray, mask, 50, 255, cv::THRESH_BINARY);

  auto kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(39, 39));
  cv::dilate(mask, mask, kernel);
  // find contours
  // find bounding boxes  Rect
  // black mask
  cv::imshow("mask", mask);

  cv::Mat hsv;
  cv::Mat hsvChannels[3];
  cv::cvtColor(second, hsv, cv::COLOR_BGR2HSV);
  cv::split(hsv, hsvChannels);
  auto valueClone = hsvChannels[2].clone();

  hsvChannels[2] *= 0.4;
  cv::bitwise_and(valueClone, mask, valueClone);
  cv::Mat maskInv;
  cv::bitwise_not(mask, maskInv);
  cv::bitwise_and(hsvChannels[2], maskInv, hsvChannels[2]);

  hsvChannels[2] += valueClone;
  cv::merge(hsvChannels, 3, hsv);

  cv::imshow("hsvChannels[2]", hsvChannels[2]);
  cv::imshow("valueClone", valueClone);

  cv::Mat result;
  cv::cvtColor(hsv, result, cv::COLOR_HSV2BGR);
  cv::imshow("result", result);

  cv::waitKey();
  cv::destroyAllWindows();
  return 0;
}
