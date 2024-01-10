set(CMAKE_CXX_STANDARD 23)

# Add the shader source files to the ShaderLibrary target
add_library(ObjectLoader object_loader.cpp object_loader.h)

# Link ShaderLibrary with Glad and other dependencies
target_link_libraries(ObjectLoader PRIVATE glm glad)

target_include_directories(ObjectLoader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../primitives")
target_include_directories(ObjectLoader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# You can set additional properties or dependencies for ShaderLibrary here if needed.
