#include "src/image_predictor.hpp"


#ifdef __cplusplus
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#endif

using namespace cv;
using namespace cv::dnn;
using namespace std;


int main (int argc, char** argv){

    Mat image;  
    image = imread(argv[1], IMREAD_COLOR);
    cout<< image.total() * image.elemSize() <<endl;

    string faceProto = "opencv_face_detector.pbtxt";
    string faceModel = "opencv_face_detector_uint8.pb";

    string ageProto = "age_deploy.prototxt";
    string ageModel = "age_net.caffemodel";

    string genderProto = "gender_deploy.prototxt";
    string genderModel = "gender_net.caffemodel";

    ImagePredictor imagePredictor(faceProto, faceModel, ageProto, ageModel, genderProto, genderModel);
    imagePredictor.predict(image.rows, image.cols, image.data);

}