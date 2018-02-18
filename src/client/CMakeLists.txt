link_directories("${TEMPO_LIB_DIR}")

add_executable(client
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemStageRenderer.hpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemStageRenderer.cpp

	${CMAKE_CURRENT_LIST_DIR}/include/client/component/ComponentRenderSceneNode.hpp
	${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentRenderSceneNode.cpp

	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set_target_properties(client PROPERTIES OUTPUT_NAME "RaveCave")

target_include_directories(client PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(client PRIVATE ${TEMPO_IRRLICHT_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_SFML_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_SDL2_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_LIB_TEMPO_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_ANAX_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_GLM_INCLUDE_DIR})

target_link_libraries(client ${IRRLICHT_LIBRARY})
# Irrlicht Dependencies
if(UNIX)
	target_link_libraries(client z)
	target_link_libraries(client X11)
	target_link_libraries(client Xxf86vm)
endif(UNIX)

target_link_libraries(client lib_tempo)

target_link_libraries(client sfml-system)
target_link_libraries(client sfml-network)
target_link_libraries(client sfml-audio)

target_link_libraries(client SDL2)
target_link_libraries(client SDL2main)
target_link_libraries(client anax)

find_package(OpenGL REQUIRED)
target_link_libraries(client ${OPENGL_gl_LIBRARY} )
target_link_libraries(client ${OPENGL_glu_LIBRARY})

# Ensure running from visual studio works correctly (finds resources for the binary)
set_target_properties(client PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${TEMPO_BIN_DIR})
