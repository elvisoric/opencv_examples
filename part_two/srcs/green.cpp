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

int main() {
  const auto mainWindow = "Main Window";
  cv::namedWindow(mainWindow);
  cv::setMouseCallback(mainWindow, onMouse);
  auto image = cv::imread("images/sudoku.jpg");

  auto cloneImage = image.clone();
  while (true) {
    for (const auto& p : dPoints) {
      cv::circle(image, p, 5, cv::Scalar{0, 200, 0}, -1);
    }
    if (dPoints.size() == 4) {
      auto sPoints = dPoints;
      auto destPoints = destinationPoints(1000, 1000);
      cv::Mat homography = cv::findHomography(sPoints, destPoints, cv::RANSAC);
      cv::Mat result;
      cv::warpPerspective(cloneImage, result, homography, cv::Size(1000, 1000));
      cv::imshow("Result", result);
    }
    cv::imshow(mainWindow, image);

    cv::waitKey(25);
  }
  cv::waitKey();
  cv::destroyAllWindows();
  return 0;
}
