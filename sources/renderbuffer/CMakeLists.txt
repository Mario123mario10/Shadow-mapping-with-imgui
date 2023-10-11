# Add the shader source files to the ShaderLibrary target
add_library(Renderbuffer renderbuffer.cpp renderbuffer.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(Renderbuffer PRIVATE glad)

target_include_directories(Renderbuffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# You can set additional properties or dependencies for ShaderLibrary here if needed.
