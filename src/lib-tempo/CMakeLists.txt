set(TEMPO_LIB_TEMPO_HEADERS
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/Application.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/config.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/model.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/song.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/structures.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/time.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/Types.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/component/ComponentGridAi.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/component/ComponentHealth.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/component/ComponentRender.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/component/ComponentTransform.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/EntityCreation.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/EntityCreationClient.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/EntityCreationServer.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/ID.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/RenderHealth.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/LevelManager.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/LevelRenderer.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/PlayerLocal.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/PlayerRemote.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/PlayerRemoteServer.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/TileMask.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/network/base.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/network/client.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/network/queue.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/network/server.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/system/SystemGridAi.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/system/SystemHealth.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/system/SystemRender.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/system/SystemTransform.hpp"
)

set(TEMPO_LIB_TEMPO_SOURCE
	"${CMAKE_CURRENT_LIST_DIR}/src/Application.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/model.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/song.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/structures.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/time.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentHealth.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentRender.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/component/ComponentTransform.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/EntityCreation.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/EntityCreationClient.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/EntityCreationServer.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/ID.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/RenderHealth.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/LevelManager.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/LevelRenderer.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/PlayerLocal.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/PlayerRemote.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/PlayerRemoteServer.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/TileMask.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/network/base.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/network/client.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/network/queue.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/network/server.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/system/SystemGridAi.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/system/SystemHealth.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/system/SystemRender.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/system/SystemTransform.cpp"
)

add_library(lib_tempo STATIC
	${TEMPO_LIB_TEMPO_HEADERS}
	${TEMPO_LIB_TEMPO_SOURCE}
)

target_include_directories(lib_tempo PRIVATE ${TEMPO_LIB_TEMPO_INCLUDE_DIR})
target_include_directories(lib_tempo PRIVATE ${TEMPO_OGRE_INCLUDE_DIR})
target_include_directories(lib_tempo PRIVATE ${TEMPO_SDL2_INCLUDE_DIR})
target_include_directories(lib_tempo PRIVATE ${TEMPO_SFML_INCLUDE_DIR})
target_include_directories(lib_tempo PRIVATE ${TEMPO_ANAX_INCLUDE_DIR})
target_include_directories(lib_tempo PRIVATE ${TEMPO_GLM_INCLUDE_DIR})
