#ifndef PLOT_H
#define PLOT_H

#include <QImage>
#include <QPixmap>
#include <vector>
#include <iostream>

class Plot
{
public:
    Plot(int pixWidth, int pixHeight);
    ~Plot();
    void importDoubleValues(const std::vector<double> &values);
    void putPixel(int x, int y, QRgb color);
    void putCross(int x, int y, QRgb color);
    int valueToPositionX(double x);
    int valueToPositionY(double y);
    QPixmap getPixmap();
    void putHorizontalMark(double y, QRgb color);
    int countPointsOverMark();
    void clear();

    double getMinX() {
        return mMinX;
    }

    double getMaxX() {
        return mMaxX;
    }

    double getMinY() {
        return mMinY;
    }

    double getMaxY() {
        return mMaxY;
    }

protected:
    QImage *mImage;

    double mMinX;
    double mMaxX;

    double mMinY;
    double mMaxY;

    int mWidth;
    int mHeight;

    double mShiftX;
    double mScaleX;        // pixX = (dataX + mShiftX) * mScaleX;
    double mShiftY;
    double mScaleY;

    int mPixBorderX;
    int mPixBorderY;

    QRgb mPlotColor;
    QRgb mBackgroundColor;

    std::vector<QRgb> mUndoHorizontalMarkPixels;
    int mUndoHorizontalMarkY;
    double mHorizontalMarkValue;

    std::vector<double> mValues;
};

#endif // PLOT_H
