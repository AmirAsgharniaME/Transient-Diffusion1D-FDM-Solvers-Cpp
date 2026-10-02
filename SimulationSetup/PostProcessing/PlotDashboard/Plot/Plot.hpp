#pragma once
#include "IO/Visualization/gnuplot/LivePointPlotter/LivePointPlotter.hpp"
#include "IO/Visualization/gnuplot/LinePlot/LinePlot.hpp"

class Plot
{
    public:
    LivePointPlotter LiveWindow;
    LinePlot Window;


    explicit Plot();



};
