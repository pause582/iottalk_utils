import requests
import json
import time

iottalk_server = "140.114.77.93" 
iottalk_server_port = "9999"
default_input_feature = "inputdevicefeature" # 建立的 model input device feature
default_output_feature = "outputdevicefeature" # 建立的 model output device feature
default_mac = "39393939" #要註冊的
default_device_name = "default_device"

max_retry = 20

class iottalk_helper:

    device_name: str = "default_device"
    mac_addr: str = default_mac
    feature_list: list = [default_input_feature, default_output_feature]

    def __init__(self, device_name:str =default_device_name,feature_list: list = [], mac_addr: str = default_mac):
        self.device_name = device_name
        self.feature_list += feature_list
        self.mac_addr = mac_addr

    def register(self):
        params={
            "profile": {
                "d_name": self.device_name, # 註冊device名稱
                "dm_name": "testmodel",
                "u_name": "yb",
                "is_sim": False,
                "df_list": self.feature_list
            }
        }

        #print("params:" + str(params))

        request_headers = {"Content-Type": "application/json"}
        request_body = json.dumps(params)

        response = requests.post(
            "http://" + iottalk_server + ":" + iottalk_server_port + "/" + self.mac_addr, 
            headers= request_headers,
            data = request_body
        )

        #print("iottalk register response: " + str(response))
        #print("status code:" + str(response.status_code))
        return response.status_code

    def push_twin(self, data_str1: str, data_str2: str,feature: str = default_input_feature):
        data = [data_str1, data_str2] #上傳的 data，在該 input device feature 內包含兩個欄位的 data
        request_headers = {"Content-Type": "application/json"}
        
        request_body = json.dumps({"data": data})

        url = "http://" + iottalk_server + ":" + iottalk_server_port + "/" + self.mac_addr + "/" + feature
        response = requests.put(url,
            headers = request_headers,
            data = request_body
        )

        return response.status_code


    def push(self, data_str: str, feature: str = default_input_feature):
        data = [data_str]
        request_headers = {"Content-Type": "application/json"}
        
        request_body = json.dumps({"data": data})

        url = "http://" + iottalk_server + ":" + iottalk_server_port + "/" + self.mac_addr + "/" + feature
        response = requests.put(url,
            headers = request_headers,
            data = request_body
        )
        #time.sleep(0.1) # 連續拉取資料時需要 sleep 停頓迴圈
        return response.status_code

    def pull_twin(self, feature: str = default_output_feature):
        
        retry_cnt = 0
        retArr = []
        content = None
        url = "http://" + iottalk_server + ":" + iottalk_server_port + "/" + self.mac_addr + "/" + feature
        
        while True:
            response = requests.get(url)
            content = json.loads(response.text)

            if(len(content["samples"]) != 0 or retry_cnt > max_retry): break
            retry_cnt = retry_cnt + 1
            time.sleep(0.1)
        
        if (len(content["samples"]) != 0):
            data = content["samples"][0][1] # 資料拉下來會有兩筆，index 0 為較新的 data，index 1 為較舊的 data
            #data_2 = content["samples"][1][1] # 新註冊時推上去只有一筆資料，所以這邊直接跑會報錯，先註解掉
            retArr = data
            #print(data_1)
            #print(data_2)
            #time.sleep(0.1) # 連續拉取資料時需要 sleep 停頓迴圈
        
        return retArr

    def pull(self, feature: str = default_output_feature):

        retry_cnt = 0
        content = None
        retStr = "empty"
        url = "http://" + iottalk_server + ":" + iottalk_server_port + "/" + self.mac_addr + "/" + feature
        
        while True:
            response = requests.get(url)
            content = json.loads(response.text)

            if(len(content["samples"]) != 0 or retry_cnt > max_retry): break
            retry_cnt = retry_cnt + 1
            time.sleep(0.1)

        #print("iottalk pull response: " + str(response))
        #print("status code:" + str(response.status_code))

        if (len(content["samples"]) != 0):
            data = content["samples"][0][1] # 資料拉下來會有兩筆，index 0 為較新的 data，index 1 為較舊的 data
            retStr = str(data[0])
        
        return retStr

# 嘗試自己興建 device model，device feature 並 register model，push pull data
# 記得需要手動將自己 register 的 model 連上 iottalk 專案內的 model，才可以進行 data 的傳輸
# 並且需要連續的在 iottalk 上 push, pull data -> 增加迴圈
# 迴圈內的 sleep 時間會影響 data 的傳輸速度 -> push 的時間不能太快，否則資料會被洗掉
# pull 時有可能會拉到相同的資料 -> 需要增加判斷資料是否是新的
# 所以通常 pull 速度需大於 push 速度


#https://iottalk.vip/static/iottalk/01/index.html#Dumm
# iottalk 使用手冊上有範例 code 寫 dummy device 模擬傳輸 (DAN.py, DAI.py, csmapi.py)
# 主要的差別為 code 有使用 request session 的方式建立單一連線
# 可以嘗試使用 request session 建立連線後丟 request 可能會比較快速