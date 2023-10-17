# Add the shader source files to the ShaderLibrary target
add_library(Texture texture.cpp texture.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(Texture PRIVATE glad stb_image)

target_include_directories(Texture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Texture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../primitives)


target_compile_definitions(Texture PUBLIC CUBEMAP_PATH="${CMAKE_SOURCE_DIR}/resources/cubemap/")

# You can set additional properties or dependencies for ShaderLibrary here if needed.
