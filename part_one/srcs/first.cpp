#include <util.hpp>
#include <iostream>
int main() {
  cv::VideoCapture cap{"images/video.mp4"};
  auto width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  auto height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

  cv::VideoWriter out{"out.mp4", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                      20, cv::Size(width, height)};
  cv::Mat frame;
  while (cap.isOpened()) {
    cap >> frame;
    if (frame.empty()) break;
    cv::imshow("frame", frame);
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    out.write(gray);
    cv::waitKey(20);
  }

  cv::waitKey(0);
  cv::destroyAllWindows();

  cap.release();
  out.release();

  return 0;
}
