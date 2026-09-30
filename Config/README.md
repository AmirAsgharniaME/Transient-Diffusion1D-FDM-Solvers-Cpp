# Config

`Config/` is the project's shared configuration and vocabulary layer. It defines default simulation inputs, enum types used across the codebase, and small helpers for converting those types into filenames, plot labels, and output directories.

The folder is header-only: its files contain constants, enums, and inline helper functions. It does not implement the numerical methods, own mesh or field data, or write output files. Instead, other parts of the program read values and types from `Config/` and perform those jobs themselves.

## Simulation inputs

`SolverInputs.hpp` groups the default inputs by purpose under the `SolverInputs` namespace:

| Group | Values | Role |
|---|---|---|
| `Geometry` | `Length` | Length of the one-dimensional physical domain. |
| `Physics` | `nu` | Diffusion coefficient used by the model. |
| `Solver` | `NumTimeLevels`, `dt`, `Tolerance` | Time-marching limit, time-step size, and convergence tolerance. |
| `Mesh` | `N` | Number of spatial nodes. |
| `InitialCondition` | `Profile` | Initial field values, sized using `Mesh::N`. |
| `BoundaryCondition` | `TopWallValue`, `BottomWallValue` | Values prescribed at the two boundaries. |

Most scalar inputs are `inline constexpr`, so they are compile-time defaults included in the program. The initial profile is an inline constant vector. This file is the central place to adjust the default numerical case; it is not a parser for user-edited runtime configuration files.

`TimeScale.hpp` provides `TimeScale::Return(Lx, alpha)`, a small utility that computes the diffusion characteristic time $L_x^2/\alpha$. It accepts its length and diffusivity as arguments rather than reading them directly from `SolverInputs`.

## Shared enums and identifiers

These headers define common, strongly typed choices so code can pass categories and identifiers instead of relying on arbitrary strings or integers:

| Header | Types or values it defines | Purpose |
|---|---|---|
| `BoundaryManager.hpp` | `BoundaryType`: `Dirichlet`, `Neumann`, `Robin`; `BoundaryLocation`: `Top`, `Bottom`, `Right`, `Left` | Describes a boundary-condition kind and the boundary's location. |
| `Orientation.hpp` | `Orientation`: `Horizontal`, `Vertical` | Describes the orientation assigned to a one-dimensional line. |
| `FieldName.hpp` | `FieldName`: `U`, `UAnalytical` | Identifies numerical and analytical fields; `To_String_View()` supplies their text names. |
| `Label.hpp` | `Label`: `Steady_State` | Supplies a named label for outputs that are not identified by a time value; also has `To_String_View()`. |
| `SolverScheme.hpp` | `SolverScheme`: `FTCS`, `DUFORT_FRANKEL`, `Laasonen`, `CrankNicolson` | Identifies a numerical scheme; `To_String()` converts a scheme to text. |
| `PathType.hpp` | `OutputCategory`: `Initial`, `Numerical`, `Analytical` | Identifies the kind of result being stored. |

These types form shared vocabulary for solver setup and post-processing. For example, a field name identifies what data is being saved, while an output category and scheme identify where the data belongs.

## Filename, legend, and directory helpers

`FileName.hpp` and `Legend.hpp` both combine a `FieldName` with either a time value or a `Label`, but they serve different output purposes:

- `FileName::Create()` returns a filename stem, such as `U_0.003` or `U_Steady_State`. The file writer can append its file extension and save it in the selected directory.
- `Legend::Create()` returns text intended for a plot legend, such as `U (t = 0.003 s)` or `U (Steady_State)`.

`PathManager.hpp` provides `Path::Create(OutputCategory, SolverScheme)`. It maps initial and analytical categories to their result directories, and places numerical output in a scheme-specific subdirectory. If no scheme is supplied, it uses `SolverScheme::FTCS` by default.

A typical output-selection flow is:

1. Choose a field identifier and an output category.
2. Use `FileName::Create()` for the data file's name and `Path::Create()` for its directory.
3. For numerical output, also provide the active `SolverScheme` so results from different methods remain in separate directories.
4. Use `Legend::Create()` when the same field and time or label need a readable plot description.

## Architectural boundary

`Config/` describes values and choices shared by the application. The code that applies boundary conditions, generates the mesh, advances a solution, writes files, and plots results lives outside this folder. This separation lets those components use the same configuration types and naming rules without placing solver algorithms in the configuration layer.
