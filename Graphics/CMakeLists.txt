cmake_minimum_required( VERSION 2.8 )
project( RocketGraphics )

set( SOIL_headers
	./SOIL/image_DXT.h
	./SOIL/image_helper.h
	./SOIL/SOIL.h
	./SOIL/stb_image_aug.h
	./SOIL/stbi_DDS_aug.h
	./SOIL/stbi_DDS_aug_c.h
)
set(SOIL_sources
	./SOIL/image_DXT.c
	./SOIL/image_helper.c
	./SOIL/SOIL.c
	./SOIL/stb_image_aug.c
)

add_library ( SOIL ${SOIL_headers} ${SOIL_sources} )

set( RocketGraphics_headers
	Clickable.h
	Input.h
	Input_Interactable.h
	Mesh.h
	Object.h
	Object_BitmapText.h
	Object_Newton.h
	Raster.h
	Scene.h
	Shader.h
	ShaderDefaults.h
	Sprite.h
	Texture.h
	Transform.h
	Universe.h
)
set( RocketGraphics_sources
	Clickable.cpp
	Input.cpp
	Input_Interactable.cpp
	Mesh.cpp
	MeshLoader.cpp
	MeshPrimitives.cpp
	Object.cpp
	Object_BitmapText.cpp
	Object_Newton.cpp
	Raster.cpp
	Scene.cpp
	Shader.cpp
	ShaderDefaults.cpp
	Sprite.cpp
	Texture.cpp
	Transform.cpp
	Universe.cpp
	main.cpp
)

#add_library( RocketGraphics
add_executable( RocketGraphics
	${Rocket_sources}
	${Rocket_headers}
	${RocketGraphics_sources}
	${RocketGraphics_headers}
)
find_package( OpenGL REQUIRED )

find_package( PkgConfig REQUIRED )
pkg_search_module( GLFW REQUIRED glfw3 )
include_directories( ${GLFW_INCLUDE_DIRS} )

target_link_libraries( RocketGraphics
	RocketCore
	${OPENGL_LIBRARIES}
	GLEW
	GLU
	GL
	${GLFW_STATIC_LIBRARIES}
	SOIL
)

