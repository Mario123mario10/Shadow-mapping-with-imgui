# Add the shader source files to the ShaderLibrary target
add_library(Camera camera.cpp camera.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(Camera PRIVATE glm)

target_include_directories(Camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# You can set additional properties or dependencies for ShaderLibrary here if needed.
