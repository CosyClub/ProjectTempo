#!/usr/bin/env bash

set -e

echo "This script is about to install a some packages..."
echo "You might want to look over what it does before contining"
read -p "Press enter to continue, or Ctrl-C to exit"

if [ `command -v apt` ]
then
		echo "Installing packages with apt..."
		# Build utils
		sudo apt install cmake
		# SFML
		sudo apt install libudev-dev libopenal-dev libvorbis-dev libflac-dev
elif [ `command -v yum` ]
then
		echo "Broken on Centos at the moment..."
		# Seems to be some error where cmake cant work out if pthreads is installed
		# Getting the exact same error as on:
		# https://cmake.org/pipermail/cmake/2015-December/062283.html

		#echo "Installing packages with yum..."
		# Build utils
		#sudo yum install cmake gcc-c++

elif [ `command -v pacman` ] # Keep this last since it will launch pacman game on ubuntu...
then
		echo "Installing packages with pacman..."
		pacman -S cmake base-devel
		# SFML
		pacman -S openal flac libvorbis libx11 libxrandr

else
		echo "Failed to find a suitable package manager with which to install dependencies"
		exit 1
fi
