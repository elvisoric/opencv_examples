#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat hsv;

double hueValue = 100;
void onMouse(int action, int x, int y, int, void*) {
  if (action == cv::EVENT_LBUTTONDOWN) {
    std::cout << hsv.at<cv::Vec3b>(y, x) << std::endl;
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Please provide background as parameter" << std::endl;
    return -1;
  }

  const auto windowName = "Window";
  cv::namedWindow(windowName);

  cv::setMouseCallback(windowName, onMouse);

  const auto backgorundSource = argv[1];
  auto background = cv::imread(backgorundSource);

  auto image = cv::imread("images/green.jpg");
  cv::resize(background, background, cv::Size(), 0.5, 0.5);
  cv::imshow(windowName, image);
  int startPointX = 350;
  int startPointY = 550;
  auto crop = background(cv::Range(startPointY, image.rows + startPointY),
                         cv::Range(startPointX, image.cols + startPointX));
  cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
  cv::Mat channels[3];
  cv::Mat finalResultChannels[3];
  cv::split(image, channels);

  std::cout << hsv.at<cv::Vec3b>(100, 100) << std::endl;

  cv::Scalar lower{58, 70, 50};
  cv::Scalar upper{76, 255, 255};

  cv::Mat mask, foreMask;
  cv::inRange(hsv, lower, upper, mask);
  cv::bitwise_not(mask, foreMask);

  cv::Mat channelsBackground[3];
  cv::split(crop, channelsBackground);

  for (int i = 0; i < 3; ++i) {
    cv::bitwise_and(channelsBackground[i], mask, channelsBackground[i]);
    cv::bitwise_and(channels[i], foreMask, channels[i]);
  }

  cv::imshow("mask", mask);
  cv::imshow("foremask", foreMask);
  cv::Mat finalBackground, finalForeground;
  cv::merge(channelsBackground, 3, finalBackground);
  cv::merge(channels, 3, finalForeground);

  cv::imshow("Final background", finalBackground);
  cv::imshow("Final fore", finalForeground);
  cv::Mat result = finalBackground + finalForeground;

  cv::imshow("Crop", crop);
  cv::imshow("Result", result);

  cv::waitKey(0);

  return 0;
}
