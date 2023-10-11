# Add the shader source files to the ShaderLibrary target
add_library(Texture texture.cpp texture.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(Texture PRIVATE glad)

target_include_directories(Texture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# You can set additional properties or dependencies for ShaderLibrary here if needed.
