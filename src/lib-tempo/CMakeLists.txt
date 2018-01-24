set(TEMPO_LIB_TEMPO_HEADERS
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/Application.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/config.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/network.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/song.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/structures.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/config.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/time.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/Tile.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/Types.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/Transform.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/Render.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/ID.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/LevelManager.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/GridAi.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/PlayerInput.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/EntityCreation.hpp"
)

set(TEMPO_LIB_TEMPO_SOURCE
	"${CMAKE_CURRENT_LIST_DIR}/src/Application.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/network.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/song.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/structures.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/time.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/Tile.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/Transform.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/Render.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/ID.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/LevelManager.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/GridAi.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/PlayerInput.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/EntityCreation.cpp"
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
