##########################################
# SFML
##########################################
# Extract archive
execute_process(COMMAND cmake -E tar xf SFML-2.4.2.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
# Build libraries
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_DOCS     OFF)
add_subdirectory(SFML-2.4.2)

##########################################
# SDL2
##########################################
# Extract archive
execute_process(COMMAND cmake -E tar xf SDL2-2.0.7.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
# Build libraries
add_subdirectory(SDL2-2.0.7)

##########################################
# Anax
##########################################
# Extract archive
execute_process(COMMAND cmake -E tar xf anax-1.1.1.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
# Build libraries
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(anax-1.1.1)
set(BUILD_SHARED_LIBS  ON)
# Copy in generated headers
configure_file(
	"${CMAKE_CURRENT_BINARY_DIR}/anax-1.1.1/include/anax/Config.hpp"
	"${TEMPO_ANAX_INCLUDE_DIR}/anax/Config.hpp"
	COPYONLY
)

##########################################
# GLM
##########################################
# Extract archive
execute_process(COMMAND cmake -E tar xf glm-0.9.8.5.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
# Build libraries
##########################################
# GLM
add_subdirectory(glm)

##########################################
# Irrlicht
##########################################
# Group output into subfolder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${TEMPO_BIN_DIR}/irrlicht/examples/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${TEMPO_BIN_DIR}/irrlicht/examples/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${TEMPO_BIN_DIR}/irrlicht/examples/bin/")
# Extract archive
execute_process(COMMAND cmake -E tar xf irrlicht-1.8.4.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
file(COPY "${CMAKE_CURRENT_LIST_DIR}/irrlicht-cmake/" DESTINATION "${CMAKE_CURRENT_LIST_DIR}/irrlicht-1.8.4/")
# Build libraries
add_subdirectory(irrlicht-1.8.4)
# Ensure we can run examples and tools
file(COPY "${CMAKE_CURRENT_LIST_DIR}/irrlicht-1.8.4/media/" DESTINATION "${TEMPO_BIN_DIR}/irrlicht/media/")
# Reset bin output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${TEMPO_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${TEMPO_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${TEMPO_BIN_DIR}")
