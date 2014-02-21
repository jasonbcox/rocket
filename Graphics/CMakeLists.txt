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

set( SOIL2_headers
	./SOIL2/image_DXT.h
	./SOIL2/image_helper.h
	./SOIL2/SOIL2.h
	./SOIL2/stb_image.h
	./SOIL2/stb_image_write.h
	./SOIL2/stbi_DDS.h
	./SOIL2/stbi_DDS_c.h
	./SOIL2/stbi_ext_c.h
	./SOIL2/etc1_utils.h
	./SOIL2/pkm_helper.h
	./SOIL2/pvr_helper.h
	./SOIL2/stbi_pkm_c.h
	./SOIL2/stbi_pvr_c.h
	./SOIL2/stbi_pvr.h
)
set(SOIL2_sources
	./SOIL2/image_DXT.c
	./SOIL2/image_helper.c
	./SOIL2/SOIL2.c
	./SOIL2/etc1_utils.c
	./SOIL2/stb_image.c
	./SOIL2/stb_image_write.c
)

add_library ( SOIL2 ${SOIL2_headers} ${SOIL2_sources} )

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
	Scissor.h
	Shader.h
	ShaderDefaults.h
	Sprite.h
	Button.h
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
	Scissor.cpp
	Shader.cpp
	ShaderDefaults.cpp
	Sprite.cpp
	Button.cpp
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
	SOIL2
)

