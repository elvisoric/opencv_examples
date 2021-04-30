#include <util.hpp>
#include <iostream>
int main() {
  auto original = cv::imread("images/coins.jpeg");
  cv::imshow("Original", original);

  cv::Mat gray;
  cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
  cv::imshow("Gray", gray);

  cv::Mat thresh, dilatedImage, erodedImage;
  cv::threshold(gray, thresh, 110, 255, cv::THRESH_BINARY_INV);
  cv::imshow("Threshold", thresh);

  auto kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
  cv::dilate(thresh, dilatedImage, kernel);
  cv::imshow("DilatedImage", dilatedImage);

  cv::erode(dilatedImage, erodedImage, kernel);
  cv::erode(erodedImage, erodedImage, kernel);
  cv::imshow("ErodedImage", erodedImage);

  cv::Mat labels;
  auto numberOfComponentes = cv::connectedComponents(erodedImage, labels);
  std::cout << "Number of components: " << numberOfComponentes << std::endl;
  // Background is treated as one component
  std::cout << "Number of coins: " << numberOfComponentes - 1 << std::endl;

  // Show all components separatly
  // for (int i = 1; i < numberOfComponentes; ++i)
  //   cv::imshow(std::to_string(i), labels == i);

  cv::waitKey(0);
  cv::destroyAllWindows();
  return 0;
}
