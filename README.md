# 2d-heat

A compact 2D heat equation solver and visualization project.


https://github.com/user-attachments/assets/94b02977-e901-40d9-9907-6669733bc874

https://github.com/user-attachments/assets/cdd6cda0-b072-4b85-988a-e6c810da08f7

https://github.com/user-attachments/assets/7a99b594-d6b5-4a44-bbe5-ee58698ea7db



## AI
The use of generative AI extends only to the ReadMe, Makefile and some little parts of the python script. Everything else was done and thought of by me.
## Summary

This repository computes and visualizes 2D heat diffusion on a rectangular plate. Two C++ examples generate time-series data files in `./build/`, and the Python visualization script converts the result into an animated GIF.

## What’s included

- `src/ex*.cpp` — a sparse linear solve with fixed boundary conditions using Eigen's `UmfPackLU`.
- `src/setups.h` — matrix assembly helpers, boundary setup, and plate I/O functions.
- `src/display.py` — reads generated data, creates a frame-by-frame animation, and saves a MP4.
- `/input.txt` — sample simulation parameters for all examples.
- `/gen_data.sh` / `app/gen_anim.sh / Makefile` — helper scripts for generating data and animations.

## How to use

0. Either edit the `input.txt` to use values you want into the 3 examples I have already setup, or write you own C++ file in `ex0.cpp`. This way you won't have to edit the other files (Makefile, display script, bash scripts) and everything should work. Use the methods provided in `setups.h`.
1. Build the C++ examples with the provided `Makefile`:
   ```bash
   make
   ```
   This produces `./build/ex*.exe`.
2. Generate data using one of the binaries or the helper script:
   - Run `./build/ex*.exe` to write `./build/data*.txt`.
   - Or use `gen_data.sh` to generate both data files from `input.txt`.
3. Create the animation with the Python visualizer:
   ```bash
   bash gen_anim.sh
   ```
   The script reads the selected `build/data*.txt` file and saves the result as `animation*.mp4`.
4. View the generated animation:
   - The main output file is `./build/animation*.mp4`.

## Output

The animation at `./build/animation3.mp4` shows an animation with no heat loss and 5 constant heat sources in the center. The animation `./build/animation1.mp4` shows similar animation, but with constant borders. The `./build/animation2.mp4` is the most interesting one, showing a non-square grid with 3 constant same borders, with the 4-th one being 0. It also has 3 constant points, combinig all possible features of this project into one showcase.

### Sources
https://doi.org/10.1016/j.ijheatmasstransfer.2025.126780
https://www-udc.ig.utexas.edu/external/becker/teaching/557/problem_sets/problem_set_fd_2dheat.pdf
https://upcommons.upc.edu/server/api/core/bitstreams/84c0b52e-8ade-4167-83b9-c2ec3a3c7ec7/content
