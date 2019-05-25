#cmake_minimum_required(VERSION 3.12)
#project(Penguins C)

#set(CMAKE_C_STANDARD 11)

#add_executable(Penguins "Source Files/Penguins.c" "Source Files/Structures.c" "Header Files/Structures.h" "Source Files/Utilities.c" "Header Files/Utilities.h" )

#target_link_libraries(Penguins m)

cmake_minimum_required(VERSION 3.12)
project(Penguins C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_C_FLAGS "-pthread")

find_package (Opengl REQUIRED)


include_directories(OpenGL/GLFW/include OpenGL/GLAD/include OpenGL/GLM/include/cglm)


add_executable(Penguins "Source Files/Penguins.c" "OpenGL/GLAD/src/glad.c" "Source Files/Structures.c" "Header Files/Structures.h" "Source Files/Utilities.c" "Header Files/Utilities.h" "Source Files/GameWindow.c" "Header Files/GameWindow.h" "Source Files/SpriteRenderer.c" "Header Files/SpriteRenderer.h" "Header Files/Penguins.h" "OpenGL/GLM/src/" "Source Files/Resources.c" "Header Files/Resources.h" "Source Files/Input.c" "Header Files/Input.h" "Source Files/Physics.c" "Header Files/Physics.h" "Source Files/Movement.c" "Header Files/Movement.h" "Source Files/GUI.c" "Header Files/GUI.h" "Source Files/Menu.c" "Header Files/Menu.h")

target_link_libraries (Penguins D:\\Users\\Oskar\\Documents\\CLion\\Penguins\\OpenGL\\GLFW\\lib-mingw-w64\\glfw3.dll m)

