#include <opencv2/opencv.hpp>

int main() {
  auto book = cv::imread("images/book.jpg");
  auto frame = cv::imread("images/me.jpg");
  cv::Mat bookGray, frameGray;
  cv::cvtColor(book, bookGray, cv::COLOR_BGR2GRAY);
  cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);

  auto orb = cv::ORB::create(1000);
  std::vector<cv::KeyPoint> keypoints1, keypoints2;
  cv::Mat descriptors1, descriptors2;

  orb->detectAndCompute(bookGray, cv::Mat(), keypoints1, descriptors1);
  descriptors1.convertTo(descriptors1, CV_32F);

  orb->detectAndCompute(frameGray, cv::Mat(), keypoints2, descriptors2);
  descriptors2.convertTo(descriptors2, CV_32F);

  cv::FlannBasedMatcher matcher{new cv::flann::KDTreeIndexParams{5},
                                new cv::flann::SearchParams{50}};
  std::vector<std::vector<cv::DMatch>> matches;

  matcher.knnMatch(descriptors1, descriptors2, matches, 2);
  const float ratio = 0.9f;
  std::vector<cv::DMatch> good;
  for (size_t i = 0; i < matches.size(); ++i) {
    if (matches[i][0].distance < ratio * matches[i][1].distance) {
      good.push_back(matches[i][0]);
    }
  }

  cv::Mat result;
  cv::drawMatches(bookGray, keypoints1, frameGray, keypoints2, matches, result);

  std::vector<cv::Point2f> srcPoints;
  std::vector<cv::Point2f> dstPoints;
  for (size_t i = 0; i < good.size(); ++i) {
    srcPoints.push_back(keypoints1[good[i].queryIdx].pt);
    dstPoints.push_back(keypoints2[good[i].trainIdx].pt);
  }

  if (good.size() > 10) {
    auto H = cv::findHomography(srcPoints, dstPoints, cv::RANSAC, 5.0);

    std::vector<cv::Point2f> bookCorners(4);
    bookCorners[0] = cv::Point2f{0, 0};
    bookCorners[1] = cv::Point2f(0, book.size().height);
    bookCorners[2] = cv::Point2f(book.size().width, book.size().height);
    bookCorners[3] = cv::Point2f(book.size().width, 0);
    std::vector<cv::Point2f> sceneCorners(4);
    cv::perspectiveTransform(bookCorners, sceneCorners, H);

    std::vector<cv::Point2i> points;
    for (const auto& p : sceneCorners) {
      points.push_back(cv::Point2i{int(p.x), int(p.y)});
    }

    cv::polylines(frame, points, true, cv::Scalar(255, 0, 0), 3);
  }

  cv::imshow("book", book);
  cv::imshow("frame", frame);
  cv::imshow("result", result);

  cv::waitKey();
  cv::destroyAllWindows();
  return 0;
}
