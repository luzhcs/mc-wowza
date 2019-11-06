from aiohttp import web
from wowza.ip_camera import ip_camera_instance_list
import json

routes = web.RouteTableDef()

@routes.get('/ip_cameras')
async def ip_cameras(request):
    ret = []
    for ip_camera in ip_camera_instance_list:
        ret.append(
            {
                "ip_camera_url": ip_camera.url
            }
        )

    
    return web.json_response(ret)

app = web.Application()
app.add_routes(routes)

if __name__ == '__main__':

    

    

    web.run_app(app, port=8081)