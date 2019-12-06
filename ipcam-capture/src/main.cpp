#include <iostream>
#include <unistd.h>
#include <thread>

#include "ipcam.hpp"

using namespace cv;
using namespace std;

void show_help_message(){
    cout<<"Option : \n";
    cout<<"\t-c camera address, default: rtsp://admin:kulcloud@123&&@10.1.101.1:554\n";
    cout<<"\t-u inference engine url, default: http://localhost:5000 \n";

    exit(0);
}

void my_handler(sig_t s){
    cout<< "Caught signal: "<<s<<"\n";
    exit(1); 
}

int main(int argc, char * argv[]){
    int c, errflag = 0;
    char * default_path = "rtsp://admin:kulcloud@123&&@10.1.101.1:554";
    char * default_server_addr = "http://localhost:5000";

    

    while ( (c = getopt(argc, argv, "c:u:h")) != -1){
        switch(c) {
            case 'c':
                default_path = optarg;
                break;
            case 'u':
                default_server_addr = optarg;
                break;
            default:
                show_help_message();
        }
    }
    cout<<"Args: \n";
    cout<<"\t Sourcing from cam path: "<< default_path <<endl;
    cout<<"\t inference engine url: " << default_server_addr <<endl;
    
    Ipcam ipcam = Ipcam(
        default_path, default_server_addr
    );
    ipcam.spinner();
    return 0;
}