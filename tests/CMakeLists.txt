link_directories("${TEMPO_LIB_DIR}")

add_executable(unit-tests
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set_target_properties(unit-tests PROPERTIES OUTPUT_NAME "UnitTests")


target_include_directories(unit-tests PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_IRRLICHT_INCLUDE_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_SFML_INCLUDE_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_LIB_TEMPO_INCLUDE_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_ANAX_INCLUDE_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_GLM_INCLUDE_DIR})

target_link_libraries(unit-tests lib_tempo)
target_link_libraries(unit-tests sfml-system)
target_link_libraries(unit-tests sfml-network)
target_link_libraries(unit-tests sfml-audio)
