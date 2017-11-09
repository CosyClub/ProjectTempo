link_directories("${TEMPO_OGRE_LIBRARY_DIR}")
link_directories("${TEMPO_LIB_DIR}")

add_executable(client
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set_target_properties(client PROPERTIES OUTPUT_NAME "RaveCave")

target_include_directories(client PRIVATE ${TEMPO_SFML_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_OGRE_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_SDL2_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_LIB_TEMPO_INCLUDE_DIR})

target_link_libraries(client lib_tempo)

target_link_libraries(client sfml-system)
target_link_libraries(client sfml-network)
target_link_libraries(client sfml-audio)

target_link_libraries(client SDL2)
target_link_libraries(client SDL2main)

# :TODO: have way to configure debug vs release - no _d for release
set(TEMPO_OGRE_LIB_SUFFIX "Static_d")

target_link_libraries(client "RenderSystem_GL3Plus${TEMPO_OGRE_LIB_SUFFIX}")
target_link_libraries(client "RenderSystem_GL${TEMPO_OGRE_LIB_SUFFIX}")
target_link_libraries(client "OgreGLSupport${TEMPO_OGRE_LIB_SUFFIX}")
target_link_libraries(client "OgreOverlay${TEMPO_OGRE_LIB_SUFFIX}")
target_link_libraries(client "OgreRTShaderSystem${TEMPO_OGRE_LIB_SUFFIX}")
target_link_libraries(client "OgreMain${TEMPO_OGRE_LIB_SUFFIX}") # must be after all other Ogre links

find_package(OpenGL REQUIRED)
target_link_libraries(client ${OPENGL_gl_LIBRARY} )
target_link_libraries(client ${OPENGL_glu_LIBRARY})

target_link_libraries(client zzip)
target_link_libraries(client FreeImage)

if(MSVC)
	target_link_libraries(client zlibstaticd)
else()
	target_link_libraries(client z)
endif()

if(UNIX)
	link_directories(OrgeDemo "/usr/lib/;/usr/lib32/;/usr/lib64/")
	target_link_libraries(client X11)
	target_link_libraries(client Xt)
	target_link_libraries(client Xaw)
	target_link_libraries(client Xrandr)
	target_link_libraries(client dl)
	target_link_libraries(client pthread)
	target_link_libraries(client sfml-audio)
	target_link_libraries(client sfml-system)
endif(UNIX)

# Ensure running from visual studio works correctly (finds resources for the binary)
set_target_properties(client PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${TEMPO_BIN_DIR})
