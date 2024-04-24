#include "common/common_utils/StrictMode.hpp"
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "rpc/rpc_error.h"
STRICT_MODE_ON

#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include "common/common_utils/FileSystem.hpp"
#include <iostream>
#include <chrono>
#include "lib/iottalk_helper.h"
#include "lib/base64.h"

int main()
{
    using namespace std;

    std::cout << "version: pull_test 1.0\n";
    std::cout << "Press Enter to start testing." << std::endl;
    std::cin.get();
    std::string file_path = "/home/pause582/workspace/hellomiku/img/";

    int run_id = 0;
    while(true){
        auto start_time = chrono::steady_clock::now();

        string pull_str = iottalk_pull();
        size_t img_size;
        char * base64_img_dec = base64Decode(pull_str.c_str(),&img_size);
                        
        std::ofstream file(file_path + to_string(start_time.time_since_epoch().count()) + ".png", std::ios::binary);
        file.write(base64_img_dec,img_size);
        file.close();
        free(base64_img_dec);

        auto elapse = chrono::steady_clock::now() - start_time;
        auto total_ms = chrono::duration_cast<chrono::milliseconds>(elapse);
        cout << "run " << run_id << " elapse time " << total_ms.count() << " ms, rate: " << 1.0 / (total_ms.count() / 1000.0) << " Hz\n";
        run_id++;
    }

    return 0;
}
