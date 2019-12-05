import pytest
from handler.ImagePredictor import ImagePredictor

class TestImagePredictor:
    def test_read_image(self, capsys):
        print ("test")
        self.ip = ImagePredictor()
        self.ip.read_image('rtsp://admin:Kulcloud&&@10.1.100.35:554')
        #self.ip.read_image(0)
        print (self.ip.predict())
        assert self.ip.frame is not None
