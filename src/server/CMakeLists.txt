link_directories("${TEMPO_LIB_DIR}")

add_executable(server
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set_target_properties(server PROPERTIES OUTPUT_NAME "RaveCave_Server")

target_include_directories(server PRIVATE ${TEMPO_SFML_INCLUDE_DIR})
target_include_directories(server PRIVATE ${TEMPO_LIB_TEMPO_INCLUDE_DIR})
target_include_directories(server PRIVATE ${TEMPO_ANAX_INCLUDE_DIR})
target_include_directories(server PRIVATE ${TEMPO_GLM_INCLUDE_DIR})

target_link_libraries(server lib_tempo)
target_link_libraries(server sfml-audio)
target_link_libraries(server sfml-system)
target_link_libraries(server sfml-network)
target_link_libraries(server anax)
