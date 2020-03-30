#include "ChartTemplate.h"

//------------------------------------------------------
// VisualTemplate                                      |
//------------------------------------------------------
VisualTemplate::VisualTemplate()
{
    chart = nullptr;
    series = new std::vector<QAbstractSeries *>;
    chartView = nullptr;
}

VisualTemplate::~VisualTemplate()
{
    delete series; // Check if this works
    delete chartView;
}

// Common
/* Function creates the chart model, populated with the series
 * @requires this->series is initialized and populated with the
 *           data for a concrete series class
 * @modifies this->chart
 * @effects this->chart is now initialized as the chart model
 */
void VisualTemplate::QChartSetup()
{
  chart = new QChart();
  for (std::vector<QAbstractSeries*>::const_iterator s = series->begin(); s != series->end(); s++)
  {
      chart->addSeries(*s);
  }
  chart->setTitle("Whatever you want to name the chart");
  chart->setAnimationOptions(QChart::AllAnimations);
}

/* Function creates the Chart View object for the given chart model
 * @requires this->chart is initialized
 * @modifies this->chartView
 * @effects this->chartView is initialized to this->chart
 */
void VisualTemplate::CreateChartView()
{
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
}

/* Function returns the QChartView widget
 * @requires this->chartView is initialized
 * @return chartView
 */
QChartView * VisualTemplate::getView() const
{
    // Rep invariant, but idk how to fix
    return chartView;
}

// Hook
/* Function to set legend attributes
 * @modifies chart->legend()
 * @effect chart->legend() is visible
 */
void VisualTemplate::Legend()
{
    chart->legend()->setVisible(true);
}

/* Function to create Axes
 * @modifies chart
 * @effect default axes are created for the chart
 */
void VisualTemplate::QAxisSetup()
{
    chart->createDefaultAxes();
}

//------------------------------------------------------
// ChartTemplate                                       |
//------------------------------------------------------
ChartTemplate::ChartTemplate() : VisualTemplate() {}
ChartTemplate::~ChartTemplate() {}

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
void ChartTemplate::initialize(ChartMap * data)
{
    if (chartView != nullptr) { delete chartView; }
    else if (chart != nullptr) { delete chart; chart = nullptr; }
    else if (!series->empty()) // Might be incorrect to free here, since chart frees it
    {
        for (std::vector<QAbstractSeries* >::iterator s = series->begin(); s != series->end(); s++)
        {
            delete *s;
        }
        series->empty();
    }
    chartView = nullptr; chart = nullptr;
    setDataAndEffects(data);
    QChartSetup();
    QAxisSetup();
    Legend();
    CreateChartView();
}

//------------------------------------------------------
// BarGUI                                              |
//------------------------------------------------------

/*void BarGUI::setDataAndEffects(ChartMap * const data)
{

}

void BarGUI::QAxisSetup()
{

} */


//------------------------------------------------------
// PieGUI                                              |
//------------------------------------------------------
PieGUI::PieGUI() : ChartTemplate() {}
PieGUI::~PieGUI() {}

/* Function creates a QPieSeries out of the data provided
 * @param data is a hashmap of std::string->double for DataName->PercentDecimal
 * @modifies this->series
 * @effects this->series to contain a PieSeries
 */
void PieGUI::setDataAndEffects(ChartMap * const data)
{
    QPieSeries * temp_series = new QPieSeries();

    for (ChartMap::const_iterator i = data->begin(); i != data->end(); i++)
    {
        temp_series->append(QString::fromStdString(i->first), i->second);
    }

    temp_series->setLabelsVisible(true);

    series->push_back(temp_series);
}

/* Override function since the pie graph doesn't use axes
 */
void PieGUI::QAxisSetup() { return; }

//------------------------------------------------------
// LineGUI                                             |
//------------------------------------------------------
LineGUI::LineGUI() : VisualTemplate() {}
LineGUI::~LineGUI() {}

/* Template method for the linegraph
 * (1) (1) Gets the series for the class
 * (2) Creates the chart model
 * (3) Initializes the axes for that chart
 * (4) Creates the legend (optional)
 *
 * @modifies this->series
 * @modifies this->chart
 * @effects this->series is now initialized to the appropriate concrete series
 * @effects this->chart is now initialized as the chart model
 */
void LineGUI::initialize(LineMap * const data)
{
    if (chartView != nullptr) { delete chartView; }
    else if (chart != nullptr) { delete chart; chart = nullptr; }
    else if (!series->empty()) // Might be incorrect to free here, since chart frees it
    {
        for (std::vector<QAbstractSeries* >::iterator s = series->begin(); s != series->end(); s++)
        {
            delete *s;
        }
        series->empty();
    }
    chartView = nullptr; chart = nullptr;
    setDataAndEffects(data);
    QChartSetup();
    QAxisSetup();
    Legend();
    CreateChartView();
}

/* Function creates a (QAbstractSeries*)QLineSeries representing the data provided, where
 *                     each pair represents a new line
 * @requires data to be an unorderedmap of std::string->std::vector<std::pair<QDateTime, double> >
 *           where std::string is the line name and std::vector contains points to graph
 *           X Axis is QDateTime, Y is double
 * @param data is an unordered map, representing the lines we need to graph
 * @modifies this->series
 * @effects this->series now contains a bunch of QLineSeries based on the data
 */
void LineGUI::setDataAndEffects(LineMap * const data)
{
    QLineSeries * temp_series;
    for (LineMap::const_iterator line = data->begin(); line != data->end(); line++) // For each line
    {
        temp_series = new QLineSeries;
        // Add each point on the line
        for (LinePoints::const_iterator point = line->second.begin(); point != line->second.end(); point++)
        {
            temp_series->append(point->first.toMSecsSinceEpoch(), point->second);
        }
        series->push_back(temp_series); // Add the line to list of lines
    }
}

/* Function to craete QDateTimeAxis
 * @modifies this->chart
 * @effect adds a QDateTimeAxis to this->chart
 */
void LineGUI::QAxisSetup()
{
    QDateTimeAxis * XAxis = new QDateTimeAxis;
    XAxis->setTickCount(10);
    XAxis->setFormat("MMM d yyyy");
    XAxis->setTitleText("Date");
    chart->addAxis(XAxis, Qt::AlignBottom);
    (*series)[0]->attachAxis(XAxis);

    QValueAxis * YAxis = new QValueAxis;
    YAxis->setLabelFormat("%i");
    YAxis->setTitleText("Value");
    chart->addAxis(YAxis, Qt::AlignLeft);
    (*series)[0]->attachAxis(YAxis);
}
