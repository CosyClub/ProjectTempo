# Rave Cave

Repository containing the source of Rave Cave client, server, and related tools.

[![Build Status](https://travis-ci.org/jnterry/nano-orm.svg?branch=master)](https://travis-ci.org/CosyClub/ProjectTempo.svg?branch=master)

## Build Steps

The build is managed by CMake, hence the project can be built from command line
or in an IDE of your choice.

1. Create a directory `build` in the project’s root directory
2. Run `cmake ..` from in the build directory from the terminal
3. Use the generated build files to compile the project. On Linux, make files
   are generated, so run make in the build directory. On Windows Visual Studio 
   project files are generated, so open and build the project
4. The produced executables will be placed in the bin directory inside the 
   project's root

## Run Steps

To run the game you must start a server and at least one client - although the
level design requires 3 clients to be running to progress. The steps outlined 
below give commands to be run from the terminal assuming the current working 
directory is the bin directory within the root of the repository.

1. Navigate to the `./bin` directory.
2. On the server machine, run the server with no arguments: 
```
./RaveCave_Server
```

2. On the client machine(s), run clients with the following arguments:
```
./RaveCave [ServerIP] [PlayerParty] [HUD]
```

The `ServerIP` will be printed by the server executable on startup, 
`PlayerParty` should be an integer between 0 and 4. The `HUD` argument is 
optional, and should be added on a 1920x1080 screen to enable the splash screen
and HUD.

_Note: Running clients on the same machine as the server may incur performance
losses._
