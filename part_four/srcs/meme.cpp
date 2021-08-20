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

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Please provide input image" << std::endl;
    return -1;
  }
  auto image = cv::imread(argv[1]);

  auto faceDetector = dlib::get_frontal_face_detector();
  dlib::cv_image<dlib::bgr_pixel> dlibImage{image};
  std::vector<dlib::rectangle> faces = faceDetector(dlibImage);
  std::cout << "Number of faces detected: " << faces.size() << std::endl;

  const std::string predictorPath =
      "data/models/shape_predictor_68_face_landmarks.dat";
  dlib::shape_predictor landmarkDetector;
  dlib::deserialize(predictorPath) >> landmarkDetector;

  for (const auto& face : faces) {
    dlib::full_object_detection landmarks = landmarkDetector(dlibImage, face);
    std::cout << landmarks.num_parts() << std::endl;
    cv::rectangle(image, cv::Point(face.left(), face.top()),
                  cv::Point(face.right(), face.bottom()),
                  cv::Scalar(0, 255, 0));
    // renderLandmarks(image, landmarks);
    renderFace(image, landmarks);
  }

  cv::imshow("Image", image);
  cv::waitKey();
  cv::destroyAllWindows();

  return 0;
}
