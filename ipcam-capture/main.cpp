#include <iostream>
#include <chrono>
#include <thread>
#include <future>

// C Includes
#include <curl/curl.h>
#include <unistd.h>

#ifdef __cplusplus
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#endif

using namespace cv;
using namespace std;


int send_image(Mat frame){

    vector<uchar> buf;
    imencode(".jpg", frame, buf);
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    struct curl_slist *headerlist = NULL;
    
    curl_formadd(&formpost, &lastptr,
        CURLFORM_COPYNAME, "content-type:",
        CURLFORM_COPYCONTENTS, "multipart/form-data",
        CURLFORM_END
    );

    curl_formadd(&formpost, &lastptr,
        CURLFORM_COPYNAME, "file",
        CURLFORM_BUFFER, "sample.jpg",
        CURLFORM_BUFFERPTR, &buf[0],
        CURLFORM_BUFFERLENGTH, buf.size(),
        CURLFORM_END
    );

    CURL *curl = curl_easy_init();

    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/upload");
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        CURLcode ret = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (ret == CURLE_OK){
		    cout << "Request sent" << endl;
		    if (http_code == 200){
			    cout << "File sent and processed" << endl;
		    } else {
 		        cout << "file upload failed, check request" << endl;
		        /* always cleanup */
		        curl_easy_cleanup(curl);
            }
		}
    }

}

int main()
{
    VideoCapture cap("rtsp://admin:kulcloud@123&&@10.1.101.1:554");
    if(!cap.isOpened())
    {   
        std::cout << "Input error\n";
        return -1;
    }
    Mat frame;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds ;
    int iter = 0;
    while (true){   
        cap.read(frame);
        if (frame.empty()) {
			std::cout << "empty video.\n";
		}else {
            imshow("image", frame);
            if (iter > 60){
                iter = 0;
                start = std::chrono::system_clock::now();
                auto fut = std::async(std::launch::async, send_image, frame);
                end = std::chrono::system_clock::now();
                elapsed_seconds = end - start;
                //cout<< fut.get() << endl;
                cout<< elapsed_seconds.count() <<endl;
            }else iter++;
            
        }
        if (waitKey(25) >= 0) break;
    }
    return 0;
}