# Penguins
An EPFU game project.
Made by Oskar Hącel and Hytham Salah

# Disclamer
Folder *bin* contains compiled .exe files of Interactive and Autonomous modes, as well as all requied files to run the game.

# Compilation
First change
```make
target_link_libraries (Penguins **FULL_PATH_TO**_lib-mingw-w64\\glfw3.dll m)
```
in **CMakeLists.txt** file.

In order to compile an **Interactive** mode, please uncomment the line in **Penguins.c** file:
```c
#define INTERACTIVE   //Uncomment this line to compile an Interactive mode.
```
Otherwise the game will compile to an Autonomous mode.
