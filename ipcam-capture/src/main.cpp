#include <iostream>
#include <unistd.h>

#include "ipcam.hpp"

using namespace cv;
using namespace std;

void show_help_message(){
    cout<<"Option : \n";
    cout<<"\t-c camera address, default: rtsp://admin:kulcloud@123&&@10.1.101.1:554\n";
    cout<<"\t-u inference engine url, default: http://localhost:5000 \n";

    exit(0);
}

int main(int argc, char * argv[]){
    int c, errflag = 0;
    char * default_path = "rtsp://admin:kulcloud@123&&@10.1.101.1:554";
    char * default_server_addr = "http://localhost:5000";

    Ipcam ipcam = Ipcam(
        default_path, default_server_addr
    );

    while ( (c = getopt(argc, argv, "c:u:h")) != -1){
        switch(c) {
            case 'c':
                ipcam.ip_cam_path = optarg;
                break;
            case 'u':
                ipcam.inferece_engine_url = optarg;
                break;
            default:
                show_help_message();
        }
    }


    cout<<"Args: \n";
    cout<<"\t Sourcing from cam path: "<< ipcam.ip_cam_path <<endl;
    cout<<"\t inference engine url: " << ipcam.inferece_engine_url <<endl;

    ipcam.spinner();
    
    return 0;
}