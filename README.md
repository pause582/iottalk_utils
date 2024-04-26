# iottalk utils

[github repo](https://github.com/pause582/iottalk_utils_cpp)

## Requirement & Dependency

* cmake
`sudo apt install cmake`
* libcurl
`sudo apt install libcurl4-openssl-dev`
* [nlohmann/json](https://github.com/nlohmann/json) (embedded in repository)

## Usage

1. instantiate a iottalk_helper object.
constructor parameters: device_name, feature_list, mac_address
2. Call its `Register` function to regester info on iottalk server.

push: push 1 string to iottalk.
push_twin: push 2 string at the same time. (`testmodel` has 2 slots)
pull: pull 1 string from iottalk.
pull_twin: pull what have been pushed by push_twin.

### C++ version
Include files in folder **libiottalk**.
Include nlohmann/json package. (have been done for example project)

### python version
Import iottalk_helper from iottalk_utils.py


## Example

### C++ version
cd into the repo folder.
```
mkdir build
cmake ..
make
./iottalk_test
```

### python version

cd into the repo folder.
`python3 iottalk_test.py`