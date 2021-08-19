#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

void drawPolyline(cv::Mat& img, const dlib::full_object_detection& landmarks,
                  const int start, const int end, bool isClosed = false) {
  std::vector<cv::Point> points;
  for (int i = start; i <= end; ++i) {
    points.push_back(cv::Point(landmarks.part(i).x(), landmarks.part(i).y()));
  }
  cv::polylines(img, points, isClosed, cv::Scalar(255, 200, 0), 2, 16);
}

// Draw face for the 68-point model.
void renderFace(cv::Mat& img, const dlib::full_object_detection& landmarks) {
  drawPolyline(img, landmarks, 0, 16);         // Jaw line
  drawPolyline(img, landmarks, 17, 21);        // Left eyebrow
  drawPolyline(img, landmarks, 22, 26);        // Right eyebrow
  drawPolyline(img, landmarks, 27, 30);        // Nose bridge
  drawPolyline(img, landmarks, 30, 35, true);  // Lower nose
  drawPolyline(img, landmarks, 36, 41, true);  // Left eye
  drawPolyline(img, landmarks, 42, 47, true);  // Right Eye
  drawPolyline(img, landmarks, 48, 59, true);  // Outer lip
  drawPolyline(img, landmarks, 60, 67, true);  // Inner lip
}

void foo(cv::Mat& image, cv::Mat& glasses, cv::Mat& cigar,
         dlib::frontal_face_detector& faceDetector) {
  // Convert OpenCV image format to Dlib's image format
  dlib::cv_image<dlib::bgr_pixel> dlibImage(image);

  std::vector<dlib::rectangle> faceRects = faceDetector(dlibImage);
  std::cout << "Number of faces detected: " << faceRects.size() << std::endl;

  dlib::shape_predictor landmarkDetector;
  const std::string predictorPath =
      "data/models/shape_predictor_68_face_landmarks.dat";
  dlib::deserialize(predictorPath) >> landmarkDetector;

  for (const auto& face : faceRects) {
    dlib::full_object_detection landmarks = landmarkDetector(dlibImage, face);
    auto diff = landmarks.part(16).x() - landmarks.part(0).x();
    double p = double(diff) / glasses.cols;
    cv::resize(glasses, glasses, cv::Size(diff, glasses.rows * p));
    cv::Rect r(landmarks.part(0).x(), landmarks.part(36).y() - glasses.rows / 2,
               diff, glasses.rows);
    cv::Mat roi = image(r);

    auto mDiff = landmarks.part(54).x() - landmarks.part(48).x();
    auto mp = double(mDiff) / cigar.rows;
    cv::resize(cigar, cigar, cv::Size(mDiff, cigar.rows * mp));
    cv::Rect rCigar(landmarks.part(66).x() - cigar.size().width,
                    landmarks.part(66).y(), mDiff, cigar.rows);
    auto cigarRoi = image(rCigar);

    cv::Mat channels[3];
    cv::Mat channelsGlasses[4];
    cv::Mat channelsCigar[4];
    cv::Mat channelsCigarRoi[3];
    cv::split(roi, channels);
    cv::split(glasses, channelsGlasses);
    cv::split(cigar, channelsCigar);
    cv::split(cigarRoi, channelsCigarRoi);

    // cv::circle(image, cv::Point(landmarks.part(48).x(),
    // landmarks.part(48).y()),
    //            5, cv::Scalar{0, 222, 0}, -1);
    // cv::circle(image, cv::Point(landmarks.part(54).x(),
    // landmarks.part(54).y()),
    //            5, cv::Scalar{0, 222, 0}, -1);
    // cv::circle(image, cv::Point(landmarks.part(66).x(),
    // landmarks.part(66).y()),
    //            5, cv::Scalar{0, 222, 0}, -1);

    cv::Mat mask, cigarMask;
    cv::bitwise_not(channelsGlasses[3], mask);

    cv::bitwise_not(channelsCigar[3], cigarMask);

    cv::bitwise_and(channels[0], mask, channels[0]);
    cv::add(channels[0], channelsGlasses[0], channels[0]);
    cv::bitwise_and(channels[1], mask, channels[1]);
    cv::add(channels[1], channelsGlasses[0], channels[1]);
    cv::bitwise_and(channels[2], mask, channels[2]);
    cv::add(channels[2], channelsGlasses[0], channels[2]);

    cv::Mat maskedEyes;
    cv::merge(channels, 3, maskedEyes);

    cv::bitwise_and(channelsCigarRoi[0], cigarMask, channelsCigarRoi[0]);
    cv::add(channelsCigarRoi[0], channelsCigar[0], channelsCigarRoi[0]);
    cv::bitwise_and(channelsCigarRoi[1], cigarMask, channelsCigarRoi[1]);
    cv::add(channelsCigarRoi[1], channelsCigar[1], channelsCigarRoi[1]);
    cv::bitwise_and(channelsCigarRoi[2], cigarMask, channelsCigarRoi[2]);
    cv::add(channelsCigarRoi[2], channelsCigar[2], channelsCigarRoi[2]);

    cv::Mat cigarFinal;
    cv::merge(channelsCigarRoi, 3, cigarFinal);
    cigarFinal.copyTo(cigarRoi);
    maskedEyes.copyTo(roi);
    cv::imshow("image", image);
  }
}

int main() {
  dlib::frontal_face_detector faceDetector = dlib::get_frontal_face_detector();
  // auto image = cv::imread("images/musk.jpg");
  auto glasses = cv::imread("images/tr.png", cv::IMREAD_UNCHANGED);
  auto cigar = cv::imread("images/cigar.png", cv::IMREAD_UNCHANGED);

  cv::VideoCapture cap(0);
  cv::Mat frame;
  while (cap.isOpened()) {
    cap >> frame;
    if (frame.empty()) break;
    cv::imshow("frame", frame);
    auto key = cv::waitKey(25);

    if (key == int('m')) {
      auto image = frame.clone();
      foo(image, glasses, cigar, faceDetector);
    }
  }

  cv::waitKey();
  cv::destroyAllWindows();
  return 0;
}

