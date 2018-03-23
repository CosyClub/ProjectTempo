link_directories("${TEMPO_LIB_DIR}")

add_executable(client
	${CMAKE_CURRENT_LIST_DIR}/include/client/component/ComponentHealthBar.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/component/ComponentKeyInput.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/component/ComponentRenderSceneNode.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/component/ComponentRenderButtonGroup.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/network/client.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemAttack.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemButtonRenderer.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemCombo.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemEntity.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemGraphicsCreation.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemMovement.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemParseKeyInput.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemRenderGUI.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemRenderHealthBars.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemRenderSceneNode.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemStageRenderer.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemTranslationAnimation.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemUpdateKeyInput.hpp

	${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentHealthBar.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentKeyInput.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentRenderSceneNode.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentRenderButtonGroup.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/network/client.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemAttack.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemButtonRenderer.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemCombo.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemEntity.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemGraphicsCreation.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemMovement.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemParseKeyInput.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemRenderGUI.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemRenderHealthBars.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemRenderSceneNode.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemStageRenderer.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemTranslationAnimation.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemUpdateKeyInput.cpp

	${CMAKE_CURRENT_LIST_DIR}/include/client/misc/CBatchingMesh.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/misc/RGBtoHSV.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/misc/YAlignedBillboardSceneNode.hpp
	${CMAKE_CURRENT_LIST_DIR}/src/misc/CBatchingMesh.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/misc/RGBtoHSV.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/misc/YAlignedBillboardSceneNode.cpp

	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set_target_properties(client PROPERTIES OUTPUT_NAME "RaveCave")

target_include_directories(client PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(client PRIVATE ${TEMPO_IRRLICHT_INCLUDE_DIR})
target_include_directories(client PRIVATE ${TEMPO_SFML_INCLUDE_DIR})
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

target_link_libraries(client anax)

find_package(OpenGL REQUIRED)
target_link_libraries(client ${OPENGL_gl_LIBRARY} )
target_link_libraries(client ${OPENGL_glu_LIBRARY})

# Ensure running from visual studio works correctly (finds resources for the binary)
set_target_properties(client PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${TEMPO_BIN_DIR})
