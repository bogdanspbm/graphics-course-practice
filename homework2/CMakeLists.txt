cmake_minimum_required(VERSION 3.0)
project(homework2)

cmake_policy(SET CMP0072 NEW)
cmake_policy(SET CMP0074 NEW)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

find_package(opengl REQUIRED)
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

add_executable(${TARGET_NAME} main.cpp
		objects/opengl/GLProgram.cpp
		utils/ShaderUtils.cpp
		utils/shaders/ScreenViewShader.h
		utils/shaders/MainShaders.h
		utils/shaders/ShadowShader.h
		objects/opengl/Camera.cpp
		objects/opengl/Camera.h
		objects/graphics/renderable/Renderable.cpp
		objects/graphics/renderable/Renderable.h
		structures/Vertex.h
		objects/graphics/renderable/Placeable.cpp
		objects/graphics/renderable/Placeable.h
		enums/GLProgramType.h
		utils/ObjectUtils.h
		objects/graphics/textures/Texture.cpp
		objects/graphics/textures/Texture.h
		libs/stb_image.h
		objects/graphics/textures/Material.cpp
		objects/graphics/textures/Material.h
		libs/stb_image.c
		enums/GLTextureType.h
		objects/graphics/light/structures/DirectionLight.h
		objects/graphics/light/Sun.cpp
		objects/graphics/light/Sun.h
		objects/graphics/light/Ambient.cpp
		objects/graphics/light/Ambient.h
		objects/graphics/light/structures/PointLight.h
		utils/MathUtils.h
		objects/graphics/renderable/ScreenView.cpp
		objects/graphics/renderable/ScreenView.h
		objects/opengl/FrameBuffer.cpp
		objects/opengl/FrameBuffer.h
        objects/input/KeyHandler.h
		objects/input/KeyHandler.cpp

)
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
