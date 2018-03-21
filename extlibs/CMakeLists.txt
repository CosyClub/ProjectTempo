##########################################
# SFML
##########################################
message ("SFML ===========================================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
# Setup SFML's audio dependencies correctly on lab machines
if (TEMPO_BUILD_LAB)
	message("*** LAB MACHINE: Overriding default audio dependencies")
	execute_process(COMMAND cmake -E tar xf lab-audio-dependencies.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

	set(FIND_LIBRARY_USE_LIB64_PATHS ON)
	find_package(OPENAL QUIET)
	find_package(VORBIS QUIET)
	find_package(FLAC QUIET)

	set(OPENAL_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/lab-audio-dependencies/include/openal/AL")
	set(OPENAL_LIBRARY "/usr/lib64/libopenal.so.1.16.0")

	set(VORBIS_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/lab-audio-dependencies/include/vorbis")
	set(OGG_INCLUDE_DIR    "${CMAKE_CURRENT_LIST_DIR}/lab-audio-dependencies/include/ogg"   )
	set(VORBISFILE_LIBRARY "/usr/lib64/libvorbisfile.so.3.3.5")
	set(VORBISENC_LIBRARY  "/usr/lib64/libvorbisenc.so.2.0.9" )
	set(VORBIS_LIBRARY     "/usr/lib64/libvorbis.so.0.4.6"    )
	set(OGG_LIBRARY        "/usr/lib64/libogg.so.0.8.0"       )

	set(FLAC_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/lab-audio-dependencies/include/flac")
	set(FLAC_LIBRARY     "${CMAKE_CURRENT_LIST_DIR}/lab-audio-dependencies/lib64/libFLAC.so")
endif()
# Extract archive
execute_process(COMMAND cmake -E tar xf SFML-2.4.2.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
# Build libraries
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_DOCS     OFF)
add_subdirectory(SFML-2.4.2)

##########################################
# Anax
##########################################
message ("ANAX ===========================================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
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
message ("GLM ============================================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
# Extract archive
execute_process(COMMAND cmake -E tar xf glm-0.9.8.5.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
# Build libraries
##########################################
# GLM
add_subdirectory(glm)

##########################################
# Irrlicht
##########################################
message ("IRRLICHT =======================================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
option (TEMPO_BUILD_IRRLICHT_EXTRAS "If set irrlicht examples and tools will be built" OFF)
if(${TEMPO_BUILD_IRRLICHT_EXTRAS})
	set(IRRLICHT_BUILD_EXAMPLES ON)
	set(IRRLICHT_BUILD_TOOLS    ON)
else()
	set(IRRLICHT_BUILD_EXAMPLES OFF)
	set(IRRLICHT_BUILD_TOOLS    OFF)
endif()
# Group output into subfolder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${TEMPO_BIN_DIR}/irrlicht/examples/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${TEMPO_BIN_DIR}/irrlicht/examples/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${TEMPO_BIN_DIR}/irrlicht/examples/bin/")
# Extract archive
message ("IRRLICHT EXTRACT ===============================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
execute_process(COMMAND cmake -E tar xf irrlicht-1.8.4.zip WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
message ("IRRLICHT COPY ==================================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/irrlicht-cmake/" DESTINATION "${CMAKE_CURRENT_LIST_DIR}/irrlicht-1.8.4/")
# Build libraries
message ("IRRLICHT ADD_SUBDIRECTORY ======================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
add_subdirectory(irrlicht-1.8.4)
# Ensure we can run examples and tools
message ("IRRLICHT COPY TOOLS ============================================================")
string(TIMESTAMP CMAKE_TIME)
message ("TIME: ${CMAKE_TIME}")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/irrlicht-1.8.4/media/" DESTINATION "${TEMPO_BIN_DIR}/irrlicht/media/")
# Reset bin output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${TEMPO_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${TEMPO_BIN_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${TEMPO_BIN_DIR}")
