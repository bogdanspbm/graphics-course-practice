cmake_minimum_required(VERSION 3.0)
project(practice8)

cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0074 NEW)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

if(APPLE)
	# brew version of glew doesn't provide GLEW_* variables
	get_target_property(GLEW_INCLUDE_DIRS GLEW::GLEW INTERFACE_INCLUDE_DIRECTORIES)
	get_target_property(GLEW_LIBRARIES GLEW::GLEW INTERFACE_LINK_LIBRARIES)
	get_target_property(GLEW_LIBRARY GLEW::GLEW LOCATION)
	list(APPEND GLEW_LIBRARIES "${GLEW_LIBRARY}")
endif()

add_subdirectory(glm)

set(TARGET_NAME "${PROJECT_NAME}")

set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${TARGET_NAME} main.cpp libs/stb_image.h libs/stb_image.c utils/ShaderUtils.cpp
		utils/ShaderUtils.h
		objects/opengl/ProgramAdapter.cpp
		objects/opengl/ProgramAdapter.h
		objects/input/KeyHandler.cpp
		objects/input/KeyHandler.h
		structures/Vertex.h
		structures/Vector4F.h
		structures/Vector2I.h
		objects/graphics/Renderable.cpp
		objects/graphics/Renderable.h
		structures/Vertex.h
		utils/ModelUtils.hpp
		utils/ModelUtils.cpp
		objects/graphics/Landscape.cpp
		objects/graphics/Landscape.h
		utils/MathUtils.cpp
		utils/MathUtils.h
		objects/graphics/Placeable.cpp
		objects/graphics/Placeable.h
		structures/Vector3F.cpp
		structures/Vector3F.h
		structures/Polygon.h
		objects/graphics/Isolines.h
		objects/graphics/Isolines.cpp
		objects/graphics/Texture.cpp
		objects/graphics/Texture.h
		objects/opengl/FrameBuffer.cpp
		objects/opengl/FrameBuffer.h
		objects/graphics/basic/Plane.cpp
		objects/graphics/basic/Plane.h
		objects/opengl/ScreenView.cpp
		objects/opengl/ScreenView.h
        structures/PointLight.h)
target_include_directories(${TARGET_NAME} PUBLIC
	"${SDL2_INCLUDE_DIRS}"
	"${GLEW_INCLUDE_DIRS}"
	"${OPENGL_INCLUDE_DIRS}"
)
target_link_libraries(${TARGET_NAME} PUBLIC
	glm
	"${GLEW_LIBRARIES}"
	"${SDL2_LIBRARIES}"
	"${OPENGL_LIBRARIES}"
)
target_compile_definitions(${TARGET_NAME} PUBLIC -DPROJECT_ROOT="${PROJECT_ROOT}")
