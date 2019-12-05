#ifndef __IPCAM_INCLUDED__
#define __IPCAM_INCLUDED__

#include <iostream>
#include <chrono>
#include <thread>
#include <future>

#include <curl/curl.h>

#ifdef __cplusplus
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#endif

using namespace cv;
using namespace std;

class Ipcam {
    public:
    char *ip_cam_path;
    char *inferece_engine_url;
    char upload_url[255];
    CURL *curl;

    Ipcam(char *ip_cam_path_input, char* inferece_engine_url_input){
        ip_cam_path = ip_cam_path_input;
        inferece_engine_url = inferece_engine_url_input;
        strcat(upload_url, this->inferece_engine_url);
        strcat(upload_url, "/upload");
        cout<< "init upload_url: " << upload_url <<"\n";
        curl = curl_easy_init();
    }

    void spinner();
    int send_image(Mat frame);

    ~Ipcam(){
        curl_easy_cleanup(curl);
    }
};

#endif