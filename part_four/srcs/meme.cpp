#include <iostream>
#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

void renderLandmarks(cv::Mat& image, dlib::full_object_detection& landmarks) {
  for (int i = 0; i < landmarks.num_parts(); ++i) {
    cv::circle(image, cv::Point(landmarks.part(i).x(), landmarks.part(i).y()),
               3, cv::Scalar(0, 200, 0), -1);
  }
}

void drawPolylines(cv::Mat& image, dlib::full_object_detection& landmarks,
                   int start, int end, bool closed = false) {
  std::vector<cv::Point> points;
  for (int i = start; i <= end; ++i) {
    points.push_back(cv::Point(landmarks.part(i).x(), landmarks.part(i).y()));
  }
  cv::polylines(image, points, closed, cv::Scalar{255, 200, 0}, 2, cv::LINE_AA);
}

void renderFace(cv::Mat& image, dlib::full_object_detection& landmarks) {
  drawPolylines(image, landmarks, 0, 16);         // Jaw lines
  drawPolylines(image, landmarks, 17, 21);        // Left eyebrow
  drawPolylines(image, landmarks, 22, 26);        // Right eyebrow
  drawPolylines(image, landmarks, 27, 30);        // Nose
  drawPolylines(image, landmarks, 30, 35, true);  // Lower Nose
  drawPolylines(image, landmarks, 36, 41, true);  // Left eye
  drawPolylines(image, landmarks, 42, 47, true);  // Right eye
  drawPolylines(image, landmarks, 48, 59, true);  // Outer lip
  drawPolylines(image, landmarks, 60, 67, true);  // Inner lip
}

dlib::cv_image<dlib::bgr_pixel> dlibImage;
int calls = 0;
std::vector<dlib::rectangle> faces;

void thugLife(cv::Mat& image, cv::Mat& glasses, cv::Mat& cigar,
              dlib::frontal_face_detector& faceDetector) {
  cv::resize(image, image, cv::Size(), 0.5, 0.5);

  if (calls % 10 == 0) {
    dlibImage = dlib::cv_image<dlib::bgr_pixel>{image};
    faces = faceDetector(dlibImage);
    std::cout << "Detect: " << calls << std::endl;
  } else {
    std::cout << "Skip: " << calls << std::endl;
  }
  ++calls;

  const std::string predictorPath =
      "data/models/shape_predictor_68_face_landmarks.dat";
  dlib::shape_predictor landmarkDetector;
  dlib::deserialize(predictorPath) >> landmarkDetector;

  for (const auto& face : faces) {
    dlib::full_object_detection landmarks = landmarkDetector(dlibImage, face);
    auto diff = landmarks.part(16).x() - landmarks.part(0).x();
    double p = static_cast<double>(diff) / glasses.cols;
    cv::resize(glasses, glasses, cv::Size(diff, glasses.rows * p));
    cv::Rect r(landmarks.part(0).x(), landmarks.part(36).y() - glasses.rows / 2,
               glasses.cols, glasses.rows);
    cv::Mat roi = image(r);
    // std::cout << roi.size() << std::endl;
    // std::cout << glasses.size() << std::endl;
    // cv::imshow("roi", roi);

    cv::Mat channels[3];
    cv::Mat channelsGlasses[4];
    cv::split(roi, channels);
    cv::split(glasses, channelsGlasses);
    cv::Mat mask;
    cv::bitwise_not(channelsGlasses[3], mask);
    // cv::imshow("mask", mask);

    cv::bitwise_and(channels[0], mask, channels[0]);
    cv::add(channels[0], channelsGlasses[0], channels[0]);
    cv::bitwise_and(channels[1], mask, channels[1]);
    cv::add(channels[1], channelsGlasses[1], channels[1]);
    cv::bitwise_and(channels[2], mask, channels[2]);
    cv::add(channels[2], channelsGlasses[2], channels[2]);

    cv::Mat maskedEyes;
    cv::merge(channels, 3, maskedEyes);
    maskedEyes.copyTo(roi);

    auto mDiff = landmarks.part(54).x() - landmarks.part(48).x();
    double mp = static_cast<double>(mDiff) / cigar.cols;
    cv::resize(cigar, cigar, cv::Size(mDiff, cigar.rows * mp));
    cv::Rect rCigar(landmarks.part(66).x() - cigar.size().width,
                    landmarks.part(66).y(), cigar.cols, cigar.rows);
    cv::Mat cigarMask;
    cv::Mat channelsCigarRoi[3];
    cv::Mat channelsCigar[4];
    auto cigarRoi = image(rCigar);

    cv::split(cigarRoi, channelsCigarRoi);
    cv::split(cigar, channelsCigar);

    cv::bitwise_not(channelsCigar[3], cigarMask);

    cv::bitwise_and(channelsCigarRoi[0], cigarMask, channelsCigarRoi[0]);
    cv::add(channelsCigarRoi[0], channelsCigar[0], channelsCigarRoi[0]);
    cv::bitwise_and(channelsCigarRoi[1], cigarMask, channelsCigarRoi[1]);
    cv::add(channelsCigarRoi[1], channelsCigar[1], channelsCigarRoi[1]);
    cv::bitwise_and(channelsCigarRoi[2], cigarMask, channelsCigarRoi[2]);
    cv::add(channelsCigarRoi[2], channelsCigar[2], channelsCigarRoi[2]);

    cv::Mat maskedCigar;
    cv::merge(channelsCigarRoi, 3, maskedCigar);
    maskedCigar.copyTo(cigarRoi);
  }

  cv::imshow("Image", image);
}

int main(int argc, char** argv) {
  auto glasses = cv::imread("images/tr.png", cv::IMREAD_UNCHANGED);
  auto cigar = cv::imread("images/cigar.png", cv::IMREAD_UNCHANGED);

  auto faceDetector = dlib::get_frontal_face_detector();

  cv::VideoCapture cap{0};
  cv::Mat frame;
  // cv::Mat image;
  std::vector<dlib::rectangle> faces;
  dlib::full_object_detection landmarks;
  int calls = 0;
  const std::string predictorPath =
      "data/models/shape_predictor_68_face_landmarks.dat";
  dlib::shape_predictor landmarkDetector;
  dlib::deserialize(predictorPath) >> landmarkDetector;
  while (cap.isOpened()) {
    cap >> frame;
    auto& image = frame;
    if (frame.empty()) break;
    if (calls % 2 == 0) {
      // cv::resize(frame, image, cv::Size(), 0.5, 0.5);
      dlib::cv_image<dlib::bgr_pixel> dImage{image};
      faces = faceDetector(dImage);
      std::cout << faces.size() << std::endl;
      for (const auto& face : faces) {
        landmarks = landmarkDetector(dlibImage, face);
      }
    } else {
      std::cout << faces.size() << " Skip" << std::endl;
    }
    renderLandmarks(image, landmarks);

    ++calls;
    cv::waitKey(20);
    // cv::imshow("Frame", frame);
    cv::imshow("Image", image);
  }

  // cv::imshow("glasses", glasses);
  // cv::imshow("cigar", cigar);
  cv::waitKey();
  cv::destroyAllWindows();

  return 0;
}
