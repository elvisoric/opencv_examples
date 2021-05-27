#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Please provide background image" << std::endl;
    return -1;
  }

  const auto backgroundSource = argv[1];
  auto background = cv::imread(backgroundSource);
  cv::imshow("Background", background);

  auto image = cv::imread("images/green.jpg");
  cv::imshow("Image", image);

  cv::resize(background, background, cv::Size(), 0.5, 0.5);

  int startPointX = 350;
  int startPointY = 550;
  auto crop = background(cv::Range(startPointY, image.rows + startPointY),
                         cv::Range(startPointX, image.cols + startPointX));
  cv::imshow("crop", crop);

  cv::Mat hsv;
  cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

  cv::Scalar lower{58, 70, 50};
  cv::Scalar upper{76, 255, 255};

  cv::Mat mask, foreMask;
  cv::inRange(hsv, lower, upper, mask);

  auto kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
  cv::Mat maskDialated;
  cv::dilate(mask, mask, kernel);

  cv::bitwise_not(mask, foreMask);

  cv::imshow("mask", mask);
  cv::imshow("foreMask", foreMask);

  cv::Mat channelsBackground[3];
  cv::split(crop, channelsBackground);

  cv::Mat channels[3];
  cv::split(image, channels);

  for (int i = 0; i < 3; ++i) {
    cv::bitwise_and(channelsBackground[i], mask, channelsBackground[i]);
    cv::bitwise_and(channels[i], foreMask, channels[i]);
  }
  cv::Mat finalBackground, finalForeground;
  cv::merge(channelsBackground, 3, finalBackground);
  cv::imshow("Final Background", finalBackground);
  cv::merge(channels, 3, finalForeground);
  cv::imshow("Final Foreground", finalForeground);

  cv::Mat result = finalBackground + finalForeground;
  cv::imshow("Result", result);

  cv::waitKey();
  cv::destroyAllWindows();

  return 0;
}
