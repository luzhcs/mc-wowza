#include "image_predictor.hpp"

void ImagePredictor::predict(int rows, int cols, unsigned char *buffer){
    cout<<"image processing start"<<endl;
    Mat frame ( rows, cols, CV_8UC3, buffer);
    cout<< "frame dims : ("<< frame.rows<<","<< frame.cols <<")"<<endl;

    //cout<< frame <<endl;

    vector<vector<int>> bboxes;
    Mat frameFace;

    Mat frameOpenCVDNN = frame.clone();
    int frameHeight = frameOpenCVDNN.rows;
    int frameWidth = frameOpenCVDNN.cols;

    double inScaleFactor = 1.0;
    Size size = Size(300, 300);
    Scalar meanVal = Scalar(104, 117, 123);

    Mat inputBlob;
    inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, size, meanVal, true, false);
    cout<<"input Blob : " << inputBlob.dims <<endl;
    faceNet.setInput(inputBlob, "data");
    Mat detection = faceNet.forward("detection_out");
    Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
    for(int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);
        if(confidence > 0.5)
        {
            int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
            int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
            int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
            int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);
            vector<int> box = {x1, y1, x2, y2};
            bboxes.push_back(box);
            rectangle(frameOpenCVDNN, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0),2, 4);
        }
    }
    cout<<"done? "<<endl;
    tie(frameFace, bboxes) = make_tuple(frameOpenCVDNN, bboxes);
    if(bboxes.size() == 0) {
        cout << "No face detected, checking next frame." << endl;
    } else {
        cout << "Face detected!" << endl;
        for (auto it = begin(bboxes); it != end(bboxes); ++it) {
            Rect rec(it->at(0) - padding, it->at(1) - padding, it->at(2) - it->at(0) + 2*padding, it->at(3) - it->at(1) + 2*padding);
            Mat face = frame(rec); // take the ROI of box on the frame

            Mat blob;
            blob = blobFromImage(face, 1, Size(227, 227), MODEL_MEAN_VALUES, false);
            genderNet.setInput(blob);
            vector<float> genderPreds = genderNet.forward();
            int max_index_gender = std::distance(genderPreds.begin(), max_element(genderPreds.begin(), genderPreds.end()));
            string gender = genderList[max_index_gender];
            cout << "Gender: " << gender << endl;
            ageNet.setInput(blob);
            vector<float> agePreds = ageNet.forward();
            int max_indice_age = std::distance(agePreds.begin(), max_element(agePreds.begin(), agePreds.end()));
            string age = ageList[max_indice_age];
            cout << "Age: " << age << endl;
            string label = gender + ", " + age; // label
            cv::putText(frameFace, label, Point(it->at(0), it->at(1) -15), cv::FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0, 255, 255), 2, cv::LINE_AA);
            imwrite("out.jpg",frameFace);
        }
        
    }
}
tuple<Mat, vector<vector<int>>> ImagePredictor::getFaceBox(Net net, Mat &frame, double conf_threshold)
{
    Mat frameOpenCVDNN = frame.clone();
    cout<<"clone done" <<endl;
    int frameHeight = frameOpenCVDNN.rows;
    int frameWidth = frameOpenCVDNN.cols;
    double inScaleFactor = 1.0;
    Size size = Size(300, 300);
    // std::vector<int> meanVal = {104, 117, 123};
    Scalar meanVal = Scalar(104, 117, 123);

    Mat inputBlob;
    inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, size, meanVal, true, false);
    cout<<"input Blob : " << inputBlob.dims <<endl;
    net.setInput(inputBlob, "data");
    Mat detection = net.forward("detection_out");

    Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    vector<vector<int>> bboxes;

    for(int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if(confidence > conf_threshold)
        {
            int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
            int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
            int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
            int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);
            vector<int> box = {x1, y1, x2, y2};
            bboxes.push_back(box);
            rectangle(frameOpenCVDNN, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0),2, 4);
        }
    }

    return make_tuple(frameOpenCVDNN, bboxes);
}



ImagePredictor* ImagePredictor_new(
    const char * faceProto,
    const char * faceModel,
    const char * ageProto,
    const char * ageModel,
    const char * genderProto,
    const char * genderModel
    )
{
    return new ImagePredictor(
            faceProto,
            faceModel,
            ageProto,
            ageModel,
            genderProto,
            genderModel
        );
}

void ImagePredictor_predict(ImagePredictor *ip,int rows, int cols, unsigned char *buffer)
{
    ip->predict(rows, cols, buffer);
}
