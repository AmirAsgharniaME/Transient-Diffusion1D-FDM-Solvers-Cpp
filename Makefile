# ==============================================================================
# Compiler and flags
# ==============================================================================

CXX := g++

CPPFLAGS := -I.
CXXFLAGS := -std=c++17 -Wall -Wextra -g

# Optional linker flags.
# For example, if your project needs a library:
# LDLIBS := -lsome_library
LDFLAGS :=
LDLIBS :=


# ==============================================================================
# Output directory
# ==============================================================================

BIN_DIR := bin


# ==============================================================================
# Config
# ==============================================================================

SOLVERINPUTS1D_HEADER := Config/SolverInputs1D.hpp
SOLVERSCHEME_HEADER  := Config/SolverScheme.hpp
FIELDNAME_HEADER     := Config/FieldName.hpp
PATHTYPE_HEADER      := Config/PathType.hpp
PATHMANAGER_HEADER   := Config/PathManager.hpp
FILENAMECREATOR_HEADER := Config/FileNameCreator.hpp
LEGENDCREATOR_HEADER  := Config/LegendCreator.hpp


CONFIG_HEADERS := \
	$(SOLVERINPUTS1D_HEADER) \
	$(SOLVERSCHEME_HEADER) \
	$(FIELDNAME_HEADER) \
	$(PATHTYPE_HEADER) \
	$(PATHMANAGER_HEADER) \
	$(FILENAMECREATOR_HEADER) \
	$(LEGENDCREATOR_HEADER)

# Currently,Config contains header-only components.
# If you add .cpp files later, define them here:
CONFIG_SRCS :=
#===================================================
# Core1D
# ==============================================================================

GEOMETRY1D_HEADER            := Core1D/Geometry1D/Geometry1D.hpp
MESH1D_HEADER                := Core1D/Mesh1D/Mesh1D.hpp
FIELD1D_HEADER               := Core1D/Field1D/Field1D.hpp
BOUNDARYPOINT_HEADER         := Core1D/BoundaryPoint/BoundaryPoint.hpp
FILEWRITER1D_HEADER          := Core1D/FileWriter1D/FileWriter1D.hpp
POSTPROCESSING1D_HEADER      := Core1D/PostProcessing1D/OutputPlotter1D.hpp
CONVERGENCE1D_HEADER         := Core1D/Convergence1D/Convergence1D.hpp
INITIALSETUP1D_HEADER        := Core1D/Initial_Setup1D.hpp
PLOTTERSETUP1D_HEADER        := Core1D/Plotter_Setup1D.hpp

CORE1D_HEADERS := \
	$(GEOMETRY1D_HEADER) \
	$(MESH1D_HEADER) \
	$(FIELD1D_HEADER) \
	$(BOUNDARYPOINT_HEADER) \
	$(FILEWRITER1D_HEADER) \
	$(POSTPROCESSING1D_HEADER) \
	$(CONVERGENCE1D_HEADER) \
	$(INITIALSETUP1D_HEADER) \
	$(PLOTTERSETUP1D_HEADER)


MESH1D_SRC                := Core1D/Mesh1D/Mesh1D.cpp
FIELD1D_SRC               := Core1D/Field1D/Field1D.cpp
FILEWRITER1D_SRC          := Core1D/FileWriter1D/FileWriter1D.cpp
POSTPROCESSING1D_SRC      := Core1D/PostProcessing1D/OutputPlotter1D.cpp
CONVERGENCE1D_SRC         := Core1D/Convergence1D/Convergence1D.cpp

CORE1D_SRCS := \
	$(MESH1D_SRC) \
	$(FIELD1D_SRC) \
	$(FILEWRITER1D_SRC) \
	$(POSTPROCESSING1D_SRC) \
	$(CONVERGENCE1D_SRC)


# ==============================================================================
# Utilities
# ==============================================================================

KEYBOARDHANDLER_HEADER := Utilities/KeyboardHandler/KeyboardHandler.hpp
PARAMETER_HEADER       := Utilities/Parameter/Parameter.hpp
STATUSPRINTER_HEADER   := Utilities/StatusPrinter/StatusPrinter.hpp
LIVEPOINTPLOTTER_HEADER := Utilities/LivePointPlotter/LivePointPlotter.hpp

UTILITY_HEADERS := \
	$(KEYBOARDHANDLER_HEADER) \
	$(PARAMETER_HEADER) \
	$(STATUSPRINTER_HEADER) \
	$(LIVEPOINTPLOTTER_HEADER)

LIVEPOINTPLOTTER_SRC := Utilities/LivePointPlotter/LivePointPlotter.cpp

UTILITY_SRCS := \
	$(LIVEPOINTPLOTTER_SRC)



# ==============================================================================
# Stability
# ==============================================================================
STABILIZATION_HEADER := Stability/FTCS/Stabilization.hpp

STABILITY_HEADERS := \
	$(STABILIZATION_HEADER)

