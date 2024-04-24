#include <iostream>
#include <string>
#include <chrono>
//#include "lib/iottalk_utils.h"
#include "iottalk_utils.h"

using namespace std;

int main(){
    std::cout << "iottalk round trip time test.\n";
    std::cout << "Press Enter to start testing." << std::endl;
    std::cin.get();
    Register();
    cout << "register";
    
    string s1 = "hello nthu";
    string s2;
    int cnt = 0;
    while(true){
        auto start_time = chrono::steady_clock::now();
        iottalk_push(s1,s1);
        auto elapse_push = chrono::steady_clock::now() - start_time;
        s2 = iottalk_pull();
        auto elapse_pull = chrono::steady_clock::now() - start_time;

        auto total_ms = chrono::duration_cast<chrono::milliseconds>(elapse_pull);
        cout << "run " << cnt << " elapse time " << total_ms.count() << " ms, rate: " << 1.0 / (total_ms.count() / 1000.0) << " Hz\n";
        cnt++;
    }
    return 0;
}
