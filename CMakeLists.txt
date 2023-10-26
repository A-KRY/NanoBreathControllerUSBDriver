cmake_minimum_required(VERSION 3.5)

project(NanoBreathControllerUSBDriver VERSION 0.1 LANGUAGES CXX)

# 改为您 vulkan.h 的路径
# Replace with your vulkan.h path.
set(Vulkan_INCLUDE_DIR  "I:\\Qt\\6.6.0\\Src\\qtwebengine\\src\\3rdparty\\chromium\\third_party\\vulkan-deps\\vulkan-headers\\src\\include\\vulkan")

# 改为您 teVirtualMIDISDK 的路径
# Replace with your teVirtualMIDISDK path.
set(teVirtualMIDISDK_DIR "C:/Program Files (x86)/Tobias Erichsen/teVirtualMIDISDK/C-Binding")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools SerialPort)

set(GEN_TS_FILES
        zh_CN.ts
        en_US.ts
        ja_JP.ts
)

set(GEN_QM_FILES
        zh_CN.qm
        en_US.qm
        ja_JP.qm
)

set(TS_FILES
        zh_CN.ts
        en_US.ts
        ja_JP.ts
)

include_directories(${teVirtualMIDISDK_DIR})

set(PROJECT_SOURCES
        main.cpp
        driverwindow.cpp
        driverwindow.h
        driverwindow.ui
        ExponentialMovingAverage.cpp
        ExponentialMovingAverage.h
        ExponentialMovingAverage.h
        NanoEventFilter.cpp
        NanoEventFilter.h
        NanoEventFilter.h
)

# 定义要生成的TS文件的目标名称
add_custom_target(generate_ts)
add_custom_target(generate_qm)

# 使用add_custom_command执行lupdate命令
add_custom_command(
        TARGET generate_ts
        COMMAND lupdate ${PROJECT_SOURCES} -ts ${GEN_TS_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running lupdate to generate TS files"
)

add_custom_command(
        TARGET generate_qm
        COMMAND lrelease ${TS_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running lupdate to generate QM files"
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NanoBreathControllerUSBDriver
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${TS_FILES}
        resource.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET NanoBreathControllerUSBDriver APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(NanoBreathControllerUSBDriver SHARED
            ${PROJECT_SOURCES}
            ${TS_FILES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(NanoBreathControllerUSBDriver
            ${PROJECT_SOURCES}
            ${TS_FILES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(NanoBreathControllerUSBDriver PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::SerialPort
        ${teVirtualMIDISDK_DIR}/teVirtualMIDI64.lib)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.NanoBreathControllerUSBDriver)
endif()
set_target_properties(NanoBreathControllerUSBDriver PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS NanoBreathControllerUSBDriver
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(NanoBreathControllerUSBDriver)
endif()
