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
                f = request.files['file']
                in_memory_file = io.BytesIO()
                f.save(in_memory_file)
                data = np.fromstring(in_memory_file.getvalue(), dtype=np.uint8)
                color_image_flag = 1
                img = cv2.imdecode(data, color_image_flag)
                ret = image_predictor.predict(img)
        except Exception as e:
                print (str(e))

        return json.dumps(ret)
		
if __name__ == '__main__':
   app.run(debug = True)