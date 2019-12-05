import numpy as np
import cv2
import io
import json

from flask import Flask, render_template, request
from werkzeug import secure_filename
from handler import ImagePredictor


app = Flask(__name__)
image_predictor = ImagePredictor.ImagePredictor()
	
@app.route('/upload', methods = ['POST'])
def upload_file():
   if request.method == 'POST':
        ret = None
        try:
                print (request.files['file'])
                print ("read file from requests")
                img_str = request.files['file'].read()
                print ("file to string buf")
                data = np.fromstring(img_str, dtype=np.uint8)
                print ("decode string buf to IMAGE COLOR FORMAT")
                img = cv2.imdecode(data, cv2.IMREAD_COLOR)
                #print ("img: {}".format(img))
                ret = image_predictor.predict(img)
        except Exception as e:
                print (str(e))

        return json.dumps(ret)
		
if __name__ == '__main__':
   app.run(debug = True)