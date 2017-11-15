set(TEMPO_LIB_TEMPO_HEADERS
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/Application.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/time.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/network.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/song.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/config.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/Position.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/Render.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/GridMotion.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/include/tempo/entity/GridAi.hpp"
)

set(TEMPO_LIB_TEMPO_SOURCE
	"${CMAKE_CURRENT_LIST_DIR}/src/Application.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/time.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/network.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/song.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/Position.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/Render.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/GridMotion.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/entity/GridAi.cpp"
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
