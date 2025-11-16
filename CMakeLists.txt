cmake_minimum_required(VERSION 3.5)
project(JuliaSet LANGUAGES CUDA CXX)

set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CXX_STANDARD 23)

set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
set(GLFW_DIR "${VENDOR_DIR}/glfw")
set(IMGUI_DIR "${VENDOR_DIR}/imgui")
set(GLAD_DIR "${VENDOR_DIR}/glad")

find_package(OpenGL REQUIRED)
find_package(CUDAToolkit REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_DIR})

add_subdirectory(${GLAD_DIR})

add_subdirectory(${IMGUI_DIR})

add_executable(${PROJECT_NAME} "src/JuliaSet.cu" "src/GlfwErrorHandler.h" "src/WindowInit.h" "src/KeyCallback.h")

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad imgui OpenGL::GL CUDA::cudart)