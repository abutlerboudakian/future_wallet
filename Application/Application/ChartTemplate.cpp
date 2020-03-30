#include "ChartTemplate.h"

ChartTemplate::ChartTemplate()
{
    chart = nullptr;
    series = nullptr;
    chartView = nullptr;
}
ChartTemplate::~ChartTemplate()
{
    delete chartView;
}

/* TemplateMethod for the class
 * (1) Gets the series for the class
 * (2) Creates the chart model
 * (3) Initializes the axes for that chart
 * (4) Creates the legend (optional)
 *
 * @modifies this->series
 * @modifies this->chart
 * @effects this->series is now initialized to the appropriate concrete series
 * @effects this->chart is now initialized as the chart model
 */
void ChartTemplate::initialize(std::unordered_map<std::string, double> * data)
{
    if (chartView != nullptr) { delete chartView; }
    else if (chart != nullptr) { delete chart; chart = nullptr; }
    else if (series != nullptr) { delete series; }
    chartView = nullptr; chart = nullptr; series = nullptr;
    series = setDataAndEffects(data);
    QChartSetup();
    QAxisSetup();
    Legend();
    CreateChartView();
}

// Common
/* Function creates the chart model, populated with the series
 * @requires this->series is initialized and populated with the
 *           data for a concrete series class
 * @modifies this->chart
 * @effects this->chart is now initialized as the chart model
 */
void ChartTemplate::QChartSetup()
{
  chart = new QChart();
  chart->addSeries(series);
  chart->setTitle("Whatever you want to name the chart");
  chart->setAnimationOptions(QChart::AllAnimations);
}

/* Function creates the Chart View object for the given chart model
 * @requires this->chart is initialized
 * @modifies this->chartView
 * @effects this->chartView is initialized to this->chart
 */
void ChartTemplate::CreateChartView()
{
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

/* Function returns the QChartView widget
 * @requires this->chartView is initialized
 * @return chartView
 */
QChartView * ChartTemplate::getView() const
{
    // Rep invariant, but idk how to fix
    return chartView;
}

// Hook
void ChartTemplate::Legend()
{
    chart->legend()->setVisible(true);
}

void ChartTemplate::QAxisSetup()
{
    return;
}

//------------------------------------------------------

/*QAbstractSeries * BarGUI::setDataAndEffects(std::unordered_map<std::string, double> * data)
{

}

void BarGUI::QAxisSetup()
{

}

//------------------------------------------------------

QAbstractSeries * LineGUI::setDataAndEffects(std::unordered_map<std::string, double> * data)
{

}

void LineGUI::QAxisSetup()
{

}

void LineGUI::Legend()
{

}*/


//------------------------------------------------------
PieGUI::PieGUI() : ChartTemplate() {}
PieGUI::~PieGUI() {}

/* Function creates a QPieSeries out of the data provided
 * @param data is a hashmap of DataName -> float
 * @returns (QAbstractSeries*)QPieSeries representing the pie series model of the given data
 */
QAbstractSeries * PieGUI::setDataAndEffects(std::unordered_map<std::string, double> * data)
{
    QPieSeries * temp_series = new QPieSeries();

    for (std::unordered_map<std::string, double>::const_iterator i = data->begin(); i != data->end(); i++)
    {
        temp_series->append(QString::fromStdString(i->first), i->second);
    }

    temp_series->setLabelsVisible(true);
    return temp_series;
}
