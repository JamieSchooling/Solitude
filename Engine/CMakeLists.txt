add_library (${CORE_LIBRARY} SHARED)

set_property(TARGET ${CORE_LIBRARY} PROPERTY CXX_STANDARD 20)

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

FetchContent_Declare(imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.92.7-docking.zip
)

FetchContent_Declare(json 
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(glfw imgui json)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(imgui PRIVATE glfw)
target_include_directories(imgui
    PUBLIC
      ${imgui_SOURCE_DIR}
      ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(${CORE_LIBRARY} PUBLIC glfw)
target_link_libraries(${CORE_LIBRARY} PUBLIC imgui)
target_link_libraries(${CORE_LIBRARY} PUBLIC nlohmann_json::nlohmann_json)