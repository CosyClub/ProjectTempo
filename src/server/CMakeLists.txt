link_directories("${TEMPO_LIB_DIR}")

add_executable(server
	${CMAKE_CURRENT_LIST_DIR}/include/server/system/SystemAI.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/server/system/SystemAttack.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/server/system/SystemCombo.hpp
	${CMAKE_CURRENT_LIST_DIR}/include/server/system/SystemMovement.hpp

	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemAI.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemAttack.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemCombo.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/system/SystemMovement.cpp

	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set_target_properties(server PROPERTIES OUTPUT_NAME "RaveCave_Server")

target_include_directories(server PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(server PRIVATE ${TEMPO_SFML_INCLUDE_DIR})
target_include_directories(server PRIVATE ${TEMPO_LIB_TEMPO_INCLUDE_DIR})
target_include_directories(server PRIVATE ${TEMPO_ANAX_INCLUDE_DIR})
target_include_directories(server PRIVATE ${TEMPO_GLM_INCLUDE_DIR})

target_link_libraries(server lib_tempo)
target_link_libraries(server sfml-audio)
target_link_libraries(server sfml-system)
target_link_libraries(server sfml-network)
target_link_libraries(server anax)

target_compile_options(server PRIVATE ${SRC_FLAGS})

# Ensure running from visual studio works correctly (finds resources for the binary)
set_target_properties(server PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${TEMPO_BIN_DIR})
