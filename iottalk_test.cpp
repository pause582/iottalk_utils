#include <iostream>
#include <string>
#include <chrono>
//#include "lib/iottalk_utils.h"
#include "libiottalk/iottalk_utils.h"

using namespace std;
const int test_round = 20;

int main(){
    cout << "iottalk round trip time test.\n" << endl;
    cout << "Press Enter to start testing." << endl;
    cin.get();

    iottalk_helper iottalk = iottalk_helper("0425",{"teststring"},"20240425");
    iottalk.Register();
    cout << "registered" << endl;
    
    string s1 = "hello nthu";
    string s2;
    int cnt = 0;
    for(int i=0;i<test_round;i++){
        auto start_time = chrono::steady_clock::now();
        cout << "push string: " << s1 << endl;
        iottalk.push(s1);
        //iottalk.push_twin("hello miku", "hello luka");

        auto elapse_push = chrono::steady_clock::now() - start_time;
        s2 = iottalk.pull();
        auto elapse_pull = chrono::steady_clock::now() - start_time;
        cout << "pull string: " << s2 << endl;
        /*vector<string> vs = iottalk.pull_twin();
        for(auto s : vs) cout << s << endl;*/

        auto total_ms = chrono::duration_cast<chrono::milliseconds>(elapse_pull);
        cout << "run " << cnt << " elapse time " << total_ms.count() << " ms, rate: " << 1.0 / (total_ms.count() / 1000.0) << " Hz\n";
        cnt++;
    }
    return 0;
}
