#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <vector>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace Eigen;

void MainWindow::loadDataClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Data File"), "", "");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QTextStream inStream(&file);

    QList<QStringList> rowList;
    while (! inStream.atEnd()) {
        QString line = inStream.readLine();
        QStringList dataRow = line.split(",");
        rowList.append(dataRow);
    }

    // TODO error checking
    int nRows = rowList.size();
    int nColumns = rowList.at(0).size();

    mOriginalData = new MatrixXf(nRows, nColumns);
    for (int i = 0; i < rowList.size(); i++) {
        QStringList row = rowList[i];
        assert(row.size() == nColumns);
        for (int j = 0; j < row.size(); j++) {
            (*mOriginalData)(i, j) = row.at(j).toDouble();
        }
    }

    std::cout << *mOriginalData << std::endl;
    mRowsAreFeatures = ui->rowsCheckBox->isChecked();
    if (!mRowsAreFeatures) {
        MatrixXf *tempMatrix = new MatrixXf(nColumns, nRows);
        *tempMatrix = mOriginalData->transpose();
        delete mOriginalData;
        mOriginalData = tempMatrix;
        std::cout << *mOriginalData << std::endl;
    }
    file.close();

    mPcaSolver->calculateEigen(*mOriginalData);
    VectorXf eigenvalues = mPcaSolver->getAbsEigenvalues();

    std::vector<double> eigenvaluesStdVector;
    for (int i = 0; i < eigenvalues.size(); i++) {
            eigenvaluesStdVector.push_back(eigenvalues(i));
    }
    updatePlot(eigenvaluesStdVector);

    ui->minValueSlider->setEnabled(true);
    ui->saveDataButton->setEnabled(true);
}

void MainWindow::saveDataClicked()
{
    mPcaSolver->performPcaUsingEigenvectors();
    MatrixXf reducedDataT = mPcaSolver->getReducedData();
    MatrixXf *reducedData;
    if (!mRowsAreFeatures) {
        reducedData = new MatrixXf(reducedDataT.cols(), reducedDataT.rows());
        *reducedData = reducedDataT.transpose();
    } else {
        reducedData = new MatrixXf(reducedDataT.rows(), reducedDataT.cols());
        *reducedData = reducedDataT;
    }


    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"), "", "");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        return;
    QTextStream outStream(&file);


    for (int i = 0; i < reducedData->rows(); i++) {
        for (int j = 0; j < reducedData->cols() - 1; j++) {
            outStream << (*reducedData)(i, j) << ",";
        }
        outStream << (*reducedData)(i, reducedData->cols() - 1) << '\n';
    }
    delete reducedData;
}

void MainWindow::getPlotLabels()
{
    ui->minXLabel->setNum(mPlot->getMinX() + 1);
    ui->maxXLabel->setNum(mPlot->getMaxX() + 1);
    double minVal = ((double)((int)mPlot->getMinY() * 10000))/10000;
    ui->minYLabel->setNum(minVal);
    ui->maxYLabel->setNum(mPlot->getMaxY());
}

void MainWindow::updatePlot(std::vector<double> values)
{
    mPlot->clear();
    mPlot->importDoubleValues(values);
    getPlotLabels();
    mScene->addPixmap(mPlot->getPixmap());
    ui->graphicsView->setScene(mScene);
    ui->graphicsView->show();
}

void MainWindow::minCorrelationChanged(int value)
{
    double plotMax = mPlot->getMaxY();
    double plotMin = mPlot->getMinY();
    double shift = plotMin;
    double scale = (plotMax - plotMin) / (mSliderMax - mSliderMin);
    mMinCovariance = (static_cast<double>(value) * scale) + shift;

    ui->minValueLabel->setNum(mMinCovariance);
    mPcaSolver->setMinAbsCovariance(mMinCovariance);

    mPlot->putHorizontalMark(mMinCovariance, qRgb(255,0 ,0));
    mScene->addPixmap(mPlot->getPixmap());
    ui->graphicsView->setScene(mScene);
    ui->graphicsView->show();

    ui->relevantFeaturesLabel->setNum(mPlot->countPointsOverMark());
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mSliderMin = 0; // do not change (SetMinValueLabel)
    mSliderMax = 999;

    ui->setupUi(this);
    ui->minValueSlider->setMinimum(mSliderMin);
    ui->minValueSlider->setMaximum(mSliderMax);
    setWindowTitle("PCA - gui");
    mScene = new QGraphicsScene();

    ui->minValueLabel->setNum(ui->minValueSlider->value());
    connect(ui->minValueSlider, SIGNAL(valueChanged(int)), this, SLOT(minCorrelationChanged(int)));
    connect(ui->loadDataButton, SIGNAL(clicked()), this, SLOT(loadDataClicked()));
    connect(ui->saveDataButton, SIGNAL(clicked()), this, SLOT(saveDataClicked()));

    mPlot = new Plot(ui->graphicsView->width() - 2, ui->graphicsView->height() - 2);

    mScene->addPixmap(mPlot->getPixmap());
    ui->graphicsView->setScene(mScene);
    ui->graphicsView->show();

    bool pcaSolverDebugMode = false;
    mPcaSolver = new PcaSolver(pcaSolverDebugMode);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mScene;
    delete mPlot;
    delete mPcaSolver;
}
