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
const QChartView * VisualTemplate::getView() const
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
BarGUI::BarGUI() : ChartTemplate() { colNames = new QStringList; }
BarGUI::~BarGUI()
{
    delete colNames;
}

/* Function to take in the bargraph data
 * @requires data to be a ChartMap with std::string->double
 *                                 denoting BarName->Value
 * @param data is a ChartMap denoting BarName->Value pairs
 * @modifies this->series
 * @effects this->series[0] now contains a QBarSeries modeling the given data
 */
void BarGUI::setDataAndEffects(ChartMap * const data)
{
    if (!colNames->isEmpty())
    {
        colNames->clear();
    }

    QBarSet * DataSet = new QBarSet("Thing");
    for (ChartMap::const_iterator i = data->begin(); i != data->end(); i++)
    {
        DataSet->append(i->second); // Push back the bar value
        colNames->push_back(QString::fromStdString(i->first)); // Push back the name of the bar
    }

    QBarSeries * temp_series = new QBarSeries();
    temp_series->append(DataSet);

    series->push_back(temp_series);
}

/* Function to set up the Axes for a BarGraph
 * @modifies chart
 * @effect chart now has the XAxis and YAxis and title set
 */
void BarGUI::QAxisSetup()
{
    QBarCategoryAxis * XAxis = new QBarCategoryAxis();
    XAxis->append(*colNames);
    chart->addAxis(XAxis, Qt::AlignBottom);
    (*(series->begin()))->attachAxis(XAxis);

    QValueAxis * YAxis = new QValueAxis();
    //YAxis->setRange(0,maxY);
    chart->addAxis(YAxis, Qt::AlignLeft);
    (*(series->begin()))->attachAxis(YAxis);
}

/* Override to hide legend
 */
void BarGUI::Legend()
{
    chart->legend()->setVisible(false);
}

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
    QDateTime largestDateTime = QDateTime(QDate(1990, 1, 1), QTime(0, 0, 0));
    double largestValue = 0;
    int series_num = 0;

    QLineSeries * temp_series;
    for (LineMap::const_iterator line = data->begin(); line != data->end(); line++) // For each line
    {
        temp_series = new QLineSeries;
        // Add each point on the line
        LinePoints::const_iterator point;
        for (point = line->second.begin(); point != line->second.end(); point++)
        {
            if (point->second > largestValue)
            {   // Update largestY
                largestValue = point->second;
                largestY = series_num;
            }
            if (point->first > largestDateTime)
            {   // Update largestX
                largestDateTime = point->first;
                largestX = series_num;
            }
            temp_series->append(point->first.toMSecsSinceEpoch(), point->second);
        }
        series->push_back(temp_series); // Add the line to list of lines
        series_num++;
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
    (*series)[largestX]->attachAxis(XAxis);

    QValueAxis * YAxis = new QValueAxis;
    YAxis->setLabelFormat("%i");
    YAxis->setTitleText("Value");
    chart->addAxis(YAxis, Qt::AlignLeft);
    (*series)[largestY]->attachAxis(YAxis);
}