# Currently,Stability contains header-only components.
# If you add .cpp files later, define them here:
STABILITY_SRCS :=
# ==============================================================================
# Linear Solvers
# ==============================================================================

GAUSSIANELIMINATION_HEADER := LinearSolvers/GaussianElimination/GaussianElimination.hpp
THOMASALGORITHM_HEADER     := LinearSolvers/ThomasAlgorithm/ThomasAlgorithm.hpp

LINEARSOLVER_HEADERS := \
	$(GAUSSIANELIMINATION_HEADER) \
	$(THOMASALGORITHM_HEADER)

GAUSSIANELIMINATION_SRC := LinearSolvers/GaussianElimination/GaussianElimination.cpp
THOMASALGORITHM_SRC     := LinearSolvers/ThomasAlgorithm/ThomasAlgorithm.cpp

LINEARSOLVER_SRCS := \
	$(GAUSSIANELIMINATION_SRC) \
	$(THOMASALGORITHM_SRC)

# ==============================================================================
# Explicit Solvers 1D
# ==============================================================================
EXPLICITSOLVERS1D_HEADER := ExplicitSolvers1D/ExplicitSolvers1D.hpp

EXPLICITSOLVERS1D_HEADERS := \
	$(EXPLICITSOLVERS1D_HEADER)

EXPLICITSOLVERS1D_SRC := ExplicitSolvers1D/ExplicitSolvers1D.cpp

EXPLICITSOLVERS1D_SRCS := \
	$(EXPLICITSOLVERS1D_SRC)
# ==============================================================================
# Implicit Solvers 1D
# ==============================================================================
COEFFICIENTMATRIX1D_HEADER := ImplicitSolvers1D/CoefficientMatrix1D/CoefficientMatrix1D.hpp
RHS1D_HEADER            := ImplicitSolvers1D/RHS1D/RHS1D.hpp
TRIDIAGONALMATRIX_HEADER := ImplicitSolvers1D/TridiagonalMatrix/TridiagonalMatrix.hpp

IMPLICITSOLVERS1D_HEADERS := \
	$(COEFFICIENTMATRIX1D_HEADER) \
	$(RHS1D_HEADER) \
	$(TRIDIAGONALMATRIX_HEADER)

COEFFICIENTMATRIX1D_SRC := ImplicitSolvers1D/CoefficientMatrix1D/CoefficientMatrix1D.cpp
RHS1D_SRC             := ImplicitSolvers1D/RHS1D/RHS1D.cpp
TRIDIAGONALMATRIX_SRC := ImplicitSolvers1D/TridiagonalMatrix/TridiagonalMatrix.cpp

IMPLICITSOLVERS1D_SRCS := \
	$(COEFFICIENTMATRIX1D_SRC) \
	$(RHS1D_SRC) \
	$(TRIDIAGONALMATRIX_SRC)

# ==============================================================================
# Linear Solvers
# ==============================================================================
GAUSSIANELIMINATION_HEADER := LinearSolvers/GaussianElimination/GaussianElimination.hpp
THOMASALGORITHM_HEADER     := LinearSolvers/ThomasAlgorithm/ThomasAlgorithm.hpp

LINEARSOLVER_HEADERS := \
	$(GAUSSIANELIMINATION_HEADER) \
	$(THOMASALGORITHM_HEADER)

GAUSSIANELIMINATION_SRC := LinearSolvers/GaussianElimination/GaussianElimination.cpp
THOMASALGORITHM_SRC     := LinearSolvers/ThomasAlgorithm/ThomasAlgorithm.cpp

LINEARSOLVER_SRCS := \
	$(GAUSSIANELIMINATION_SRC) \
	$(THOMASALGORITHM_SRC)
# ==============================================================================
# Verification
# ==============================================================================
VERIFICATION_HEADER := Verification/AnalyticalDiffusion1D/AnalyticalDiffusion1D.hpp

VERIFICATION_HEADERS := \
	$(VERIFICATION_HEADER)

VERIFICATION_SRC := Verification/AnalyticalDiffusion1D/AnalyticalDiffusion1D.cpp

VERIFICATION_SRCS := \
	$(VERIFICATION_SRC)
# ==============================================================================

# ==============================================================================
# Common project files
# ==============================================================================

COMMON_HEADERS := \
	$(CONFIG_HEADERS) \
	$(CORE1D_HEADERS) \
	$(LIVEPLOTTER_HEADERS) \
	$(UTILITY_HEADERS) \
	$(VERIFICATION_HEADERS)


COMMON_SRCS := \
	$(CONFIG_SRCS) \
	$(CORE1D_SRCS) \
	$(LIVEPLOTTER_SRCS) \
	$(UTILITY_SRCS) \
	$(VERIFICATION_SRCS)


# ==============================================================================
# Programs
# ==============================================================================

PROGRAMS := FTCS DUFORT LAASONEN CRANK


# ------------------------------------------------------------------------------
# FTCS
# ------------------------------------------------------------------------------

FTCS_SRC := main/Explicit_FTCS.cpp

