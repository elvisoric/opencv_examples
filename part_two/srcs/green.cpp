#include <opencv2/opencv.hpp>

std::vector<cv::Point> sPoints;
void onMouse(int action, int x, int y, int, void*) {
  if (action == cv::EVENT_LBUTTONDOWN) {
    std::cout << x << " " << y << std::endl;
    if (sPoints.size() == 4) sPoints.clear();
    sPoints.push_back(cv::Point{x, y});
  }
}

std::vector<cv::Point> destinationPoints(int width, int height) {
  return {{0, 0}, {width, 0}, {width, height}, {0, height}};
}

int main() {
  const auto mainWindow = "Main Window";
  cv::namedWindow(mainWindow);
  cv::setMouseCallback(mainWindow, onMouse);
  auto bicom = cv::imread("images/bicom.jpg");
  cv::imshow("Bicom", bicom);
  cv::VideoCapture cap{1};
  auto capWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  auto capHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  cv::Mat image;
  while (cap.isOpened()) {
    cap >> image;
    if (image.empty()) break;

    for (const auto& p : sPoints) {
      cv::circle(image, p, 5, cv::Scalar{0, 200, 0}, -1);
    }
    if (sPoints.size() == 4) {
      auto dPoints = destinationPoints(capWidth, capHeight);
      cv::Mat homography = cv::findHomography(dPoints, sPoints, cv::RANSAC);
      cv::Mat result;
      cv::warpPerspective(bicom, result, homography,
                          cv::Size(capWidth, capHeight));
      cv::imshow("Result", result);
    }
    cv::imshow(mainWindow, image);
    cv::waitKey(25);
  }

  cv::waitKey();
  cv::destroyAllWindows();

  return 0;
}
