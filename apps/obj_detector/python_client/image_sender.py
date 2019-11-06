from __future__ import print_function
import requests
import json
import cv2
import time
import aiohttp
import asyncio
from aiohttp import formdata

async def sending_img(session, url, data):
    async with session.post(url, data=data) as response:
        return await response.text()

async def main():
    addr = 'http://localhost:5000'
    test_url = addr + '/upload'
    cap = cv2.VideoCapture("rtsp://admin:Kulcloud&&@10.1.100.35:554")
    cap.set(3,640)
    cap.set(4,480)
    async with aiohttp.ClientSession() as session:
        start = time.time()
        while cv2.waitKey(1) < 0:
            # Read frame
            end = time.time ()
            if end - start > 1:
                start = end
                hasFrame, frame = cap.read()
                if not hasFrame:
                    cv2.waitKey()
                else:
                    cv2.imshow("test", frame)
                    frame = cv2.imencode(".jpg", frame)
                    data = formdata.FormData()
                    data.add_field('file',
                        frame[1].tostring(),
                        filename='tmp.jpg',
                        content_type='image/jpeg'
                    )

                    ret = await sending_img(session, test_url, data)
                    print(ret)


if __name__ == '__main__':
    loop = asyncio.get_event_loop()
    loop.run_until_complete(main())
