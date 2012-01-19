#include "plot.h"

Plot::Plot(int pixWidth, int pixHeight)
{
    mWidth = pixWidth;
    mHeight = pixHeight;
    mPixBorderX = 1;
    mPixBorderY = 1;
    mImage = new QImage(mWidth, mHeight, QImage::Format_RGB32);
    mBackgroundColor = qRgb(255, 255, 255);
    mImage->fill(mBackgroundColor);
    mPlotColor = qRgb(0, 0, 255);
}

Plot::~Plot() {
    delete mImage;
}

void Plot::clear() {
    mImage->fill(mBackgroundColor);
    mUndoHorizontalMarkPixels.clear();
}

QPixmap Plot::getPixmap()
{
    return QPixmap::fromImage(*mImage);
}

void Plot::putHorizontalMark(double y, QRgb color)
{
    // undo previous line
    if (mUndoHorizontalMarkPixels.size() != 0) {
        for (unsigned int i = 0; i < mUndoHorizontalMarkPixels.size(); i++) {
            mImage->setPixel(i, mUndoHorizontalMarkY, mUndoHorizontalMarkPixels[i]);
        }
        mUndoHorizontalMarkPixels.clear();
    }
    // put new line, save undo information
    int plotY = valueToPositionY(y);
    mUndoHorizontalMarkY = plotY;
    mHorizontalMarkValue = y;
    for (int i = 0; i < mWidth; i++) {
        mUndoHorizontalMarkPixels.push_back(mImage->pixel(i, plotY));
        mImage->setPixel(i, plotY, color);
    }
}

int Plot::countPointsOverMark()
{
    int count = 0;
    for (unsigned int i = 0; i < mValues.size(); i++) {
        if (mValues[i] >= mHorizontalMarkValue) {
            count++;
        }
    }
    return count;
}

void Plot::importDoubleValues(const std::vector<double> &values)
{
    mMinX = 0;
    mMaxX = values.size() - 1;
    mValues = values;
/*

    for (int i = 0; i < values.size(); i++) {
        std::cout << values[i] << " ";
    }
    std::cout << std::endl;

*/
    mMinY = mValues[0];
    mMaxY = mValues[0];
    for (unsigned int i = 1; i < mValues.size(); i++) {
        if (mValues[i] > mMaxY) {
            mMaxY = mValues[i];
        }
        if (mValues[i] < mMinY) {
            mMinY = mValues[i];
        }
    }

    mShiftX = -mMinX;
    mShiftY = -mMinY;

    mScaleX = static_cast<double>(mWidth - 1 - 2 * mPixBorderX) / (mMaxX - mMinX);
    mScaleY = static_cast<double>(mHeight - 1 - 2 * mPixBorderY) / (mMaxY - mMinY);
    std::cout << "shift x: " << mShiftX << " scale x: " << mScaleX << std::endl;
    std::cout << "shift y: " << mShiftY << " scale y: " << mScaleY << std::endl;

    for (unsigned int i = 0; i < mValues.size(); i++) {
        int pixX = valueToPositionX(i);
        int pixY = valueToPositionY(mValues[i]);
        putCross(pixX, pixY, mPlotColor);
    }
}

int Plot::valueToPositionX(double x)
{
    return static_cast<int>((x + mShiftX) * mScaleX) + mPixBorderX;
}

int Plot::valueToPositionY(double y)
{
    return (mHeight - 1 - mPixBorderY) - static_cast<int>((y + mShiftY) * mScaleY);
}


void Plot::putPixel(int x, int y, QRgb color)
{
    //std::cout << "x: " << pixX << " y: " << pixY << std::endl;
    if ((x >= 0) && (x < mWidth) && (y >= 0) && (y < mHeight)) {
        mImage->setPixel(x, y, color);
    }
}

void Plot::putCross(int x, int y, QRgb color)
{
    putPixel(x, y, color);
    putPixel(x - 1, y, color);
    putPixel(x + 1, y, color);
    putPixel(x, y - 1, color);
    putPixel(x, y + 1, color);
}
