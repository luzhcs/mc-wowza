#include "ipcam.hpp"

void Ipcam::spinner(){
    VideoCapture cap(this->ip_cam_path);
    if(!cap.isOpened())
    {   
        std::cout << "Input error\n";
        return;
    }
    Mat frame;
    
    int iter = 0;
    while (true){
        if (cap.isOpened()){
            try {
                cap.read(frame);
                if (frame.empty()) {
                    VideoCapture cap(this->ip_cam_path);
                    std::cout << "Empty video\n";
                }else {
                    if (iter > 60){
                        iter = 0;
                        auto fut = std::async(std::launch::async, &Ipcam::send_image, this, frame);
                    }else iter++;    
                }
            }catch (Exception ex){
                cout<<ex.msg<<endl;
            }
        }
        if (waitKey(25) >= 0) break;
    }
}

int Ipcam::send_image(Mat frame){

    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds ;

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

        CURLcode ret = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (ret == CURLE_OK)
            (http_code == 200) ? cout << "\nFile sent and processed" << endl : cout << "\nFile uploading is failed, please check request" << endl;
        
    }
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    cout<< "elpased time: "<< setw(10) << elapsed_seconds.count() <<endl;

}