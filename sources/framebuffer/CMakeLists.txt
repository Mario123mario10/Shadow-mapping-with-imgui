set(CMAKE_CXX_STANDARD 23)

# Add the shader source files to the ShaderLibrary target
add_library(Framebuffer framebuffer.cpp framebuffer.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(Framebuffer PRIVATE glad Texture Renderbuffer)

target_include_directories(Framebuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Framebuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../texture)
target_include_directories(Framebuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../renderbuffer)

# You can set additional properties or dependencies for ShaderLibrary here if needed.
