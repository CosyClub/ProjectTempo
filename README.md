# Rave Cave

  Repository containing the source of Rave Cave client, server, and related tools.

# Build Steps

  Build is managed by CMake - and hence project can be build from command line or in an IDE of your choice. Install CMake, then:

  1. Clone Repository
	2. If on Linux, run the "install_dependencies.sh" script
     :TODO: only works on Debian based systems currently...
	3. Create a directory "build" in root of repository
  4. Navigate to build directory in terminal
  5. Run the command `cmake ..` from the build directory, this will take a while as it builds Ogre and its dependencies
	6. Build the project:
     - On Linux CMake will generate make files by default, so just run `make` in the build directory
     - On Windows CMake will generate Visual Studio Project Files by default, open these and build as usual
	7. Produced binaries will be placed in the "bin" folder, run them from there
