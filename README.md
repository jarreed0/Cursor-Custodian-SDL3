# [Cursor Custodian](https://github.com/PolyMarsDev/Cursor-Custodian)

Cursor Custodian is a game created in 48 hours for the [2020 GMTK Game Jam](https://itch.io/jam/gmtk-2020) using C++ and [SDL](https://www.libsdl.org/). It can be played in your web browser [here](https://polymars.itch.io/cursor-custodian).
## Screenshots
![](https://img.itch.zone/aW1hZ2UvNzAwMzMzLzM4NzgxNjkuZ2lm/347x500/fqtnfO.gif) 

![](https://img.itch.zone/aW1hZ2UvNzAwMzMzLzM4NjIyOTgucG5n/347x500/9sZ24Q.png) ![](https://img.itch.zone/aW1hZ2UvNzAwMzMzLzM4NjIyOTkucG5n/347x500/qe2Eg7.png) ![](https://img.itch.zone/aW1hZ2UvNzAwMzMzLzM4NjIzMjAucG5n/347x500/yZPiM2.png) ![](https://img.itch.zone/aW1hZ2UvNzAwMzMzLzM4NjIzMjIucG5n/347x500/c6hUSn.png)


## Background
Cursor Custodian is a simple endless platformer where you use your cursor to guide the player away from obstacles. The player moves away from your cursor and will jump if you click the left mouse button near him. More information on how to play is available on the game's [itch.io page](https://polymars.itch.io/cursor-custodian).

## Compiling
### Windows
After installing [Mingw64](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/8.1.0/threads-win32/seh/x86_64-8.1.0-release-win32-seh-rt_v6-rev0.7z/download), [SDL](https://www.libsdl.org/download-2.0.php), [SDL_Image](https://www.libsdl.org/projects/SDL_image/), [SDL_TTF](https://www.libsdl.org/projects/SDL_ttf/), and [SDL_Mixer](https://www.libsdl.org/projects/SDL_mixer/), execute the following command in the project's root directory:
```
g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include -I C:/SDL3-w64/include && g++ *.o -o bin/release/main -s -L C:/SDL3-w64/lib -lmingw32 -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer && start bin/release/main
```
The compiled ``.exe`` is located in ``./bin``. For it to run, you must copy the ``./res`` folder as well as all ``.dll`` files from your SDL installation to its directory. (Note: this SDL3 build has not been tested on Windows).
### Linux
After installing the dev packages of SDL3 for your distribution, execute the following command in the project's root directory:
```
g++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include && mkdir -p bin/release && g++ *.o -o bin/release/main -s -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer
```
The compiled binary ``main`` is located in ``./bin``. For it to run, you must copy the ``./res`` folder to its directory.
### Web
Emscripten has not been tested with this SDL3 version but hypothetically should be implemented in the SDL3 main callbacks.

## Contributing
Pull requests are welcome! For major refactors, please open an issue first to discuss what you would like to improve. Feel free to create a fork of this repository or use the code for any other noncommercial purposes.

[Originally created in SDL2 by PolyMars](https://github.com/PolyMarsDev/Cursor-Custodian). It now using SDL3.
