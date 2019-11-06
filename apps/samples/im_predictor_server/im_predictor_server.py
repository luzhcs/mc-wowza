from ctypes import *
import cv2

from sys import getsizeof

class ImagePredictor(object):
 
    def __init__( self ):
        self.lib = cdll.LoadLibrary('../build/lib/libimagePredictor.dylib')
        self.face_detector = create_string_buffer(b"opencv_face_detector.pbtxt")
        self.face_model = create_string_buffer(b"opencv_face_detector_uint8.pb")
        self.age_proto = create_string_buffer(b"age_deploy.prototxt")
        self.age_model = create_string_buffer(b"age_net.caffemodel")
        self.gender_proto = create_string_buffer(b"gender_deploy.prototxt")
        self.gender_model = create_string_buffer(b"gender_net.caffemodel")
        self.obj = self.lib.ImagePredictor_new(
            self.face_detector,
            self.face_model,
            self.age_proto,
            self.age_model,
            self.gender_proto,
            self.gender_model
        )

    def get_image(self):
        self.img = cv2.imread('sample1.jpg', cv2.IMREAD_COLOR)
        print (self.img.shape[0], self.img.shape[1], self.img.data)
        self.rows = c_int(self.img.shape[0])
        self.cols = c_int(self.img.shape[1])
        self.image_bytes = (c_ubyte * len(self.img.tobytes())).from_buffer_copy(self.img.tobytes())

    def predict(self):
        self.lib.ImagePredictor_predict(self.obj, self.rows, self.cols, byref(self.image_bytes))

    

ip = ImagePredictor()
ip.get_image()
ip.predict()