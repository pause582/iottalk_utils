#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include "http_helper.h"

using namespace std;
using namespace nlohmann;

string iottalk_server = "140.114.77.93" ;
string iottalk_server_port = "9999";
string iottalk_input_device_feature = "inputdevicefeature"; // 建立的 model input device feature
string iottalk_output_device_feature = "outputdevicefeature"; // 建立的 model output device feature
string iottalk_mac = "39393939"; //要註冊的

void Register(){
    vector<string> device_feature_list = {iottalk_input_device_feature, iottalk_output_device_feature};
    json params = {{
        "profile", {
            {"d_name", "hatsunedevice"}, // 註冊device名稱
            {"dm_name", "testmodel"},
            {"u_name", "yb"},
            {"is_sim", false},
            {"df_list", device_feature_list}
        }
    }};

    cout << "params: " << params << endl;

    vector<string> request_headers = {"Content-Type: application/json"};
    string request_body = params.dump();

    string url = "http://" + iottalk_server + ":" + iottalk_server_port + "/" + iottalk_mac;
    string response = requests::post(url,request_headers,request_body);
    cout << "iottalk register response: " << response << endl;
}

void iottalk_push(string& str1,string str2 = "miku"){
    vector<string> data = {str1, str2}; //上傳的 data，在該 input device feature 內包含兩個欄位的 data
    vector<string> request_headers = {"Content-Type: application/json"};
    
    string request_body = json({{"data", data}}).dump();
    //cout << request_body << endl;

    string url = "http://" + iottalk_server + ":" + iottalk_server_port + "/" + iottalk_mac + "/" + iottalk_input_device_feature;
    string response = requests::put(url,request_headers,request_body);
    
    cout << "iottalk push response: \n" << response << endl;
    //this_thread::sleep_for(chrono::milliseconds(100)); 
    // 連續傳輸時需要 sleep 停頓迴圈  
}

string iottalk_pull(){
    string url = "http://" + iottalk_server + ":" + iottalk_server_port + "/" + iottalk_mac + "/" + iottalk_output_device_feature;
    string response = requests::get(url);
    json content;
    string retStr = "empty";
    cout << "iottalk pull response: \n" << response << endl;

    try{
        content = json::parse(response);
    }catch(exception e){
        cout << e.what();
    }

    
    //cout << "JSON parse: " << content;
    if (content["samples"] != nullptr){
        auto data = content["samples"][0][1];// 資料拉下來會有兩筆，index 0 為較新的 data，index 1 為較舊的 data
        //int data_2 = atoi(content["samples"][1][1]); // 新註冊時推上去只有一筆資料，所以這邊直接跑會報錯，先註解掉
        
        //cout << "data: " << endl;
        //cout << data << endl;
        //print(data_2)
        retStr = data[0];
        //this_thread::sleep_for(chrono::milliseconds(100)); // 連續拉取資料時需要 sleep 停頓迴圈
    }
    cout << "iottalk_pull\n";
    return retStr;
}

vector<string> iottalk_pull_multi(){
    string url = "http://" + iottalk_server + ":" + iottalk_server_port + "/" + iottalk_mac + "/" + iottalk_output_device_feature;
    string response = requests::get(url);
    vector<string> retStrV;
    //cout << "iottalk pull response: \n" << response << endl;

    json content = json::parse(response);
    //cout << "JSON parse: " << content;
    if (content["samples"] != nullptr){
        auto data = content["samples"][0][1];// 資料拉下來會有兩筆，index 0 為較新的 data，index 1 為較舊的 data
        //int data_2 = atoi(content["samples"][1][1]); // 新註冊時推上去只有一筆資料，所以這邊直接跑會報錯，先註解掉
        
        //cout << "data: " << endl;
        //cout << data << endl;
        //print(data_2)
        for(string s : data[0]){
            retStrV.emplace_back(s);
        }
        //this_thread::sleep_for(chrono::milliseconds(100)); // 連續拉取資料時需要 sleep 停頓迴圈
    }
    cout << "iottalk_pull\n";
    return retStrV;
}