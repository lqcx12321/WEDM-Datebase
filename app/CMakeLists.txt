cmake_minimum_required(VERSION 3.10)
project(app)

# -------------------------------------------------------
# 必须启用 Qt 的 AUTOMOC / AUTOUIC / AUTORCC 系统
# -------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# -------------------------------------------------------
# 查找 Qt5 模块
# -------------------------------------------------------
find_package(Qt5 REQUIRED COMPONENTS Widgets Sql Charts)

# -------------------------------------------------------
# 添加源文件
# -------------------------------------------------------
set(SOURCES
    main.cpp
    src/ui/mainwindow.cpp
    src/dao/db.cpp
    src/service/selectionservice.cpp
    src/service/recastdialog.cpp
)

set(HEADERS
    src/ui/mainwindow.h
    src/dao/db.h
    src/service/selectionservice.h
    src/service/recastdialog.h
    src/model/types.h
)

qt5_add_resources(RESOURCES)

add_executable(wedm_app
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

target_include_directories(wedm_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(wedm_app PRIVATE
    Qt5::Widgets
    Qt5::Charts
    Qt5::Sql
    dataprocess
    recommend
)
