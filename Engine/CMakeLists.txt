add_library (${CORE_LIBRARY} SHARED)

target_compile_definitions(${CORE_LIBRARY} PRIVATE BUILDING_CORE_LIBRARY)

target_sources(${CORE_LIBRARY}
    PRIVATE
        "src/Core/Application.cpp" 
        "src/Rendering/Window.cpp"
        "src/Core/EventSystem.cpp"
    PRIVATE
    FILE_SET HEADERS
    BASE_DIRS
        include
    FILES
        "include/Core/EntryPoint.h"
        "include/Core/Application.h"
        "include/Core/Subsystem.h"
        "include/Solitude.h" 
        "include/Rendering/Window.h" "include/Core/Event.h" "include/Core/EventSystem.h")

target_include_directories(${CORE_LIBRARY}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/include"
)

include(FetchContent)

FetchContent_Declare(
	glfw 
	GIT_REPOSITORY	https://github.com/glfw/glfw.git 
	GIT_TAG			3.5.1
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

target_link_libraries(${CORE_LIBRARY} PUBLIC glfw)