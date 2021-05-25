#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
  auto image = cv::imread("images/split.png");

  cv::imshow("original", image);

  cv::Mat hsv;
  cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

  cv::Mat channels[3];
  cv::split(hsv, channels);
  cv::imshow("H", channels[0]);
  cv::imshow("S", channels[1]);
  cv::imshow("V", channels[2]);

  auto factor = channels[2].clone();
  factor.setTo(50);
  cv::Mat newImage = channels[2] + factor;
  cv::imshow("newImage", newImage);

  channels[2] = newImage;

  cv::Mat finalImage;
  cv::merge(channels, 3, finalImage);
  cv::cvtColor(finalImage, finalImage, cv::COLOR_HSV2BGR);
  cv::imshow("final", finalImage);
  cv::waitKey();
  cv::destroyAllWindows();
  return 0;
}

// int main(int argc, char** argv) {
//   if (argc < 2) {
//     std::cout << "Please provide background as parameter" << std::endl;
//     return -1;
//   }
//
//   const auto backgorundSource = argv[1];
//   auto background = cv::imread(backgorundSource);
//
//   cv::imshow("Background", background);
//
//   auto image = cv::imread("images/green.jpg");
//   cv::imshow("Image", image);
//
//   cv::waitKey(0);
//
//   return 0;
// }
