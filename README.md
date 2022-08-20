# GameOfLife
A playground for Conway's Game of Life

**Note:**
**Will be migrated to pure C and OpenGL as massively scalable in the future when I can bother.**
Currently cannot be reasonably scaled. Rendering breaks down when there are more than one cells per pixel, and should be re-written.
Optimization is not really a concern, since it cannot be scaled to a degree where it would matter anyway.

---

Program has the ability of setting the initial conditions either manually, or from an appropriate monochrome bitmap file, although the user must specify how many pixels each grid square is. Alternatively it can be initialized randomly.

Haven't yet added usage manual, so you probably wont be able to use it without reading the code. Although following examples may be useful assuming that the current directory contains `GameOfLife.exe`, `SDL2.dll` and `gun_50ppc.bmp` files, present on the release.

Following call will randomly populate the number x number array of cells, prompt for confirmation and then run the game.

```
./GameOfLife -p <number> -i random
```

Alternatively, one may wish to initialize manually, as follows

```
./GameOfLife -p <number> -i manual
```
which will create an empty array of number x number cells and prompt user to fill it. Left click sets a cell alive, and right click sets it dead.


Initializing by an image has the following syntax when calling the program. 

```
./GameOfLife.exe -p <number> -i image --path <path/to/image> -ppc <# pixels per cell in the image> --color <alive cell color on the image>
```

For example, the following call

```
./GameOfLife.exe -p 50 -i image --path ./gun_50ppc.bmp -ppc 50 --color black
```

Creates an array of 50x50 cells, sets the initialization type to by image, who has the given path, and live cell color is black on the given image. This is the default configuration anyway, and running the program with no arguments with no arguments produces the same result. Image here is the image of a glider gun taken from the Wikipedia.
