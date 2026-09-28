#pragma once

#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>

class OutputPlotter1D {
public:
    enum class LineStyle {
        Solid,
        Dashed,
        Dotted,
        DashDotted
    };

    OutputPlotter1D(
        const std::string& title,
        const std::string& xLabel,
        const std::string& yLabel,
        bool logarithmicX = false,
        bool logarithmicY = false
    );

    ~OutputPlotter1D();

    OutputPlotter1D(const OutputPlotter1D&) = delete;
    OutputPlotter1D& operator=(const OutputPlotter1D&) = delete;

    void AddPlot(
        const std::filesystem::path& relativeDirectory,
        const std::string& fileName,
        LineStyle lineStyle,
        double lineWidth,
        const std::string& legend
    );

private:
    struct Plot {
        std::filesystem::path filePath;
        LineStyle lineStyle;
        double lineWidth;
        std::string legend;
    };

    FILE* gnuplotPipe = nullptr;
    std::string plotTitle;
    std::string xAxisLabel;
    std::string yAxisLabel;
    bool logX;
    bool logY;
    std::vector<Plot> plots;

    void configurePlot() const;
    void renderPlots() const;
};
