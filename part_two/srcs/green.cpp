#include <opencv2/opencv.hpp>

std::vector<cv::Point> dPoints;

void onMouse(int action, int x, int y, int, void*) {
  if (action == cv::EVENT_LBUTTONDOWN) {
    std::cout << x << " " << y << std::endl;
    if (dPoints.size() == 4) dPoints.clear();
    dPoints.push_back(cv::Point{x, y});
  }
}

std::vector<cv::Point> destinationPoints(int width, int height) {
  return {{0, 0}, {width, 0}, {width, height}, {0, height}};
}

const int destWidth = 1200;
const double ratio = 0.75;
const int destHeight = destWidth * ratio;

int main() {
  const auto mainWindow = "Main Window";
  cv::namedWindow(mainWindow);
  cv::setMouseCallback(mainWindow, onMouse);
  cv::VideoCapture cap{1};
  cv::Mat frame;

  while (cap.isOpened()) {
    cap >> frame;
    if (frame.empty()) break;
    for (const auto& p : dPoints) {
      cv::circle(frame, p, 5, cv::Scalar{0, 200, 0}, -1);
    }
    if (dPoints.size() == 4) {
      auto sPoints = dPoints;
      auto destPoints = destinationPoints(destWidth, destHeight);
      cv::Mat homography = cv::findHomography(sPoints, destPoints, cv::RANSAC);
      cv::Mat result;
      cv::warpPerspective(frame, result, homography,
                          cv::Size(destWidth, destHeight));
      cv::imshow("Result", result);
    }
    cv::imshow(mainWindow, frame);
    cv::waitKey(25);
  }
  cv::waitKey();
  cv::destroyAllWindows();
  return 0;
}
