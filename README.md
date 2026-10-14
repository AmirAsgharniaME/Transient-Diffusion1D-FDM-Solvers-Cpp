# Transient 1D Diffusion FDM Solvers

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-3.20%2B-064F8C.svg?logo=cmake&logoColor=white)](https://cmake.org/)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![Build Status](https://img.shields.io/badge/Build-CMake%20supported-blue.svg)

A self-contained, modular C++20 repository for solving the one-dimensional transient diffusion equation with four finite-difference schemes. Its internal libraries provide geometry and mesh data, boundary and scalar-field handling, numerical discretizations, linear solvers, simulation setup, file output, and Gnuplot visualization.

## Contents

- [Model](#model)
- [Numerical methods](#numerical-methods)
- [Requirements](#requirements)
- [Build](#build)
- [Run](#run)
- [Configure a simulation](#configure-a-simulation)
- [Results and visualization](#results-and-visualization)
- [Project layout](#project-layout)
- [Architecture](#architecture)

## Model

The application solves the 1D transient diffusion equation

$$
\frac{\partial u}{\partial t} = \nu \frac{\partial^2 u}{\partial y^2}, \quad 0 \le y \le L
$$

where $u$ is a scalar field, $\nu$ is the diffusion coefficient, $L$ is the domain length, and $t$ is time. The simulation uses a uniform mesh and fixed-value (Dirichlet) conditions at the domain endpoints. For $N$ mesh nodes, the spacing and diffusion number are

$$
\Delta y = \frac{L}{N-1}, \qquad r = \frac{\nu\,\Delta t}{(\Delta y)^2}.
$$

## Numerical methods

| Solver executable | Method | Description |
|---|---|---|
| `Explicit_FTCS` | FTCS | Forward-time, centered-space explicit update. The solver reduces the time step when needed to satisfy the FTCS diffusion-number stability limit. |
| `Explicit_DUFORT_FRANKEL` | DuFort–Frankel | Explicit three-time-level update. Its first level is initialized with FTCS; startup is substepped when needed to satisfy the FTCS stability limit. |
| `Implicit_Laasonen` | Laasonen (backward Euler) | Fully implicit update solved with the Thomas TDMA algorithm. |
| `Implicit_CrankNicolson` | Crank–Nicolson | Trapezoidal time discretization solved with the Thomas TDMA algorithm. |

The repository also includes Gaussian elimination with partial pivoting for general dense linear systems.

## Requirements

- CMake 3.20 or newer
- A C++20 compiler supported by CMake (GCC, Clang, AppleClang, or MSVC)
- Gnuplot on `PATH` for live plotting

The project declares no external C++ package dependencies.

## Build

Run these commands from the repository root:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

The four solver executables are written to `bin/`:

```text
bin/Explicit_FTCS
bin/Explicit_DUFORT_FRANKEL
bin/Implicit_Laasonen
bin/Implicit_CrankNicolson
```

To build an individual solver, use its CMake target name:

```sh
cmake --build build --target Explicit_FTCS
cmake --build build --target Explicit_DUFORT_FRANKEL
cmake --build build --target Implicit_Laasonen
cmake --build build --target Implicit_CrankNicolson
```

To build only the internal static libraries:

```sh
cmake --build build --target cfd_all_libraries
```

## Run

Launch an executable from the repository root so relative output paths resolve to `Results/`:

```sh
./bin/Explicit_FTCS
./bin/Explicit_DUFORT_FRANKEL
./bin/Implicit_Laasonen
./bin/Implicit_CrankNicolson
```

Each executable creates the shared simulation context, selects its scheme, advances the solution, applies boundary values, and tracks relative field change. The run stops when the configured steady-state tolerance or maximum number of time levels is reached. The explicit solvers also poll for the Escape key, and the programs may wait for Enter before exiting.

## Configure a simulation

Default compile-time inputs are defined in [`Config/SolverInputs.hpp`](Config/SolverInputs.hpp). Edit these values and rebuild to change the simulation.

| Input | Default | Meaning |
|---|---:|---|
| `Geometry::Length` | `0.04` | Domain length, in meters |
| `Physics::nu` | `0.00217` | Diffusion coefficient |
| `Mesh::N` | `40` | Number of uniform mesh nodes |
| `Solver::NumTimeLevels` | `20000` | Maximum number of time levels |
| `Solver::dt` | `0.0001` | Requested time step |
| `Solver::Tolerance` | `1e-6` | Relative field-change threshold for steady-state stopping |
| `InitialCondition::Profile` | `N` zeros | Initial value at each mesh node |
| `BoundaryCondition::TopWallValue` | `0.0` | Value at the top endpoint |
| `BoundaryCondition::BottomWallValue` | `40.0` | Value at the bottom endpoint |

These are C++ constants, not runtime configuration values. The mesh spacing is calculated as $\Delta y = \frac{L}{N-1}$.

## Results and visualization

The field writer creates the output directories as needed and writes whitespace-delimited `.dat` files with mesh coordinate and field value columns. Results are organized as follows:

```text
Results/
├── Initial_Field/                  # Initial field
├── Numerical_Field/
│   ├── FTCS/                       # FTCS results
│   ├── DUFORT_FRANKEL/              # DuFort–Frankel results
│   ├── Laasonen/                   # Laasonen results
│   └── CrankNicolson/               # Crank–Nicolson results
└── Analytical_Field/               # Analytical steady-state and final-time fields
```

The simulation writes the initial field and analytical reference on startup, periodic numerical fields during execution, and final numerical and analytical fields at completion. The internal plotting dashboard and Gnuplot integration provide live convergence and field-comparison plots. Install Gnuplot and ensure it is available on `PATH` to use those plots. Generated result files are ignored by Git.

## Project layout

```text
.
├── Config/                         # Compile-time solver inputs and configuration notes
├── Core/
│   ├── Geometry/                   # Physical domain geometry
│   ├── Mesh/                       # One-dimensional uniform mesh
│   ├── Boundary/                   # Boundary values and boundary management
│   └── Field/                      # Scalar fields and field names
├── Discretization/
│   ├── Explicit/FTCS/               # FTCS update and stability handling
│   ├── Explicit/DuFortFrankel/      # DuFort–Frankel update
│   ├── Implicit/Laasonen/           # Laasonen system assembly
│   ├── Implicit/CrankNicolson/      # Crank–Nicolson system assembly
│   ├── Implicit/Boundary/           # Implicit boundary enforcement
│   └── SolverScheme/               # Shared solver scheme definitions
├── LinearSolvers/
│   ├── ThomasAlgorithm/             # Thomas tridiagonal matrix algorithm (TDMA)
│   ├── GaussianElimination/         # Dense Gaussian elimination with partial pivoting
│   └── Matrix/                      # Coefficient matrices and right-hand sides
├── SimulationSetup/
│   ├── SimulationParams/            # Simulation parameter setup
│   ├── SimulationBoundaries/        # Boundary setup for the simulation
│   ├── SimulationContext/           # Shared simulation state and orchestration
│   └── PostProcessing/PlotDashboard/ # Real-time plotting dashboard
├── IO/
│   ├── OutputWriter/                # Field writer and ASCII .dat output
│   ├── PathManager/                 # Output path construction
│   └── Visualization/gnuplot/       # Gnuplot live and line plotting
├── Utilities/                      # Convergence, time-scale, status, and keyboard helpers
├── Verification/                   # Analytical reference solution
├── main/                           # Entry point for each solver executable
├── Results/                        # Runtime output location
├── CMakeLists.txt                  # C++20 build and internal library definitions
└── README.md                       # Project guide
```

## Architecture

The repository is organized as internal C++ libraries with clear responsibilities:

| Module | Responsibility |
|---|---|
| `Core/` | Geometry, mesh, boundaries, and scalar fields |
| `Discretization/` | FTCS, DuFort–Frankel, Laasonen, and Crank–Nicolson schemes |
| `LinearSolvers/` | Thomas TDMA algorithm and Gaussian elimination with partial pivoting |
| `SimulationSetup/` | Parameters, simulation context, and real-time plotting dashboard |
| `IO/` | Field writer, ASCII `.dat` exporter, and Gnuplot live visualization |

CMake builds the modules as static libraries and links them into the four solver executables. Shared setup, field management, convergence tracking, output, and plotting are coordinated by the simulation context and its supporting modules.
