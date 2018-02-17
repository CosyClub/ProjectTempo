link_directories("${TEMPO_OGRE_LIBRARY_DIR}")
link_directories("${TEMPO_LIB_DIR}")

add_executable(client
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set_target_properties(client PROPERTIES OUTPUT_NAME "RaveCave")

target_include_directories(client PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(client PRIVATE ${TEMPO_SFML_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_SDL2_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_LIB_TEMPO_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_ANAX_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_GLM_INCLUDE_DIR})

target_link_libraries(client lib_tempo)

target_link_libraries(client sfml-system)
target_link_libraries(client sfml-network)
target_link_libraries(client sfml-audio)

target_link_libraries(client SDL2)
target_link_libraries(client SDL2main)
target_link_libraries(client anax)

target_link_libraries(client freetyped            )
if(UNIX)
	target_link_libraries(client harfbuzz)
endif(UNIX)

find_package(OpenGL REQUIRED)
target_link_libraries(client ${OPENGL_gl_LIBRARY} )
target_link_libraries(client ${OPENGL_glu_LIBRARY})

target_link_libraries(client zzip)

if(MSVC)
	target_link_libraries(client zlibstaticd)
else()
	target_link_libraries(client z)
	target_link_libraries(client FreeImage)
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
