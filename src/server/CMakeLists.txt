link_directories("${TEMPO_LIB_DIR}")

add_executable(server
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_include_directories(server PRIVATE ${TEMPO_SFML_INCLUDE_DIR})

target_link_libraries(server sfml-network)

set_target_properties(server PROPERTIES OUTPUT_NAME "RaveCave_Server")
