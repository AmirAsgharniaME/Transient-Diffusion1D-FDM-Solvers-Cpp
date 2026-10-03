# Transient 1D Diffusion FDM Solvers in Modern C++

[![Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=flat&logo=c%2B%2B)](https://en.cppreference.com/w/cpp/17)
[![Compiler](https://img.shields.io/badge/Compiler-GCC%20%2F%20Clang-orange.svg?style=flat)](https://gcc.gnu.org/)
[![Build](https://img.shields.io/badge/Build-Makefile-lightgrey.svg?style=flat)](Makefile)
[![Field](https://img.shields.io/badge/CFD-Finite%20Difference%20Method-green.svg)](#governing-equation)

A modular, high-performance, object-oriented numerical framework written in **Modern C++ (C++17)** to solve the **one-dimensional transient diffusion equation** (parabolic PDE) using classical explicit and implicit Finite Difference Methods (FDM).

The project includes custom linear algebra solvers, live runtime tracking, stabilization verification, and exact analytical solutions for rigorous code verification and bench-marking.

---

## Table of Contents
- [Governing Equation & Physics](#governing-equation--physics)
- [Numerical Schemes Implemented](#numerical-schemes-implemented)
- [Software Architecture & Design](#software-architecture--design)
- [Linear Solvers](#linear-solvers)
- [Code Verification](#code-verification)
- [Directory Structure](#directory-structure)
- [Prerequisites & Building](#prerequisites--building)
- [Running the Solvers](#running-the-solvers)
- [Key Features](#key-features)

---
## Governing Equation & Physics

The solver targets the 1D unsteady heat conduction / viscous diffusion equation:

$$\frac{\partial u}{\partial t} = \nu \frac{\partial^2 u}{\partial y^2}$$

where:
- $u(y, t)$: Scalar field (velocity or temperature profile)
- $\nu$: Diffusion coefficient / kinematic viscosity
- $y \in [0, L]$: One-dimensional spatial domain
- $t$: Time domain

### Discretization Parameters
- Spatial mesh size: $\Delta y = \frac{L}{N - 1}$
- Time step: $\Delta t$
- Diffusion number (mesh Fourier number / CFL for diffusion):
  $$d = \frac{\nu \Delta t}{(\Delta y)^2}$$

---

## Numerical Schemes Implemented

| Scheme | Type | Truncation Error | Stability Criterion | Solvers / Algorithms Used |
| :--- | :--- | :--- | :--- | :--- |
| **FTCS** (Forward-Time Central-Space) | Explicit | $\mathcal{O}(\Delta t, \Delta y^2)$ | Conditionally Stable ($d \le 0.5$) | Single-step vector update |
| **DuFort-Frankel** | Explicit | $\mathcal{O}(\Delta t^2, \Delta y^2, (\Delta t / \Delta y)^2)$ | Unconditionally Stable | Three-time-level formulation |
| **Laasonen (BTCS)** | Implicit | $\mathcal{O}(\Delta t, \Delta y^2)$ | Unconditionally Stable | Thomas (TDMA) / Gaussian Elimination |
| **Crank-Nicolson** | Implicit | $\mathcal{O}(\Delta t^2, \Delta y^2)$ | Unconditionally Stable | Thomas (TDMA) / Gaussian Elimination |

---
## Software Architecture & Design

The codebase adheres to Object-Oriented Programming (OOP) and modular separation-of-concerns principles:

- **`Core1D/`**: 
  - `Mesh1D`: Handles spatial discretization, uniform cell generation, and grid coordinates.
  - `Geometry1D`: Stores domain bounds and physical dimensions ($L$).
  - `BoundaryPoint`: Implements boundary conditions (Dirichlet/Neumann) at domain edges.
  - `Field1D`: Container for spatial scalar fields with optimized indexing and contiguous memory layout.
  - `Convergence1D`: Evaluates tolerance, residual norms ($L_1, L_2, L_\infty$), and convergence rates.
  - `FileWriter1D` & `PostProcessing1D`: Formats and outputs field profiles for external analysis and plotting.

- **`ExplicitSolvers1D/`**:
  - Encapsulates algebraic march algorithms for explicit time-stepping schemes (FTCS and multi-time-level DuFort-Frankel).

- **`ImplicitSolvers1D/`**:
  - `CoefficientMatrix1D`: Dynamically constructs the left-hand-side system matrix based on selected temporal weights ($\theta = 1$ for Laasonen, $\theta = 0.5$ for Crank-Nicolson).
  - `TridiagonalMatrix`: Memory-efficient storage storing only main diagonal ($b$), sub-diagonal ($a$), and super-diagonal ($c$) vectors.
  - `RHS1D`: Builds the explicit source and previous-time-level contribution vector.

- **`LinearSolvers/`**:
  - **Thomas Algorithm (TDMA)**: Specialized Gaussian elimination for tridiagonal systems with $\mathcal{O}(N)$ computational complexity and linear memory footprint.
  - **Gaussian Elimination**: General direct elimination algorithm with backward substitution, serving as a baseline solver and verification reference.

- **`Stability/`**:
  - Houses parameter validators (e.g., checking $d = \frac{\nu \Delta t}{\Delta y^2} \le 0.5$ for FTCS) to prevent numerical divergence prior to execution.

---

## Code Verification

To mathematically prove spatial and temporal convergence, all numerical predictions are validated against the exact closed-form analytical solution derived via separation of variables (Fourier series expansion):

$$u(y, t) = u_{\text{steady}}(y) + \sum_{m=1}^{\infty} C_m \sin\left(\frac{m \pi y}{L}\right) \exp\left(-\nu \left(\frac{m \pi}{L}\right)^2 t\right)$$

The `Verification/AnalyticalDiffusion1D` class computes the exact field at arbitrary time levels $t$ and automatically calculates discrete error norms:
- $L_1$ Norm: Average absolute deviation across all grid nodes.
- $L_2$ Norm: Root-mean-square (RMS) error norm.
- $L_\infty$ Norm: Maximum local absolute error:
  $$\|E\|_\infty = \max_{1 \le j \le N} |u_j^{\text{numerical}} - u_j^{\text{analytical}}|$$

---

## Directory Structure

```text
├── bin/                       # Output directory for compiled binaries
├── Config/                    # Physical parameters, runtime switches, and path managers
├── Core1D/                    # Core mesh, geometry, scalar fields, and file exporters
├── ExplicitSolvers1D/         # Explicit time-stepping engine (FTCS, DuFort-Frankel)
├── ImplicitSolvers1D/         # Tridiagonal coefficient matrix and RHS builders
├── LinearSolvers/             # Direct linear algebra engines (Thomas TDMA, Gauss)
├── main/                      # Application entry points for each discrete solver
├── Stability/                 # Mathematical stability verifiers (CFL / diffusion number)
├── Utilities/                 # Console reporters, live plotters, and keyboard monitors
├── Verification/              # Analytical ground-truth solver & norm calculators
└── Makefile                   # Production GNU Makefile with modular build targets
```
---

## Prerequisites & Building

### Requirements
- **Compiler**: Modern C++ compiler with full **C++17** support (`g++` $\ge 9.0$ or `clang++` $\ge 10.0$)
- **Build System**: GNU Make
- **Debugger & Tools** (Recommended): GDB, VS Code C/C++ Extension Pack
- **Visualization** (Optional): `gnuplot` for rendering output data curves

### Compilation Workflow

The project uses a unified, dependency-aware GNU `Makefile`. To build the binaries, open your terminal in the repository root:

#### 1. Compile All Solvers Simultaneously:
```bash
make -j$(nproc)
```

#### 2. Compile Individual Target Schemes:
```bash
# Compile Forward-Time Central-Space explicit solver
make FTCS

# Compile DuFort-Frankel explicit solver
make DUFORT

# Compile Laasonen (BTCS) implicit solver
make LAASONEN

# Compile Crank-Nicolson implicit solver
make CRANK
```

#### 3. Clean Build Artifacts:
```bash
make clean
```

---
## Running the Solvers

Once compiled, executable binaries are generated inside the `bin/` directory. Run any scheme directly from the project root:

### 1. Execute an Explicit Scheme
```bash
# Run Forward-Time Central-Space solver
./bin/FTCS

# Run DuFort-Frankel solver
./bin/DUFORT
```

### 2. Execute an Implicit Scheme
```bash
# Run Laasonen (BTCS) solver
./bin/LAASONEN

# Run Crank-Nicolson solver
./bin/CRANK
```

### 3. Simulation Workflow & Real-Time Tracking
During execution, each solver performs the following pipeline automatically:
1. **Grid & Parameter Setup**: Instantiates `Mesh1D`, assigns physical properties ($\nu, L$), and evaluates the mesh Fourier number ($d$).
2. **Stability & Pre-flight Check**: Validates the numerical configuration against analytical stability boundaries.
3. **Time-Marching Loop**: Advances the scalar field $u(y, t)$ across time levels with active convergence/residual tracking.
4. **Analytical Verification**: Computes the exact solution at the target physical time and reports discrete error norms ($L_1, L_2, L_\infty$).
5. **Data Export**: Writes final profile distributions and time-history files to formatted output paths for post-processing and Gnuplot rendering.

---

## Key Features

- **Standardized C++17 Implementation**: Engineered using modern idioms, zero-overhead abstractions, and standard containers (`std::vector`, `std::array`) to ensure strict type safety, clean memory management, and deterministic performance.
- **Cache-Conscious Memory Layout**: Contiguous 1D buffer allocations maximize L1/L2 data cache hit rates, eliminating pointer indirection and heap fragmentation during time-marching loops.
- **Integrated Verification Engine**: Built-in Fourier series analytical solver calculates exact continuous solutions on-the-fly, enabling automated $L_2$ and $L_\infty$ error norm computations.
- **Rigorous Numerical Diagnostics**: Automated stability validation enforces Von Neumann / CFL diffusion number thresholds ($d \le 0.5$ for FTCS; conditional stability criteria for DuFort-Frankel), preventing unphysical oscillations and numerical blow-ups before execution.
- **Decoupled OOP Architecture**: Clean separation of concerns isolating grid management, physical properties, boundary conditions, time-stepping loops, and linear algebra solvers (Thomas Algorithm / TDMA).
- **Automated Output Serialization**: Export modules structure field data into clean, delimited CSV/DAT formats pre-configured for automated plotting with Gnuplot, Python/Matplotlib, or Paraview.
- **Modular Parallel Build System**: Robust `Makefile` supporting parallel multi-target compilation (`make -j$(nproc)`), incremental builds, dependency tracking, and aggressive compiler optimizations (`-O3 -march=native`).

---