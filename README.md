# SimpleP3D
A Simple pseudo 3D game with using raycasting algorithm.

## Features
* ### Camera
  **W.A.S.D** movement
  * Rotation
  * Field of view
  * Near
  * Far
  * Fish eye effect
* ### Map
  * Collision
  * Raycasting with bresenham algorithm


## Dependencies
* [GLFW](https://github.com/glfw/glfw)
* [GLEW](https://github.com/nigels-com/glew), in particular [CMake porting](https://github.com/Perlmint/glew-cmake)
* [glText](https://github.com/vallentin/glText)

## Usage

### Windows

You can use some IDEs that support CMake, like [Visual studio](https://visualstudio.microsoft.com)  
Or you can (you need download cmake and some kind of compiler on windows)
```PowerShell
git clone https://github.com/Dae-Moon/SimpleP3D.git
cd SimpleP3D
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
start SimpleP3D.exe
```

### Linux
1. Install dependencies (CMake, Wayland, X11)
* #### Derbian
```sh
sudo apt install cmake
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev
```
* #### Fedora
```sh
sudo dnf install cmake
sudo dnf install wayland-devel libxkbcommon-devel libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel
```
* #### FreeBSD
```sh
sudo pkg install cmake
sudo pkg install wayland libxkbcommon evdev-proto xorgproto
```

2. Build
```sh
git clone https://github.com/Dae-Moon/SimpleP3D.git
cd SimpleP3D
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

3. Run
```sh
./SimpleP3D
```
