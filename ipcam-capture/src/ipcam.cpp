#include "ipcam.hpp"



void Ipcam::print_fps(){
    cout<<Ipcam::fps<<endl;
}

void Ipcam::spinner(){
    Mat frame;
    int iter = 0;
    VideoCapture cap(this->ip_cam_path, CAP_FFMPEG);
    if (cap.isOpened()) cout<< "Opening RTSP stream ..."<<endl;
    else {
        cout << "Input error\n";
        return;
    }
    std::chrono::time_point<std::chrono::system_clock> start, end, interval_start;
    std::chrono::duration<double> elapsed_seconds;

    interval_start = std::chrono::system_clock::now();
    while (true){
        if ((end - interval_start).count() > 1000000 ) {
            interval_start = std::chrono::system_clock::now();
            print_fps();
        }
        start = std::chrono::system_clock::now();
        try {
            if (cap.isOpened()){                
                cap.read(frame);
                if (frame.empty()){
                    cout<<"Empty frame.\n";
                }else{
                    imshow("test_frame", frame);
                    if (iter > 60){
                        iter = 0;
                        //auto fut = std::async(std::launch::async, &Ipcam::send_image, this, frame);
                        std::thread(&Ipcam::send_image, this, frame).detach();
                    }
                    iter++;
                }
            }
            if (waitKey(25) >= 0) break;
        }
        catch (Exception ex){
            cout<<"Errors on capturing : "<<ex.msg<<endl;
            Ipcam::re_open_cap(cap);
        }
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        Ipcam::fps = 1/elapsed_seconds.count();
    }
}

int Ipcam::re_open_cap(VideoCapture &cap){
    cout<<"re start Capturing ..."<<endl;
    cap.open(this->ip_cam_path, CAP_FFMPEG);
}

void Ipcam::send_image(Mat frame){
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;

    start = std::chrono::system_clock::now();

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
    
    if (curl){
        curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, upload_url);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
        cout<<upload_url<<endl;
        try{
            CURLcode ret = curl_easy_perform(curl);
            long http_code = 0;
            curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (ret == CURLE_OK)
                (http_code == 200) ? cout << "\nFile sent and processed" << endl : cout << "\nFile uploading is failed, please check request" << endl;
        } catch (Exception ex) {
            cout<<ex.what()<<endl;
        }
    }
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    cout<< "elpased time: "<< setw(10) << elapsed_seconds.count() <<endl;
}