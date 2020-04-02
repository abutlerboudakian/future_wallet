#ifndef CHARTTEMPLATE_H
#define CHARTTEMPLATE_H

#include <string>
#include <unordered_map>
#include <vector>

#include <QString>

#include <QChart>
#include <QChartView>
#include <QAbstractSeries>

#include <QBarSeries>
#include <QBarSet>
#include <QStringList>
#include <QBarCategoryAxis>

#include <QPieSeries>

#include <QLineSeries>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>

#define ChartMap std::unordered_map<std::string, double>
#define LinePoints std::vector<std::pair<QDateTime, double> >
#define LineMap std::unordered_map<std::string, LinePoints>

QT_CHARTS_USE_NAMESPACE

class VisualTemplate
{
    /* The base template method class used for visual graphs */
public:
    VisualTemplate();
    virtual ~VisualTemplate();
    virtual QChartView * getView() const;

protected:
    // Common
    virtual void QChartSetup();
    virtual void CreateChartView();

    // Hook
    virtual void QAxisSetup();
    virtual void Legend();

    // Data
    QChart * chart;
    std::vector<QAbstractSeries *> * series;
    QChartView * chartView;

    // Helper function
    void clearSeries();
};

class ChartTemplate : public VisualTemplate
{
    /* Derived base template method class for
       visualizing data from a single timestamp */
public:
    ChartTemplate();
    virtual ~ChartTemplate();
    virtual void make(ChartMap * const data) final;

private:
    // Placeholder
    virtual void setDataAndEffects(ChartMap * data) = 0;
};

class BarGUI : public ChartTemplate {
    /* Derived template class for making a bar graph */
public:
    BarGUI();
    ~BarGUI();

private:
    void setDataAndEffects(ChartMap * const data) override;
    void QAxisSetup() override;
    void Legend() override;

    double maxY; // For the max in the range
    QStringList * colNames; // For the column names
};

class PieGUI : public ChartTemplate {
    /* Derived template class for making a pie graph */
public:
    PieGUI();
    ~PieGUI();

private:
    void setDataAndEffects(ChartMap * const data) override;
    void QAxisSetup() override;
};

class LineGUI : public VisualTemplate {
public:
    LineGUI();
    ~LineGUI();
    void make(LineMap * data);

private:
    void setDataAndEffects(LineMap * const data);
    void QAxisSetup() override;

    int largestX, largestY; // Keep track of the series index with the largest timestamp interval
                            // and largest value
};



#endif // CHARTTEMPLATE_H
