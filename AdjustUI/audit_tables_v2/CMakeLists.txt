cmake_minimum_required(VERSION 3.20.0)
project(audit_tables_v2)

add_subdirectory(lua-5.4.4)

# set(JSON_Install OFF CACHE INTERNAL "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add_subdirectory(OpenXLSX)
# set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(${PROJECT_NAME}
    main.cpp
    src/table.cpp
    src/tableManager.cpp
    src/auditor.cpp
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/includes
    ${PROJECT_SOURCE_DIR}/lua-5.4.4/src
)
target_link_libraries(${PROJECT_NAME} OpenXLSX::OpenXLSX nlohmann_json::nlohmann_json lualib)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

list(APPEND EXE_FILE_LOCATION Release Debug)

foreach(LOCATION IN LISTS EXE_FILE_LOCATION)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/rule.json
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${LOCATION}/rule.json
    )

    file(GLOB LUA_SCRIPTS scripts/*.lua)
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${LOCATION}/scripts)
    foreach(lua_script IN LISTS LUA_SCRIPTS)
        configure_file(${lua_script} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${LOCATION}/scripts/ COPYONLY)
    endforeach()
endforeach()
