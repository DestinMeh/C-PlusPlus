cmake_minimum_required(VERSION 3.16)

project(MusicPlaylist VERSION 0.1 LANGUAGES CXX)

# 1. Standard C++ Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Find Qt Packages
find_package(Qt6 REQUIRED COMPONENTS Gui Qml Quick)

# 3. Automatic code generation for Qt (MOC, RCC, UIC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 4. Define the Executable and Source Files
# This follows the /src and /include structure
add_executable(MusicPlaylist
    src/main.cpp
    src/AudioBridge.cpp
    src/PlaylistManager.cpp
    src/Song.cpp

    include/AudioBridge.h
    include/PlaylistManager.h
    include/Song.h
    # Add your QML resources (explained below)
    ui/resources.qrc 
 "extern/miniaudio.h")

# 5. Include Directories
# Tells the compiler to look in /include and /extern for headers
target_include_directories(MusicPlaylist PRIVATE include extern
)

# 6. Link Libraries
# Links the Qt framework to your app
target_link_libraries(MusicPlaylist PRIVATE 
    Qt6::Gui 
    Qt6::Qml 
    Qt6::Quick
)

# 7. Platform Specifics (Optional)
if(WIN32)
    # Prevents a console window from popping up behind your GUI on Windows
    set_target_properties(MusicPlaylist PROPERTIES WIN32_EXECUTABLE TRUE)
endif()