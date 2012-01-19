#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>
#include <algorithm>
#include "Eigen/Dense"
#include "plot.h"
#include "pcasolver.h"

using namespace Eigen;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void loadDataClicked();
    void saveDataClicked();
    void minCorrelationChanged(int value);


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void getPlotLabels();
    void updatePlot(std::vector<double> values);

    Ui::MainWindow *ui;
    QGraphicsScene *mScene;
    Plot *mPlot;
    PcaSolver *mPcaSolver;
    int mSliderMin;
    int mSliderMax;
    double mMinVariance;
    bool mRowsAreFeatures;
};

#endif // MAINWINDOW_H
