cmake_minimum_required(VERSION 3.5)
project(JuliaSet LANGUAGES CUDA CXX)

set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CXX_STANDARD 20)

set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
set(GLFW_DIR "${VENDOR_DIR}/glfw")
set(IMGUI_DIR "${VENDOR_DIR}/imgui")
set(GLAD_DIR "${VENDOR_DIR}/glad")
set(GLM_DIR "${VENDOR_DIR}/glm")
set(FREETYPE_DIR "${VENDOR_DIR}/freetype")

find_package(OpenGL REQUIRED)
find_package(CUDAToolkit REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_DIR})

add_subdirectory(${GLAD_DIR})
add_subdirectory(${GLM_DIR})
add_subdirectory(${IMGUI_DIR})
add_subdirectory(${FREETYPE_DIR})

add_executable(${PROJECT_NAME}
    src/Main.cpp

    src/Backend/GLFW.cpp 
    src/Backend/Backend.cpp 
    src/UserInput/UserInput.cpp
    src/UI/TextRenderer.cpp 
    src/API/GL_Backend.cpp
    src/UI/FpsService.cpp
    src/JuliaSet/JuliaKernels.cu
    src/JuliaSet/JuliaService.cpp
    src/Imgui/ImguiService.cpp)

configure_file(
    ${CMAKE_SOURCE_DIR}/src/misc/root_directory.h.in
    ${CMAKE_BINARY_DIR}/generated/root_directory.h
    @ONLY)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}/generated)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
    glm
    imgui
    freetype
    OpenGL::GL
    CUDA::cudart)
