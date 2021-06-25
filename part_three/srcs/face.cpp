#include <opencv2/opencv.hpp>

int main() {
  auto source = cv::imread("images/obama.jpg");
  auto sourceMask = cv::imread("images/obama-mask.jpg", cv::IMREAD_GRAYSCALE);
  auto destination = cv::imread("images/trump.jpg");

  cv::imshow("source", source);
  cv::imshow("sourceMask", sourceMask);
  cv::imshow("destination", destination);

  cv::Mat alpha;
  cv::cvtColor(sourceMask, alpha, cv::COLOR_GRAY2BGR);
  alpha.convertTo(alpha, CV_32FC3);
  alpha = alpha / 255.0;

  source.convertTo(source, CV_32FC3);
  destination.convertTo(destination, CV_32FC3);

  std::vector<cv::Mat> sourceChannels(3);
  std::vector<cv::Mat> destinationChannels(3);
  std::vector<cv::Mat> alphaChannels(3);
  std::vector<cv::Mat> outputChannels(3);
  cv::split(source, sourceChannels);
  cv::split(destination, destinationChannels);
  cv::split(alpha, alphaChannels);

  for (int i = 0; i < 3; ++i) {
    outputChannels[i] = sourceChannels[i].mul(alphaChannels[i]) +
                        destinationChannels[i].mul(1 - alphaChannels[i]);
  }

  cv::Mat output;
  cv::merge(outputChannels, output);
  output.convertTo(output, CV_8UC3);
  cv::imshow("output", output);

  source.convertTo(source, CV_8UC3);
  destination.convertTo(destination, CV_8UC3);

  cv::Mat sourceMaskBin, outputClone, outputMixedClone;
  cv::threshold(sourceMask, sourceMaskBin, 128, 255, cv::THRESH_BINARY);
  auto m = cv::moments(sourceMaskBin);
  cv::Point center(m.m01 / m.m00, m.m10 / m.m00);
  cv::seamlessClone(source, destination, sourceMask, center, outputClone,
                    cv::NORMAL_CLONE);

  cv::seamlessClone(source, destination, sourceMask, center, outputMixedClone,
                    cv::MIXED_CLONE);

  cv::imshow("outputClone", outputClone);
  cv::imshow("outputMixedClone", outputMixedClone);

  cv::waitKey();
  cv::destroyAllWindows();
  return 0;
}
