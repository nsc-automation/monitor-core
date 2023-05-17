cmake_minimum_required(VERSION 3.24)
project(monitor_core)

set(CMAKE_CXX_STANDARD 20)

set(SourceList
        main.cpp

        Renderer.cpp
        WindowViewport.cpp
        )

set(HeadersList
        pch.h
        Renderer.h

        WindowViewport.h)

add_executable(monitor_core WIN32 ${SourceList} ${HeadersList})
set_source_files_properties(${HeadersList} PROPERTIES HEADER_FILE_ONLY ON)

target_precompile_headers(monitor_core PRIVATE pch.h)

#add_subdirectory(external/tinyobjloader TinyObjectLoader)
#target_link_libraries(monitor_core PRIVATE TinyObjectLoader)
#target_include_directories(monitor_core PRIVATE external/tinyobjloader)