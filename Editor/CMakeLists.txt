add_executable(${APPLICATION})

target_sources(${APPLICATION}
    PRIVATE
        "src/Core/Editor.cpp"
    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS
        include
    FILES
        "include/Core/Editor.h"
)

target_include_directories(${APPLICATION}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

include(FetchContent)

set_property(TARGET ${APPLICATION} PROPERTY CXX_STANDARD 20)

target_link_libraries(${APPLICATION} PUBLIC ${CORE_LIBRARY})

add_custom_command(TARGET "${APPLICATION}" POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy 
    $<TARGET_FILE:${CORE_LIBRARY}> $<TARGET_FILE_DIR:${APPLICATION}>/)