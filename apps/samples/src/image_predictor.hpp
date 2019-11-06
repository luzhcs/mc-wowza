#include <tuple>
#include <iostream>

#ifdef __cplusplus
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#endif

using namespace cv;
using namespace cv::dnn;
using namespace std;

    class ImagePredictor{
        public:
            Scalar MODEL_MEAN_VALUES = Scalar(78.4263377603, 87.7689143744, 114.895847746);
            vector<string> ageList = {"(0-2)", "(4-6)", "(8-12)", "(15-20)", "(25-32)",
                "(38-43)", "(48-53)", "(60-100)"};

            vector<string> genderList = {"Male", "Female"};
            int padding = 20;

            Net ageNet, genderNet, faceNet;
            VideoCapture cap;

            explicit ImagePredictor(
                string faceProto,
                string faceModel,
                string ageProto,
                string ageModel,
                string genderProto,
                string genderModel
            ){
                ageNet = readNet(ageModel, ageProto);
                genderNet = readNet(genderModel, genderProto);
                faceNet = readNet(faceModel, faceProto);
            }
            void predict(int rows, int cols, unsigned char *buffer);
        private:
            tuple<Mat, vector<vector<int>>> getFaceBox(Net net, Mat &frame, double conf_threshold);
            
    };


extern "C" {
    ImagePredictor* ImagePredictor_new(
    const char * faceProto,
    const char * faceModel,
    const char * ageProto,
    const char * ageModel,
    const char * genderProto,
    const char * genderModel
    );
    void ImagePredictor_predict(ImagePredictor *ip, int rows, int cols, unsigned char *buffer);
}
