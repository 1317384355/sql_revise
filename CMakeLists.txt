cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(demo LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "e:/qt5.14.2/5.14.2/mingw73_64")                      # Qt Kit Dir
set(OpenCV_DIR "C:/libs/OpenCV-MinGW-Build-OpenCV-4.5.2-x64/x64/mingw/lib") # OpenCV Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(C:/libs/QxOrm-1.4.9-MinGW/QxOrm.cmake) # 暂时看不懂
include_directories("C:/libs/QxOrm-1.4.9-MinGW/include") # 引入外部头文件
link_directories("C:/libs/QxOrm-1.4.9-MinGW/lib")	     # 引入动态链接库
set(CMAKE_DEBUG_POSTFIX "d")

find_package(OpenCV REQUIRED )                                       # OpenCV COMPONENTS
find_package(Qt5 COMPONENTS Core Sql Gui Widgets Quick Qml Sql REQUIRED) # Qt COMPONENTS

# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

LIST(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "ui")  # 必须, 用于寻找ui文件

FILE(GLOB SRCS ./**)                             # 设置总路径 
FILE(GLOB WIDGET_HEADERS "./include/widget/*.h") # 添加头文件
FILE(GLOB DB_HEADERS "./include/db/*.h")
FILE(GLOB SOURCE "./src/*.cpp")                  # 添加源文件
FILE(GLOB WIDGET_SOURCE "./src/widget/*.cpp")
FILE(GLOB DB_SOURCE "./src/db/*.cpp")
FILE(GLOB GUI "./ui/*.ui")                       # 添加ui文件
LIST(APPEND SRCS ${SOURCE} ${WIDGET_HEADERS} ${DB_HEADERS} ${WIDGET_SOURCE} ${DB_SOURCE} ${GUI} ) # 变量合并

add_executable(${PROJECT_NAME}
    # WIN32 # If you need a terminal for debug, please comment this statement 
    ${SRCS}
)

# 必须修改
target_compile_definitions(${PROJECT_NAME} PRIVATE -D_BUILDING_QX_PRO) # _BUILDING_QX_PRO 按需修改 
target_link_libraries(${PROJECT_NAME} ${QX_LIBRARIES} QxOrm)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::Qml Qt5::Sql) # Qt5 Shared Library