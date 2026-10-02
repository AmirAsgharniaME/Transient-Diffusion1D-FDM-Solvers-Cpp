#include "SimulationSetup/PostProcessing/PlotDashboard/Plot/Plot.hpp"

Plot::Plot()
    : LiveWindow(
          "Solver Convergence",
          "Iteration",
          "Rate of Field Change",
          true),
      Window(
          "Velocity Comparison",
          "y",
          "Velocity",
          false,
          false)
{
    LiveWindow.AddSeries(
        "relative field change",
        "Relative Field Change");

    // Window.AddPlot(
    //     Path::Create(OutputCategory::Initial),
    //     FileName::Create(FieldName::U, 0.0),
    //     LinePlot::LineStyle::Solid,
    //     2.5,
    //     Legend::Create(FieldName::U, 0.0));
}
