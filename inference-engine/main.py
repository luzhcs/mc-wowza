


from handler import image_handler

from aiohttp import web

app = web.Application()

ih = image_handler.ImageHandler()

app.router.add_post('/predict', ih.image_handle )
	
if __name__ == '__main__':
    web.run_app(app, host="0.0.0.0", port=5001)