# COS344 – Computer Graphics Practicals

**Student:** u23536030  
**Course:** COS344 – Computer Graphics  
**University of Pretoria**

---

## Table of Contents

- [P2](#p2)
- [P3](#p3)
- [P4](#p4)

---

## P2

A 2D OpenGL scene editor supporting shape placement, transformation, selection, wireframe rendering, and scene persistence.

![Images](DemoScene.png)

### Dependencies

Ensure the following are installed before building:

```bash
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    libglfw3-dev \
    libglew-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libglm-dev
```

### Quick Start (Submission Folder)

If you received a flat submission folder, a single script handles everything:

```bash
# 1. Navigate into the submission folder
cd submission/

# 2. Restore structure, build, and run — all in one command
./submission.sh makerun
```

### Manual Build (From Source)

If you have the full project structure:

```bash
# 1. Clone / extract the project, then navigate to the root
cd P2/

# 2. Create and enter the build directory
mkdir -p build && cd build

# 3. Configure with CMake
cmake ..

# 4. Build (uses all available cores)
make -j$(nproc)

# 5. Run
make run
# or directly:
./bin/MyOpenGLProject
```

### submission.sh Reference

The `submission.sh` script supports the following commands, all run from **inside** the submission folder:

| Command | Run from | Description |
|---|---|---|
| `./submission.sh create` | Project root | Flatten all files into `./submission/` |
| `./submission.sh makerun` | `submission/` | Restore structure → build → run |
| `./submission.sh clean` | `submission/` | Remove build artifacts (keeps save files) |

### Controls

| Key | Action |
|---|---|
| `1` | Select golf ball |
| `2` / `3` | Select obstacle |
| `4` | Select golf hole |
| `0` | Deselect |
| `W A S D` | Move selected object |
| `+` / `-` | Scale up / down |
| `Q` / `E` | Rotate CCW / CW |
| `Enter` | Toggle wireframe |
| `G` | Toggle debug grid |
| `P` | Pause / Resume |
| `R` | Restart scene |
| `↑` / `↓` | Increase / decrease FPS target |

### Project Structure

```
P2/
├── main.cpp
├── CMakeLists.txt
├── submission.sh
├── src/
│   ├── *.cpp
│   ├── glad.c
│   └── shaders/
│       ├── fragment/fragment.glsl
│       ├── vertex/vertex.glsl
│       └── wireframe.glsl
├── include/
│   ├── engine/
│   ├── math/
│   ├── glad/
│   └── KHR/
└── build/
    └── bin/
        ├── MyOpenGLProject
        ├── Demo.txt
        └── DemoSave.txt
```

### Troubleshooting

**`cmake` not found**
```bash
sudo apt install cmake
```

**GLFW / GLEW not found during CMake configure**
```bash
sudo apt install libglfw3-dev libglew-dev
```

**Linker errors with X11**
```bash
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

**Program runs but window doesn't appear**  
Ensure you are running in a graphical environment (not a headless SSH session without X forwarding). If using SSH, enable X forwarding with `ssh -X`.

---

## P3

> _Coming soon._

---

## P4

> _Coming soon._

---

*Last updated: 2026*
