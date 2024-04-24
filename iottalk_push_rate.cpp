// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

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
    using namespace msr::airlib;
    using namespace std;

    msr::airlib::MultirotorRpcLibClient client;
    typedef ImageCaptureBase::ImageRequest ImageRequest;
    typedef ImageCaptureBase::ImageResponse ImageResponse;
    typedef ImageCaptureBase::ImageType ImageType;
    typedef common_utils::FileSystem FileSystem;

    try {
        client.confirmConnection();

        std::cout << "version: push_test 1.0\n";
        std::cout << "Press Enter to start testing." << std::endl;
        std::cin.get();
        std::string file_path = "/home/pause582/workspace/hellomiku/img/";

        int run_id = 0;
        while(true){
            auto start_time = chrono::steady_clock::now();

            /*const std::vector<ImageRequest> request{ ImageRequest("0", ImageType::Scene), 
                ImageRequest("1", ImageType::Scene), ImageRequest("2", ImageType::Scene), 
                ImageRequest("3", ImageType::Scene), ImageRequest("4", ImageType::Scene),
                ImageRequest("0", ImageType::DepthVis), 
                ImageRequest("1", ImageType::DepthVis), ImageRequest("2", ImageType::DepthVis), 
                ImageRequest("3", ImageType::DepthVis), ImageRequest("4", ImageType::DepthVis) };*/
            
            const std::vector<ImageRequest> request{ ImageRequest("0", ImageType::Scene),ImageRequest("4", ImageType::Scene)};
            const std::vector<ImageResponse>& response = client.simGetImages(request);

            /*for (const ImageResponse& image_info : response){
                char * base64_img = base64Encode(reinterpret_cast<const char*>(image_info.image_data_uint8.data()), image_info.image_data_uint8.size());
                string base64_img_str(base64_img);
                free(base64_img);
                iottalk_push(base64_img_str);
            }*/

            char * base64_img_0 = base64Encode(reinterpret_cast<const char*>(response[0].image_data_uint8.data()), response[0].image_data_uint8.size());
            char * base64_img_1 = base64Encode(reinterpret_cast<const char*>(response[1].image_data_uint8.data()), response[1].image_data_uint8.size());
            string base64_img_str0(base64_img_0);
            string base64_img_str1(base64_img_1);
            free(base64_img_0);
            free(base64_img_1);
            iottalk_push(base64_img_str0,base64_img_str1);

            auto elapse = chrono::steady_clock::now() - start_time;
            auto total_ms = chrono::duration_cast<chrono::milliseconds>(elapse);
            cout << "run " << run_id << " elapse time " << total_ms.count() << " ms, rate: " << 1.0 / (total_ms.count() / 1000.0) << " Hz\n";
            run_id++;
        }
        // #
        //std::cout << "# of images received: " << response.size() << std::endl;
    }
    catch (rpc::rpc_error& e) {
        const auto msg = e.get_error().as<std::string>();
        std::cout << "Exception raised by the API, something went wrong." << std::endl
                  << msg << std::endl;
    }

    return 0;
}
