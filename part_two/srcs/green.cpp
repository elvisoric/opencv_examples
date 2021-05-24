#include <iostream>
#include <opencv2/opencv.hpp>
int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Please provide background as parameter" << std::endl;
    return -1;
  }

  const auto backgorundSource = argv[1];
  auto background = cv::imread(backgorundSource);

  cv::imshow("Background", background);
  cv::VideoCapture cap(0);
  cv::Mat frame;
  while (cap.isOpened()) {
    cap >> frame;
    if (frame.empty()) break;
    cv::imshow("Frame", frame);
    cv::waitKey(25);
  }

  cv::waitKey(0);

  return 0;
}
