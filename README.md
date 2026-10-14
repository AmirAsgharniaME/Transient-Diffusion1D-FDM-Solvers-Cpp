# Transient 1D Diffusion FDM Solvers

A C++20 application for solving the one-dimensional transient diffusion equation with four finite-difference schemes. The project includes shared simulation setup, explicit and implicit discretizations, direct linear solvers, analytical reference fields, field-file output, and gnuplot visualization.

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

The application solves the one-dimensional diffusion equation

$$
\frac{\partial u}{\partial t} = \nu \frac{\partial^2 u}{\partial y^2},
\qquad 0 \le y \le L,
$$

where $u$ is a scalar field, $\nu$ is the diffusion coefficient, $L$ is the domain length, and $t$ is time. The current simulation setup uses a uniform mesh and fixed-value (Dirichlet) conditions at the two ends of the domain.

For $N$ mesh nodes, the spacing is $\Delta y = L/(N-1)$. The diffusion number used by the schemes is

$$
r = \frac{\nu\,\Delta t}{(\Delta y)^2}.
$$

## Numerical methods

| Executable | Scheme | Description |
|---|---|---|
| `Explicit_FTCS` | FTCS | Forward-time, centered-space explicit update. The program reduces `dt` when needed to satisfy the FTCS diffusion-number stability limit. |
| `Explicit_DUFORT_FRANKEL` | DuFort–Frankel | Explicit three-time-level update. Its first level is initialized with FTCS; if the configured diffusion number is too large, that startup is substepped using a stable FTCS step. |
| `Implicit_Laasonen` | Laasonen (backward Euler) | Fully implicit system solved with the Thomas tridiagonal algorithm. |
| `Implicit_CrankNicolson` | Crank–Nicolson | Trapezoidal time discretization solved with the Thomas tridiagonal algorithm. |

The Thomas algorithm is used by the implicit executables. Gaussian elimination with partial pivoting is also available in `LinearSolvers/` as a general dense solver.

## Requirements

- CMake 3.20 or newer
- A C++20 compiler supported by CMake (GCC, Clang, AppleClang, or MSVC)
- gnuplot available on `PATH` for the live convergence and field comparison plots

CMake enables warning and optimization flags for supported compilers and enables interprocedural optimization when the compiler supports it. No external C++ package dependencies are declared.

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

To build just one solver, configure the project as above and use its CMake target name:

```sh
cmake --build build --target Explicit_FTCS
cmake --build build --target Explicit_DUFORT_FRANKEL
cmake --build build --target Implicit_Laasonen
cmake --build build --target Implicit_CrankNicolson
```

To build only the static libraries, use `cmake --build build --target cfd_all_libraries`.

## Run

Launch an executable from the repository root. The application uses relative paths for its results directory, so running it from the repository root keeps outputs under the project’s `Results/` directory.

```sh
./bin/Explicit_FTCS
./bin/Explicit_DUFORT_FRANKEL
./bin/Implicit_Laasonen
./bin/Implicit_CrankNicolson
```

Each executable constructs the shared simulation context, selects its scheme, advances up to the configured number of time levels, applies the boundary values, tracks relative field change, and stops when the configured steady-state tolerance is reached or the time-level limit is reached. The explicit executables also poll for the Escape key. The programs may wait for a final Enter key after completing.

## Configure a simulation

Default inputs are centralized in [`Config/SolverInputs.hpp`](Config/SolverInputs.hpp). Edit these compile-time values and rebuild to change the simulation:

| Setting | Current default | Meaning |
|---|---:|---|
| `Geometry::Length` | `0.04` | Domain length |
| `Physics::nu` | `0.00217` | Diffusion coefficient |
| `Mesh::N` | `40` | Number of uniform mesh nodes |
| `Solver::NumTimeLevels` | `20000` | Maximum number of time levels |
| `Solver::dt` | `0.0001` | Requested time step |
| `Solver::Tolerance` | `1e-6` | Relative field-change threshold for steady-state stopping |
| `InitialCondition::Profile` | all zeros | Initial nodal field values |
| `BoundaryCondition::TopWallValue` | `0.0` | Value at the top endpoint |
| `BoundaryCondition::BottomWallValue` | `40.0` | Value at the bottom endpoint |

These settings are C++ constants, not values loaded from a runtime configuration file. The mesh contains `N` nodes and computes its spacing as `Length / (N - 1)`.

## Results and visualization

The field writer creates the output directories as needed and writes whitespace-delimited `.dat` files. Each file has two columns: mesh coordinate and field value. Outputs are organized as follows:

```text
Results/
├── Initial_Field/       # Initial field
├── Numerical_Field/
│   ├── FTCS/
│   ├── DUFORT_FRANKEL/
│   ├── Laasonen/
│   └── CrankNicolson/
└── Analytical_Field/    # Analytical steady-state and final-time fields
```

The simulation writes the initial field and steady-state analytical reference on startup, periodic numerical fields during the run, and final numerical and analytical fields when the steady-state tolerance is met. `SimulationSetup` and `IO/Visualization/gnuplot/` provide live convergence plotting and field comparison plots; install gnuplot and ensure it can be found on `PATH` to use them. Generated `.dat` files under `Results/` are ignored by Git.

## Project layout

```text
Config/             Compile-time inputs and shared enums
Core/               Geometry, mesh, boundaries, and scalar fields
Discretization/     Explicit and implicit schemes and boundary enforcement
LinearSolvers/      Dense/tridiagonal matrix data and direct solvers
SimulationSetup/    Parameters, boundaries, simulation context, and plot dashboard
IO/                 Field output, path/name helpers, and gnuplot integration
Utilities/          Relative-change, time-scale, status, and keyboard utilities
Verification/       Analytical diffusion solution
main/               One entry point for each solver executable
Results/            Runtime field output
```

`PROJECT_STRUCTURE.dat` contains a more detailed source-by-source architecture map and data-flow description.

## Architecture

CMake builds the code as static libraries grouped by responsibility: `cfd_core`, `cfd_explicit`, `cfd_linear_solvers`, `cfd_implicit`, `cfd_io`, `cfd_utilities`, `cfd_verification`, and `cfd_simulation_setup`. Each executable in `main/` selects a scheme and delegates common setup, field management, convergence tracking, result writing, and plotting to `SimulationContext` and its supporting modules.
