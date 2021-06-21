#include <iostream>
#include <opencv2/opencv.hpp>

const int width = 1200;
const double ratio = 0.75;
const int height = width * ratio;

std::vector<cv::Point> destinationPoints() {
  return {{0, 0}, {width, 0}, {0, height}, {width, height}};
}

std::vector<cv::Point> destinationPoints(int width, int height) {
  return {{0, 0}, {width, 0}, {0, height}, {width, height}};
}

std::vector<cv::Point> sourcePoints(int width, int height) {
  return {{0, 0}, {width - 1, 0}, {width - 1, height - 1}, {0, height}};
}

std::vector<cv::Point> dPoints;

void onMouse(int action, int x, int y, int, void*) {
  if (action == cv::EVENT_LBUTTONDOWN) {
    std::cout << x << " " << y << std::endl;
    if (dPoints.size() == 4) dPoints.clear();
    dPoints.push_back(cv::Point{x, y});
  }
}

int main() {
  const auto mainWindow = "main";
  cv::namedWindow(mainWindow);
  cv::setMouseCallback(mainWindow, onMouse);
  auto background = cv::imread("images/bicom.jpg");
  cv::imshow("background", background);

  cv::VideoCapture cap{0};
  cv::Mat frame;

  while (cap.isOpened()) {
    cap >> frame;
    if (frame.empty()) break;
    for (const auto& p : dPoints) {
      cv::circle(frame, p, 3, cv::Scalar{0, 230, 0}, -1);
    }

    if (dPoints.size() == 4) {
      std::vector<cv::Point> sPoints =
          sourcePoints(background.cols, background.rows);

      cv::Mat homography = cv::findHomography(sPoints, dPoints, cv::RANSAC);

      cv::Mat result;
      cv::warpPerspective(background, result, homography, frame.size());
      cv::imshow("Final", result);
      cv::Mat mask, gray;
      cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
      cv::imshow("Gray", gray);

      cv::threshold(gray, mask, 1, 255, cv::THRESH_BINARY);
      cv::imshow("mask", mask);
      cv::Mat maskFinal;
      cv::bitwise_not(mask, mask);
      cv::Mat maskChannels[3] = {mask, mask, mask};
      cv::merge(maskChannels, 3, maskFinal);
      cv::bitwise_and(frame, maskFinal, frame);
      cv::add(frame, result, frame);
    }

    cv::imshow(mainWindow, frame);

    cv::waitKey(20);
  }

  std::cout << cap.isOpened() << std::endl;
  cv::waitKey(0);

  return 0;
}
