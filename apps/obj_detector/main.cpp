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

void send_img(Mat &frame){

    std::vector <uchar> buffer;
    CURLcode ret;
    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headers=NULL;

    buffer.resize(frame.rows * frame.cols * frame.channels());
    buffer.assign((uchar*) frame.datastart, (uchar*) frame.dataend );

    cout<<"r: "<<frame.rows<<" c: "<<frame.cols<<endl;
    

    curl_global_init(CURL_GLOBAL_ALL);

    // CURLFORMcode code = curl_formadd(&formpost,
    //            &lastptr,
    //            CURLFORM_COPYNAME, "file",
    //            CURLFORM_COPYCONTENTS, "send",
    //            CURLFORM_END);

    cout<<static_cast<long>(buffer.size())<<endl;
    uchar * p = buffer.data();
    // CURLFORMcode code = curl_formadd(&formpost, &lastptr,
    //         CURLFORM_COPYNAME, "file",
    //         CURLFORM_PTRCONTENTS, buffer.data(),
    //         CURLFORM_CONTENTSLENGTH, static_cast<long>(buffer.size()),
    //         CURLFORM_END);

    cout<< buffer.size() <<endl;

    // if (code != 0) {
    //     // to avoid other problems ...
    //     fprintf(stderr, "curl_formadd() failed\n");
    //     exit(-1);
    // }

    headers = curl_slist_append(headers, "Content-Type: multipart/form-data");


    CURL *curl = curl_easy_init();
    if(curl) {
        /* set up the read callback with CURLOPT_READFUNCTION */
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/upload");
        //curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer.data());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, buffer.size());
        ret = curl_easy_perform(curl);
        if (ret != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(ret));
        curl_easy_cleanup(curl);
    }
}

int main()
{
    VideoCapture cap("rtsp://admin:Kulcloud&&@10.1.100.35:554");
    if(!cap.isOpened())
    {   
        std::cout << "Input error\n";
        return -1;
    }
    Mat frame;

    int iter = 0;
    std::chrono::seconds interval(1);
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    while (true){   
        cap.read(frame);
        if (frame.empty()) {
			std::cout << "빈 영상이 캡쳐되었습니다.\n";
		}else {
            //imshow("image", frame);
            
            std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
            std::chrono::duration<double> sec = end - start;
            if (sec > interval) {
                send_img(frame);
                start = std::chrono::system_clock::now();
            }
        }
        if (waitKey(25) >= 0) break;
    }
    return 0;
}