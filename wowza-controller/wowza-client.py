import requests
import json

url = '/v2/servers'

class WowzaLiveStream:
    def __init__(self, base_url):
        self.base_url = base_url
        self.headers = {
            'Accept': 'application/json'
        }

    def info(self):
        try:
            path = self.base_url + url
            response = requests.get(path, headers=self.headers)
        except:
            print("info error")
            return None
        return response.content



if __name__ == "__main__":
    wls = WowzaLiveStream('http://localhost:8087')
    print(wls.info())

    