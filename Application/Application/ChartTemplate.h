#ifndef CHARTTEMPLATE_H
#define CHARTTEMPLATE_H

#include <string>
#include <unordered_map>

#include <QString>

#include <QChart>
#include <QChartView>
#include <QAbstractSeries>

#include <QBarSeries>
#include <QBarCategoryAxis>

#include <QPieSeries>

QT_CHARTS_USE_NAMESPACE

class ChartTemplate
{
public:
    ChartTemplate();
    virtual ~ChartTemplate();
    virtual void initialize(std::unordered_map<std::string, double> * data) final;
    virtual QChartView * getView() const;

protected:
    // Placeholder
    virtual QAbstractSeries * setDataAndEffects(std::unordered_map<std::string, double> * data) = 0;

    // Common
    virtual void QChartSetup();
    virtual void CreateChartView();

    // Hook
    virtual void QAxisSetup();
    virtual void Legend();

    // Data
    QChart * chart;
    QAbstractSeries * series;
    QChartView * chartView;
};

/*class BarGUI : public ChartTemplate {
public:
    BarGUI();
    ~BarGUI();

private:
    QAbstractSeries * setDataAndEffects(std::unordered_map<std::string, double> * data) override;
    void QAxisSetup() override;
}; */

class LineGUI : public ChartTemplate {
public:
    LineGUI();
    ~LineGUI();

private:
    QAbstractSeries * setDataAndEffects(std::unordered_map<std::string, double> * data) override;
    void QAxisSetup() override;
    void Legend() override;
};

class PieGUI : public ChartTemplate {
public:
    PieGUI();
    ~PieGUI();

private:
    QAbstractSeries * setDataAndEffects(std::unordered_map<std::string, double> * data) override;
};

#endif // CHARTTEMPLATE_H
