import time

class IpCamera:
    def __init__(self, camera_ip_address, port, source_path):
        # common: ip camera url format
        # rtsp://192.168.0.4:1935/myapplication/test.mp4
        # rtsp://[camera-ip-address]:554/axis-media/media.amp
        # rtsp://[camera-ip-address]:[port]/[source_path]
        self.url = "rtsp://" + camera_ip_address + ":" + str(port) + "/" + source_path
        self.running = False
        self.interval = 1

    def spinning(self):
        while self.running:
            self.get_frame()
            time.sleep(self.interval)
            

    def get_frame(self):
        print (self.url)
        pass

ip_camera_instance_list = []
ip_camera_instance_list.append(IpCamera("localhost", 535, "test.amp"))