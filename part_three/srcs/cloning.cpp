#include <opencv2/opencv.hpp>

int main() {
  auto background = cv::imread("images/wood.png");
  auto foreground = cv::imread("images/bicom_paper.png");

  cv::imshow("background", background);
  cv::imshow("foreground", foreground);

  cv::Mat normalClone, mixedClone;
  cv::Mat mask =
      255 * cv::Mat::ones(foreground.rows, foreground.cols, foreground.depth());
  cv::Point center{foreground.cols / 2, foreground.rows / 2};
  cv::seamlessClone(foreground, background, mask, center, normalClone,
                    cv::NORMAL_CLONE);
  cv::seamlessClone(foreground, background, mask, center, mixedClone,
                    cv::MIXED_CLONE);

  cv::imshow("mixedClone", mixedClone);

  cv::waitKey();
  cv::destroyAllWindows();

  return 0;
}
