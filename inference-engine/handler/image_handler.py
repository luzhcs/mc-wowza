import numpy as np
import cv2
import io
import json

from aiohttp import web
from aiohttp import MultipartReader
from ml import opencvdnn

ofd = opencvdnn.OpencvFaceDetector()

class ImageHandler:
    def __init__(self):
        pass

    def json_error(self, message):
        return web.Response(
            body=json.dumps({'error': message}).encode('utf-8'),
            content_type='application/json')

    async def image_handle(self, request):
        if request.method == 'POST':
            ret = {}
            try:
                reader = MultipartReader.from_response(request)
                while True:
                    part = await reader.next()
                    if part is None:
                        break
                    data = await part.read()
                    data = np.asarray(data, dtype=np.uint8)
                    img = cv2.imdecode(data, cv2.IMREAD_COLOR)
                    ret = ofd.predict(img)
            except web.HTTPException as e:
                return self.json_error(e.reason)

            return web.json_response(ret)
		