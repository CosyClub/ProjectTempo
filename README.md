# Rave Cave

  Repository containing the source of Rave Cave client, server, and related tools.

[![Build Status](https://travis-ci.org/jnterry/nano-orm.svg?branch=master)](https://travis-ci.org/CosyClub/ProjectTempo.svg?branch=master)

# Build Steps

The build is managed by CMake, hence the project can be built from command line
or in an IDE of your choice.

1. Clone Repository
2. Create a directory `build` in root of repository
3. Navigate to build directory in terminal
4. Run the command `cmake ..` from the build directory, this will take a while
   as it builds Ogre and its dependencies
5. Build the project:
    - On Linux CMake will generate make files by default, so just run `make` in
      the build directory
    - On Windows CMake will generate Visual Studio Project Files by default,
      open these and build as usual
6. Produced binaries will be placed in the `bin` folder, run them from there

# Running on lab machine
Instructions for building and running on Lab Machines:

  1. Pull the repo and navigate to the root folder. Then run:

```  
mkdir build
cd build
cmake ..
make [-j8]
cd ../bin/
```

  2. Now you need your IP address, for that run `ifconfig` and look for the IPv4
     address under `eno1`, it should be something like ~`137.222.xxx.xxx`~. Keep
     note of this IP address, I'll refer to it as `[ip_address]`
  3. Now run the server on one machine with `./RaveCave_Server`
  4. Now run the client on any machine in the lab with `./RaveCave [ip_address]`
  5. Et voila! You should be good to go!

_Note: Running clients on the same machine as the server may incur performance
losses._
