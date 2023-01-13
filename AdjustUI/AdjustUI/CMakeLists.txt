cmake_minimum_required(VERSION 3.20.0)
project(AdjustUI)

add_executable(${PROJECT_NAME}
    main.cpp
)
# target_include_directories(${PROJECT_NAME}
#     PRIVATE
#     ${PROJECT_SOURCE_DIR}/includes
#     ${PROJECT_SOURCE_DIR}/lua-5.4.4/src
# )
# target_link_libraries(${PROJECT_NAME} OpenXLSX::OpenXLSX nlohmann_json::nlohmann_json lualib)
target_link_libraries(${PROJECT_NAME} pugixml)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

file(GLOB CSD_FILES cocosstudio/ui/*.csd)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/cocosstudio/ui)
foreach(csd_file IN LISTS CSD_FILES)
    configure_file(${csd_file} ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/cocosstudio/ui COPYONLY)
endforeach()
