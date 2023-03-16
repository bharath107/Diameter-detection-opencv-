#include "QtWidgetsApplication1.h"
#include <opencv2/opencv.hpp>
#include "ui_QtWidgetsApplication1.h"
using namespace std;
using namespace cv;
static int i = 1;
QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton,   SIGNAL(clicked()), this, SLOT(Read_Image()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(inspect()));
    connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(next_image()));
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{

}
void QtWidgetsApplication1::Read_Image()
{
    Mat Test1 = imread("C:/Users/SVT/Desktop/bharath/assignment/test1.tif", IMREAD_GRAYSCALE);
    QPixmap pixmap = QPixmap::fromImage(QImage(Test1.data, Test1.cols, Test1.rows, Test1.step, QImage::Format_Grayscale8));
    ui.label->setPixmap(pixmap);
}
void QtWidgetsApplication1::setwindow()
{
    Mat Test1 = imread("C:/Users/SVT/Desktop/bharath/assignment/test1.tif", IMREAD_GRAYSCALE);
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 1.0;
    Scalar fontColor = Scalar(255, 255, 255);
    int thickness = 1;
    int baseline = 0;
    int Width1 = Test1.cols;
    int Height1 = Test1.rows;
    string text = "Width: " + to_string(Width1) + ", Height: " + to_string(Height1);
    Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
    Point textOrg((Test1.cols - textSize.width) / 2, (Test1.rows + textSize.height) / 2);
    putText(Test1, text, textOrg, fontFace, fontScale, fontColor, thickness, LINE_AA);
    QPixmap pixmap = QPixmap::fromImage(QImage(Test1.data, Test1.cols, Test1.rows, Test1.step, QImage::Format_Grayscale8));
    ui.label->setPixmap(pixmap);
}
void QtWidgetsApplication1::inspect()
{
    // Load the input image
    Mat image = imread("C:/Users/SVT/Desktop/bharath/assignment/test" + to_string(i) + ".tif", IMREAD_GRAYSCALE);

    // Apply Gaussian blur to reduce noise
    Mat blurred;
    GaussianBlur(image, blurred, Size(5, 5), 0);

    // Detect edges using Canny edge detection
    Mat edges;
    Canny(blurred, edges, 100, 200);

    // Find contours in the image
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Loop over all the contours
    for (size_t i = 0; i < contours.size(); i++) {
        // Approximate the contour with a polygon
        vector<Point> approx;
        approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);

        // Check if the polygon has four corners and is convex
        if (approx.size() == 4 && isContourConvex(approx)) {
            // Order the corners of the rectangle
            Point2f corners[4];
            for (size_t j = 0; j < 4; j++) {
                corners[j] = approx[j];
            }
            sort(corners, corners + 4, [](const Point2f& a, const Point2f& b) {
                return (a.x + a.y) < (b.x + b.y);
                });

            // Compute the height and width of the rectangle
            Point2f tl = corners[0];
            Point2f tr = corners[1];
            Point2f br = corners[2];
            Point2f bl = corners[3];

            float rect_width = norm(tr - tl);
            float rect_height = norm(bl - tl);

            // Draw the rectangle and display its height and width
            //rectangle(image, tl, br, Scalar(0, 255, 0), 3);
            string text_rect = "Width: " + to_string(rect_width) + ", Height: " + to_string(rect_height);
            putText(image, text_rect, Point(tl.x, tl.y + 20),
                FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255), 2);
        }
    }
    vector<Vec3f> circles;
    HoughCircles(edges, circles, HOUGH_GRADIENT, 1, 50, 100, 30, 0, 0);

    vector<int> diameters;
    for (size_t i = 0; i < circles.size(); i++) {
        int diameter = cvRound(circles[i][2]) * 2;
        diameters.push_back(diameter);
    }

    for (size_t i = 0; i < circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(image, center, radius, Scalar(255, 0, 0), 3, LINE_AA);
        string text_circle = "Diameter: " + to_string(diameters[i]);
        int baseline = 0;
        Size textSize = getTextSize(text_circle, FONT_HERSHEY_SIMPLEX, 0.6, 2, &baseline);
        putText(image, text_circle, Point(center.x - (textSize.width / 2), center.y + radius + textSize.height),
                FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0), 2);
    }

    QPixmap pixmap = QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.cols * image.elemSize1(), QImage::Format_Grayscale8));
    ui.label->setPixmap(pixmap);
}

void QtWidgetsApplication1::next_image()
{
    i++;
    if (i == 7)
        i = 1;
    Mat image = imread("C:/Users/SVT/Desktop/bharath/assignment/test" + to_string(i) + ".tif", IMREAD_GRAYSCALE);
    QPixmap pixmap = QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.cols * image.channels(), QImage::Format_Grayscale8));
    ui.label->setPixmap(pixmap);
}
