set(TEMPO_CLIENT_SOURCES
	${CMAKE_CURRENT_LIST_DIR}/include/client/component/ComponentHealthBar.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/component/ComponentKeyInput.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/component/ComponentRenderButtonGroup.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/component/ComponentRenderSceneNode.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/component/ComponentRenderSpikes.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/network/client.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemAttack.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemButtonRenderer.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemCombo.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemEntity.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemGraphicsCreation.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemLighting.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemMovement.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemParseKeyInput.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemRenderGUI.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemRenderHealing.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemRenderHealthBars.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemRenderSceneNode.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemRenderAttack.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemRenderSpikes.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemStageRenderer.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemTranslationAnimation.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/system/SystemUpdateKeyInput.hpp

	${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentHealthBar.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentKeyInput.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentRenderButtonGroup.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentRenderSceneNode.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentRenderSpikes.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/network/client.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemAttack.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemButtonRenderer.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemCombo.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemEntity.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemGraphicsCreation.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemLighting.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemMovement.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemParseKeyInput.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemRenderGUI.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemRenderHealing.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemRenderHealthBars.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemRenderSceneNode.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemRenderAttack.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemRenderSpikes.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemStageRenderer.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemTranslationAnimation.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemUpdateKeyInput.cpp

	${CMAKE_CURRENT_LIST_DIR}/include/client/misc/CBatchingMesh.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/misc/Color.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/misc/Lighting.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/misc/RGBtoHSV.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/misc/YAlignedBillboardSceneNode.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/client/misc/Camera.hpp
	${CMAKE_CURRENT_LIST_DIR}/src/misc/CBatchingMesh.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/misc/Color.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/misc/Lighting.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/misc/RGBtoHSV.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/misc/YAlignedBillboardSceneNode.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/misc/Camera.cpp
)
