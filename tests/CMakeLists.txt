link_directories("${TEMPO_LIB_DIR}")

include("${TEMPO_CLIENT_DIR}/client_sources.cmake")

add_executable(unit-tests
	# Ensure we can test code built into the client
	${TEMPO_CLIENT_SOURCES}

	# Main file for unit tests
	${CMAKE_CURRENT_LIST_DIR}/main.cpp

	# Test cases below...
	${CMAKE_CURRENT_LIST_DIR}/utilities/mask.cpp
	${CMAKE_CURRENT_LIST_DIR}/utilities/RGBandHSV.cpp
)

set_target_properties(unit-tests PROPERTIES OUTPUT_NAME "UnitTests")

message("****** ${TEMPO_CLIENT_INCLUDE_DIR}")
target_include_directories(unit-tests PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_CLIENT_INCLUDE_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_IRRLICHT_INCLUDE_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_SFML_INCLUDE_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_LIB_TEMPO_INCLUDE_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_ANAX_INCLUDE_DIR})
target_include_directories(unit-tests PRIVATE ${TEMPO_GLM_INCLUDE_DIR})

target_link_libraries(unit-tests lib_tempo          )
target_link_libraries(unit-tests sfml-system        )
target_link_libraries(unit-tests sfml-network       )
target_link_libraries(unit-tests sfml-audio         )
target_link_libraries(unit-tests ${IRRLICHT_LIBRARY})
target_link_libraries(unit-tests anax               )
