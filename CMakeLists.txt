cmake_minimum_required(VERSION 3.16)

project("iottalk_utils_cpp")

# 新增一個 CMake 目標，目標型態為可執行檔。
add_executable(iottalk_test)
target_sources(iottalk_test
    PRIVATE
        "iottalk_test.cpp"
)
# 新增目標 project-02 的 Include 目錄
target_include_directories(iottalk_test
    PRIVATE
        "lib/"
)

# 將指定資料夾的 CMake 專案（含有 CMakeLists.txt）一起加入建置。
add_subdirectory("lib/")

# 新增目標 project-02 所連結的函式庫。函式庫名稱為其他 CMake 專案的目標名稱。通常來自
# find_package 或 add_subdirectory。以這個範例來說，libstatic1 函式庫是來自
# add_subdirectory 指令所加入的 CMake 專案。
target_link_libraries(iottalk_test
    PRIVATE
        lib
)