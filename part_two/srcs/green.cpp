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

  auto image = cv::imread("images/green.jpg");
  cv::imshow("Image", image);
  auto crop = background(cv::Range(200, image.rows + 200),
                         cv::Range(200, image.cols + 200));

  cv::imshow("Crop", crop);

  cv::waitKey(0);

  return 0;
}
