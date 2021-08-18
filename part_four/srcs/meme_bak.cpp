// #include <iostream>
// #include <opencv2/opencv.hpp>
// #include <dlib/opencv.h>
// #include <dlib/image_processing.h>
// #include <dlib/image_processing/frontal_face_detector.h>
// #include <fstream>
//
// void writeLandmarkToFile(dlib::full_object_detection& landmarks,
//                          const std::string& filename) {
//   std::ofstream ofs(filename);
//
//   for (int i = 0; i < landmarks.num_parts(); ++i) {
//     ofs << landmarks.part(i).x() << " " << landmarks.part(i).y() <<
//     std::endl;
//   }
// }
//
// void drawPolyline(cv::Mat& img, const dlib::full_object_detection& landmarks,
//                   const int start, const int end, bool isClosed = false) {
//   std::vector<cv::Point> points;
//   for (int i = start; i <= end; ++i) {
//     points.push_back(cv::Point(landmarks.part(i).x(),
//     landmarks.part(i).y()));
//   }
//   cv::polylines(img, points, isClosed, cv::Scalar(255, 200, 0), 2, 16);
// }
// // Draw face for the 68-point model.
// void renderFace(cv::Mat& img, const dlib::full_object_detection& landmarks) {
//   // drawPolyline(img, landmarks, 0, 16);         // Jaw line
//   // drawPolyline(img, landmarks, 17, 21);        // Left eyebrow
//   // drawPolyline(img, landmarks, 22, 26);        // Right eyebrow
//   // drawPolyline(img, landmarks, 27, 30);        // Nose bridge
//   // drawPolyline(img, landmarks, 30, 35, true);  // Lower nose
//   // drawPolyline(img, landmarks, 36, 41, true);  // Left eye
//   // drawPolyline(img, landmarks, 42, 47, true);  // Right Eye
//   drawPolyline(img, landmarks, 48, 59, true);  // Outer lip
//   drawPolyline(img, landmarks, 60, 67, true);  // Inner lip
// }
//
// void renderFace(cv::Mat& img,  // Image to draw the points on
//                 const std::vector<cv::Point2f>& points,  // Vector of points
//                 cv::Scalar color,                        // color points
//                 int radius = 3)                          // Radius of points.
// {
//   for (int i = 0; i < points.size(); i++) {
//     cv::circle(img, points[i], radius, color, -1);
//   }
// }
//
// const std::string dataPath = "data/";
// const std::string predictorPath =
//     dataPath + "models/shape_predictor_68_face_landmarks.dat";
//
// struct Lip {
//   float a;
//   float b;
//   float c;
//   friend std::ostream& operator<<(std::ostream& os, const Lip& lip) {
//     return os << lip.a << " " << lip.b << " " << lip.c;
//   }
// };
//
// bool operator<(const Lip& lhs, const Lip& rhs) {
//   return lhs.a < rhs.a && lhs.b < rhs.b && lhs.c < rhs.c;
// }
//
// int main() {
//   auto faceDetector = dlib::get_frontal_face_detector();
//
//   dlib::shape_predictor landmarkDetector;
//   dlib::deserialize(predictorPath) >> landmarkDetector;
//
//   std::string imageFilename = dataPath + "images/family.jpg";
//   cv::VideoCapture cap(1);
//   cv::Mat img;
//   bool renderMuted = false;
//   int mutedCounter = 0;
//   while (cap.isOpened()) {
//     cap >> img;
//     auto original = img.clone();
//     if (img.empty()) break;
//     // cv::Mat img = cv::imread(imageFilename);
//
//     // landmarks will be stored in results/family_0.txt
//     std::string landmarksBasename("results/family");
//
//     // convert opencv image format to dlib's image format
//     dlib::cv_image<dlib::bgr_pixel> dlibIm(img);
//
//     // detect faces in the image
//     std::vector<dlib::rectangle> faceRects = faceDetector(dlibIm);
//     // std::cout << "Number of faces detected: " << faceRects.size() <<
//     // std::endl;
//
//     std::vector<dlib::full_object_detection> landmarksAll;
//     std::vector<cv::Point2f> points;
//
//     for (int i = 0; i < faceRects.size(); ++i) {
//       // for every face rect, run landmarkDetector
//       dlib::full_object_detection landmarks =
//           landmarkDetector(dlibIm, faceRects[i]);
//       landmarksAll.push_back(landmarks);
//       //
//       points.push_back(cv::Point2f{static_cast<float>(landmarks.part(i).x()),
//       // static_cast<float>(landmarks.part(i).y())}); renderFace(img,
//       landmarks); for (int j = 0; j < landmarks.num_parts(); ++j) {
//         points.push_back(
//             cv::Point2f(static_cast<float>(landmarks.part(j).x()),
//                         static_cast<float>(landmarks.part(j).y())));
//       }
//       Lip outerUpLip{points.at(49).y, points.at(50).y, points.at(51).y};
//       // std::cout << "outer up " << outerUpLip << std::endl;
//       Lip outerDownLip{points.at(57).y, points.at(56).y, points.at(55).y};
//       // std::cout << "outer down " << outerDownLip << std::endl;
//
//       auto comp = points.at(62).x - points.at(61).x;
//       Lip innerUpLip{points.at(60).y, points.at(61).y, points.at(62).y};
//       // std::cout << "inner up " << innerUpLip << std::endl;
//       Lip innerDownLip{points.at(66).y, points.at(65).y, points.at(64).y};
//       // std::cout << "inner down " << innerDownLip << std::endl;
//
//       if (outerUpLip < innerUpLip && innerDownLip < outerDownLip) {
//         if (innerUpLip < innerDownLip) {
//           // std::cout << "OPPENED" << std::endl;
//           // std::cout << "inner up " << innerUpLip << std::endl;
//           // std::cout << "inner down " << innerDownLip << std::endl;
//           // std::cout << "INNER DIFF: " << innerDownLip.a - innerUpLip.a <<
//           " "
//           //           << innerDownLip.b - innerUpLip.b << " "
//           //           << innerDownLip.c - innerUpLip.c << std::endl;
//           static int counter = 0;
//           {
//             auto a = outerDownLip.a - outerUpLip.a;
//             auto c = outerDownLip.c - outerUpLip.c;
//             auto avg = (a + c) / 2;
//             // std::cout << " outer AVG diff " << avg << std::endl;
//             // std::cout << "COMP " << comp << std::endl;
//             if (avg > comp * 0.25) {
//               std::cout << ++counter << " : SPEAKING...." << std::endl;
//             }
//           }
//           // {
//           //   auto a = innerDownLip.a - innerUpLip.a;
//           //   auto c = innerDownLip.c - innerUpLip.c;
//           //   std::cout << " inner AVG diff " << (a + c) / 2 << std::endl;
//           //   std::cout << "COMP " << comp << std::endl;
//           // }
//
//         } else {
//           // std::cout << "CLOSED" << std::endl;
//         }
//       } else {
//         // std::cout << "DO NOT PERFORM CHECK" << std::endl;
//       }
//
//       // renderFace(img, points, cv::Scalar(0, 200, 0));
//       // renderFace(img, lips, cv::Scalar(0, 100, 0));
//       // if (a > comp) {
//       //   renderMuted = true;
//       //   mutedCounter = 5;
//       // }
//       // if (renderMuted) {
//       //   cv::putText(img, "You are muted!",
//       //               cv::Point(img.cols / 2, 3 * img.rows / 4),
//       //               cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 200));
//       //   if (--mutedCounter == 0) renderMuted = false;
//       //   std::cout << "You are muted!" << std::endl;
//       // }
//
//       // std::stringstream landmarksFilename;
//       // landmarksFilename << landmarksBasename << "_" << i << ".txt";
//       // std::cout << "Saving Landmarks to " << landmarksFilename.str() <<
//       // std::endl; writeLandmarkToFile(landmarks, landmarksFilename.str());
//       // std::string outputFilename{"results/familyLandmarks.jpg"};
//       // cv::imwrite(outputFilename, img);
//     }
//     cv::imshow("Image", img);
//     cv::imshow("Original", original);
//     cv::waitKey(10);
//   }
//
//   cv::waitKey(0);
//
//   return 0;
// }
