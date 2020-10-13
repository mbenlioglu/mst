# Crated by mbenlioglu on Oct 04, 2020.
# -------------------------------------------------------------

file(GLOB test_files "test_*.cpp")
file(GLOB data_files "../data/rmat0[456]*.gr")

foreach(test_src ${test_files})
    get_filename_component(exe ${test_src} NAME_WE)
    add_executable(${exe} ${test_src})
    target_include_directories(${exe} PUBLIC
            "${CMAKE_SOURCE_DIR}/include"
            "${CMAKE_SOURCE_DIR}/src")
    foreach(data_file ${data_files})
        get_filename_component(df ${data_file} NAME_WE)
        add_test(NAME ${exe}-${df} COMMAND
                ${CMAKE_CURRENT_BINARY_DIR}/${exe} ../data/${df}.gr ../data/${df}.extra results/${df}.out
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        set_tests_properties(${exe}-${df}
                PROPERTIES
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                TIMEOUT 100)
    endforeach()
endforeach()
