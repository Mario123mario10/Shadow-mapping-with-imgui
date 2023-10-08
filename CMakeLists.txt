cmake_minimum_required(VERSION 3.16)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>") #we want to use the static runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

project(mygame)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/glfw-3.3.2)			
add_subdirectory(thirdparty/glad)				
add_subdirectory(thirdparty/stb_image)			
add_subdirectory(thirdparty/stb_truetype)		
add_subdirectory(thirdparty/glm)				
add_subdirectory(thirdparty/imgui-docking)		


file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


file(GLOB_RECURSE MODULE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sources/*")

# here add subdirectory created in your module
add_subdirectory(sources/shader)
#add_subdirectory(sources/primitives)
add_subdirectory(sources/vertex_buffer)
add_subdirectory(sources/object_loader)

add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 23)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC MODELS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/models/")


target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} )


if(MSVC) # Microsoft Visual Studio
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
	set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
	set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

endif()

# here add directories with your own module
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/sources/shader")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/sources/vertex_buffer")

# here link against library created in your module
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE ShaderLibrary VertexBuffer ObjectLoader)

# thirdparty libraries
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glm glfw glad stb_image stb_truetype imgui)
