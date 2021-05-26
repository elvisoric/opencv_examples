#include <opencv2/opencv.hpp>

std::vector<cv::Point> dPoints;

void onMouse(int action, int x, int y, int, void*) {
  if (action == cv::EVENT_LBUTTONDOWN) {
    std::cout << x << " " << y << std::endl;
    if (dPoints.size() == 4) dPoints.clear();
    dPoints.push_back(cv::Point{x, y});
  }
}

std::vector<cv::Point> sourcePoints(int width, int height) {
  return {{0, 0}, {width, 0}, {width, height}, {0, height}};
}

int main() {
  const auto mainWindow = "Main Window";
  cv::namedWindow(mainWindow);
  cv::setMouseCallback(mainWindow, onMouse);
  auto image = cv::imread("images/bicom.jpg");
  cv::imshow("Image", image);

  cv::VideoCapture cap{1};
  cv::Mat frame;
  while (cap.isOpened()) {
    cap >> frame;
    if (frame.empty()) break;
    for (const auto& p : dPoints) {
      cv::circle(frame, p, 5, cv::Scalar{0, 200, 0}, -1);
    }
    if (dPoints.size() == 4) {
      auto sPoints = sourcePoints(image.cols, image.rows);
      cv::Mat homography = cv::findHomography(sPoints, dPoints, cv::RANSAC);
      cv::Mat result, gray, mask, finalResult, maskAll;
      cv::warpPerspective(image, result, homography, frame.size());
      cv::cvtColor(result, gray, cv::COLOR_BGR2GRAY);
      cv::threshold(gray, mask, 2, 255, cv::THRESH_BINARY);
      cv::bitwise_not(mask, mask);
      cv::imshow("mask", mask);

      cv::imshow("gray", gray);
      cv::imshow("Result", result);
      cv::Mat maskChannels[3] = {mask, mask, mask};
      cv::merge(maskChannels, 3, maskAll);

      cv::bitwise_and(frame, maskAll, frame);
      cv::add(frame, result, frame);
    }
    cv::imshow(mainWindow, frame);

    cv::waitKey(25);
  }

  cv::waitKey();
  cv::destroyAllWindows();
  return 0;
}
