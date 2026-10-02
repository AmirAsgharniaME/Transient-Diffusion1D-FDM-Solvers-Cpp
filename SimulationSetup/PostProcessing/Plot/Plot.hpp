#pragma once
#include "IO/Visualization/LivePointPlotter/LivePointPlotter.hpp"
#include "IO/Visualization/Plotter/Plotter.hpp"

class Plot
{
    public:
    LivePointPlotter LiveWindow;
    Plotter Window;


       explicit Plot()
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
                LiveWindow.AddSeries(
                    "relative field change",
                    "Relative Field Change");
                    
        //         Window.AddPlot(
        //         Path::Create(OutputCategory::Initial),
        //         FileName::Create(FieldName::U,0.0),
        //         Plotter::LineStyle::Solid, 
        //         2.5, 
        //         Legend::Create(FieldName::U,0.0)
        //    );
            }



};