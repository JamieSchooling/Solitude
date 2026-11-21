add_executable(${APPLICATION})

set_property(TARGET ${APPLICATION} PROPERTY CXX_STANDARD 20)

target_sources(${APPLICATION}
    PRIVATE
        "src/Core/Editor.cpp"
        "src/Core/ProjectManager.cpp"
    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS
        include
    FILES
        "include/Core/Editor.h"
        "include/Core/ProjectManager.h" )

target_include_directories(${APPLICATION}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

include(FetchContent)

FetchContent_Declare(nfd
    URL https://github.com/btzy/nativefiledialog-extended/archive/refs/tags/v1.3.0.zip
)

FetchContent_Declare(zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG v1.3.1
    GIT_SHALLOW TRUE
)

FetchContent_Declare(ziplib
    GIT_REPOSITORY https://bitbucket.org/wbenny/ziplib.git
    GIT_TAG v0.01
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)

set(REPROC++ ON)
FetchContent_Declare(reproc++
    GIT_REPOSITORY https://github.com/daandemeyer/reproc.git
    GIT_TAG 14.2.8
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(nfd zlib ziplib reproc++)

add_library(ZipLib STATIC
    ${ziplib_SOURCE_DIR}/Source/ZipLib/EndOfCentralDirectoryBlock.cpp
    ${ziplib_SOURCE_DIR}/Source/ZipLib/ZipArchive.cpp
    ${ziplib_SOURCE_DIR}/Source/ZipLib/ZipArchiveEntry.cpp
    ${ziplib_SOURCE_DIR}/Source/ZipLib/ZipFile.cpp
    ${ziplib_SOURCE_DIR}/Source/ZipLib/ZipCentralDirectoryFileHeader.cpp
    ${ziplib_SOURCE_DIR}/Source/ZipLib/ZipGenericExtraField.cpp
    ${ziplib_SOURCE_DIR}/Source/ZipLib/ZipLocalFileHeader.cpp
)
target_include_directories(ZipLib
    PUBLIC
      ${ziplib_SOURCE_DIR}/Source
)
target_link_libraries(ZipLib PUBLIC zlibstatic)

target_link_libraries(${APPLICATION} PUBLIC ${CORE_LIBRARY})
target_link_libraries(${APPLICATION} PRIVATE nfd)
target_link_libraries(${APPLICATION} PRIVATE ZipLib)
target_link_libraries(${APPLICATION} PRIVATE reproc++)

add_custom_command(TARGET "${APPLICATION}" POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy 
    $<TARGET_FILE:${CORE_LIBRARY}> $<TARGET_FILE_DIR:${APPLICATION}>/)

add_custom_command(TARGET "${APPLICATION}" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${APPLICATION}>/resources)