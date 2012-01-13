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
    MatrixXf *mOriginalData;
    MatrixXf *mProcessedData;
    MatrixXf *mPrincipalComponents;
    QGraphicsScene *mScene;
    Plot *mPlot;
    int mSliderMin;
    int mSliderMax;
    PcaSolver *mPcaSolver;
    double mMinCorrelation;
};

#endif // MAINWINDOW_H