FTCS_EXTRA_SRCS := $(EXPLICITSOLVERS1D_SRCS)
FTCS_EXTRA_HEADERS := $(STABILIZATION_HEADERS) $(EXPLICITSOLVERS1D_HEADERS)

FTCS_EXE := $(BIN_DIR)/Explicit_FTCS


# ------------------------------------------------------------------------------
# Dufort-Frankel
# ------------------------------------------------------------------------------

DUFORT_SRC := main/Explicit_DUFORT_FRANKEL.cpp

DUFORT_EXTRA_SRCS := $(EXPLICITSOLVERS1D_SRCS)
DUFORT_EXTRA_HEADERS := $(STABILIZATION_HEADERS) $(EXPLICITSOLVERS1D_HEADERS)

DUFORT_EXE := $(BIN_DIR)/Explicit_DUFORT_FRANKEL


# ------------------------------------------------------------------------------
# Laasonen
# ------------------------------------------------------------------------------

LAASONEN_SRC := main/Implicit_Laasonen.cpp

LAASONEN_EXTRA_SRCS := $(IMPLICITSOLVERS1D_SRCS) $(LINEARSOLVER_SRCS)
LAASONEN_EXTRA_HEADERS := $(IMPLICITSOLVERS1D_HEADERS) $(LINEARSOLVER_HEADERS)

LAASONEN_EXE := $(BIN_DIR)/Implicit_Laasonen


# ------------------------------------------------------------------------------
# Crank-Nicolson
# ------------------------------------------------------------------------------

CRANK_SRC := main/Implicit_CrankNicolson.cpp

CRANK_EXTRA_SRCS := $(IMPLICITSOLVERS1D_SRCS) $(LINEARSOLVER_SRCS)
CRANK_EXTRA_HEADERS := $(IMPLICITSOLVERS1D_HEADERS) $(LINEARSOLVER_HEADERS)

CRANK_EXE := $(BIN_DIR)/Implicit_CrankNicolson


# ==============================================================================
# Aggregate targets
# ==============================================================================

ALL_EXES := \
	$(FTCS_EXE) \
	$(DUFORT_EXE) \
	$(LAASONEN_EXE) \
	$(CRANK_EXE)


.PHONY: all ftcs dufort laasonen crank clean rebuild help

all: $(ALL_EXES)


# ==============================================================================
# Output directory
# ==============================================================================

$(BIN_DIR):
	mkdir -p $@


# ==============================================================================
# Build rules
# ==============================================================================

$(FTCS_EXE): $(FTCS_SRC) $(COMMON_SRCS) $(COMMON_HEADERS) $(FTCS_EXTRA_SRCS) $(FTCS_EXTRA_HEADERS) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ \
		$(FTCS_SRC) \
		$(COMMON_SRCS) \
		$(FTCS_EXTRA_SRCS) \
		$(LDLIBS)


$(DUFORT_EXE): $(DUFORT_SRC) $(COMMON_SRCS) $(COMMON_HEADERS) $(DUFORT_EXTRA_SRCS) $(DUFORT_EXTRA_HEADERS) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ \
		$(DUFORT_SRC) \
		$(COMMON_SRCS) \
		$(DUFORT_EXTRA_SRCS) \
		$(LDLIBS)


$(LAASONEN_EXE): $(LAASONEN_SRC) $(COMMON_SRCS) $(COMMON_HEADERS) $(LAASONEN_EXTRA_SRCS) $(LAASONEN_EXTRA_HEADERS) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ \
		$(LAASONEN_SRC) \
		$(COMMON_SRCS) \
		$(LAASONEN_EXTRA_SRCS) \
		$(LDLIBS)



$(CRANK_EXE): $(CRANK_SRC) $(COMMON_SRCS) $(COMMON_HEADERS) $(CRANK_EXTRA_SRCS) $(CRANK_EXTRA_HEADERS) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ \
		$(CRANK_SRC) \
		$(COMMON_SRCS) \
		$(CRANK_EXTRA_SRCS) \
		$(LDLIBS)



# ==============================================================================
# Individual targets
# ==============================================================================

ftcs: $(FTCS_EXE)

dufort: $(DUFORT_EXE)

laasonen: $(LAASONEN_EXE)

crank: $(CRANK_EXE)


# ==============================================================================
# Cleaning
# ==============================================================================

clean:
	rm -f $(ALL_EXES)


rebuild: clean all


# ==============================================================================
# Help
# ==============================================================================

help:
	@echo "Available targets:"
	@echo "  make              Build all programs"
	@echo "  make all          Build all programs"
	@echo "  make ftcs         Build Explicit_FTCS"
	@echo "  make dufort       Build Explicit_DUFORT_FRANKEL"
	@echo "  make laasonen     Build Implicit_Laasonen"
	@echo "  make crank        Build Implicit_CrankNicolson"
	@echo "  make clean        Remove all executables"
	@echo "  make rebuild      Clean and build all programs"