#pragma once
#include "Utilities/LivePointPlotter/LivePointPlotter.hpp"
#include "Core1D/PostProcessing1D/OutputPlotter1D.hpp"
#include "Config/PathManager.hpp"
#include "Config/FileNameCreator.hpp"
#include "Config/LegendCreator.hpp"
#include "Config/FieldName.hpp"

struct Plotter_Setup1D
{

    //Live Plotter Initialization
    LivePointPlotter LiveWindow;
    OutputPlotter1D Window;


            Plotter_Setup1D()
            :LiveWindow(
            "Solver Convergence",
            "Iteration",
            "Rate of Field Change",
            true), //Logarithmic Y axis
            Window(
            "Velocity Comparison",
            "y",
            "Velocity",
            false,  // Linear x-axis
            false   // Linear y-axis
            )
            {
                LiveWindow.AddSeries("rate_of_field_change", "Rate of Field Change");
                Window.AddPlot(
                PathManager::GetOutputPath(OutputCategory::Initial),
                NameCreator::CreateName(FieldName::U,0.0), 
                OutputPlotter1D::LineStyle::Solid, 
                2.5, 
                LegendCreator::CreatePlotLegend(FieldName::U,0.0)
    );

            }


};