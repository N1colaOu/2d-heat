# 2d-heat

A compact 2D heat equation solver and visualization project.

![Heat diffusion animation](./build/animation1.gif)

## Summary

This repository computes and visualizes 2D heat diffusion on a rectangular plate. Two C++ examples generate time-series data files in `./build/`, and the Python visualization script converts the result into an animated GIF.

## What’s included

- `src/ex0.cpp` — a sparse linear solve with fixed boundary conditions using Eigen's `SimplicialLLT`.
- `src/ex1.cpp` — a more general solver for boundary conditions on all plate edges using Eigen's `UmfPackLU` support.
- `src/setups.h` — matrix assembly helpers, boundary setup, and plate I/O functions.
- `src/display.py` — reads generated data, creates a frame-by-frame animation, and saves a GIF.
- `app/input.txt` — sample simulation parameters for both examples.
- `app/gen_data.sh` / `app/gen_anim.sh` — helper scripts for generating data and animations.
- `build/data0.txt` / `build/data1.txt` — precomputed simulation output files.

## How to use

1. Build one of the C++ examples (`ex0.cpp` or `ex1.cpp`) with Eigen and UMFPACK support.
2. Run the binary to write `./build/data0.txt` or `./build/data1.txt`.
3. Run `src/display.py` to load the data and produce `animation1.gif`.

## Output

The animation at `./build/animation1.gif` shows the heat diffusion over time, using a consistent colormap for clear comparison between simulation frames.
