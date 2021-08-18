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

int main() {
  auto faceDetector = dlib::get_frontal_face_detector();
  auto image = cv::imread("images/family.jpg");

  // Convert OpenCV image format to Dlib's image format
  dlib::cv_image<dlib::bgr_pixel> dlibImage(image);

  std::vector<dlib::rectangle> faceRects = faceDetector(dlibImage);
  std::cout << "Number of faces detected: " << faceRects.size() << std::endl;

  dlib::shape_predictor landmarkDetector;
  const std::string predictorPath =
      "data/models/shape_predictor_68_face_landmarks.dat";
  dlib::deserialize(predictorPath) >> landmarkDetector;

  std::vector<dlib::full_object_detection> landmarksAll;

  for (const auto& face : faceRects) {
    dlib::full_object_detection landmarks = landmarkDetector(dlibImage, face);
    std::cout << "Landmarks: " << landmarks.num_parts() << std::endl;
    renderFace(image, landmarks);
    cv::rectangle(image, cv::Point(face.left(), face.top()),
                  cv::Point(face.right(), face.bottom()),
                  cv::Scalar{0, 244, 0});
  }

  cv::imshow("Image", image);
  cv::waitKey();
  cv::destroyAllWindows();
  return 0;
}

