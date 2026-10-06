# ECS-game-engine

Simple 2D game engine built from scratch in C++, using an Entity-Component-System architecture and OpenGL rendering.

<img src="media/demo.gif" width="500">

## Features
- Entity-Component-System: entity lookup, component add/remove
- 2D OpenGL renderer with sprite sheet texture support
- Input handling

## In progress
- A Vampire Survivors-style game built on top of the engine
- Unit tests with Google Test
- CI with GitHub Actions

## Requirements
- OpenGL and GLFW development packages

## Build

```bash
git clone https://github.com/qwert33e1/ECS-game-engine.git
cd ECS-game-engine
./build.sh
```

## Project structure
- src/ — engine and game source code
- external/ — third-party libraries (Glad, picoPNG, glm)
- Textures/ — sprite assets