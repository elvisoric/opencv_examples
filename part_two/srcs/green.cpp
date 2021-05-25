#include <iostream>

#include <opencv2/opencv.hpp>
using namespace cv;

Mat pencilSketch(Mat image, int arguments = 0) {
  Mat pencilSketchImage;
  cv::Mat gray, bluredImage;
  cv::GaussianBlur(image, bluredImage, cv::Size(3, 3), 0);
  cv::cvtColor(bluredImage, gray, cv::COLOR_BGR2GRAY);
  cv::Laplacian(gray, pencilSketchImage, image.depth(), 5);
  cv::bitwise_not(pencilSketchImage, pencilSketchImage);
  cv::threshold(pencilSketchImage, pencilSketchImage, 120, 255,
                cv::THRESH_BINARY);

  return pencilSketchImage;
}

Mat cartoonify(Mat image, int arguments = 0) {
  Mat cartoonImage;
  auto pencilImage = pencilSketch(image);

  cv::Mat bluredImage;
  cv::bilateralFilter(image, bluredImage, 35, 80, 80);

  cv::cvtColor(pencilImage, pencilImage, cv::COLOR_GRAY2BGR);
  cv::bitwise_and(bluredImage, pencilImage, cartoonImage);

  return cartoonImage;
}

int main() {
  cv::VideoCapture cap{0};

  cv::Mat image;
  while (cap.isOpened()) {
    cap >> image;
    if (image.empty()) break;
    cv::Mat cartoonImage = pencilSketch(image);
    cv::imshow("Cartoon", cartoonImage);
    cv::waitKey(25);
  }
  cv::destroyAllWindows();
  return 0;
}
