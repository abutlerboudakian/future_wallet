#ifndef CHARTTEMPLATE_H
#define CHARTTEMPLATE_H

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

#include "types.hpp"

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
    virtual void make(const ChartMap * data) final;

private:
    // Placeholder
    virtual void setDataAndEffects(const ChartMap * data) = 0;
};

class BarGUI : public ChartTemplate {
    /* Derived template class for making a bar graph */
public:
    BarGUI();
    ~BarGUI();

private:
    void setDataAndEffects(const ChartMap * data) override;
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
    void setDataAndEffects(const ChartMap * data) override;
    void QAxisSetup() override;
};

class LineGUI : public VisualTemplate {
public:
    LineGUI();
    ~LineGUI();
    void make(const LineMap * data);

private:
    void setDataAndEffects(const LineMap * data);
    void QAxisSetup() override;

    int largestX, largestY; // Keep track of the series index with the largest timestamp interval
                            // and largest value
};



#endif // CHARTTEMPLATE_H
