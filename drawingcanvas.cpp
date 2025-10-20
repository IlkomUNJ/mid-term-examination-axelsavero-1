#include "drawingcanvas.h"
#include <QPainter>
#include <QDebug>
#include <QDir>


DrawingCanvas::DrawingCanvas(QWidget *parent)  {
    // Set a minimum size for the canvas
    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    // Set a solid background color
    setStyleSheet("background-color: white; border: 1px solid gray;");
}

void DrawingCanvas::clearPoints(){
    m_points.clear();
    // Trigger a repaint to clear the canvas
    update();
}

void DrawingCanvas::paintLines(){
    /* Todo
     * Implement lines drawing per even pair
    */

    isPaintLinesClicked = true;
    update();
}

void DrawingCanvas::segmentDetection(){
    QPixmap pixmap = this->grab(); //
    QImage image = pixmap.toImage();

    cout << "image width " << image.width() << endl;
    cout << "image height " << image.height() << endl;

    QVector<QPoint> purpleRects;
    //To not crash we set initial size of the matrix
    vector<CustomMatrix> windows(image.width()*image.height());

    // Get the pixel value as an ARGB integer (QRgb is a typedef for unsigned int)
    for (int x = 1; x < image.width() - 1; x++) {
        for (int y = 1; y < image.height() - 1; y++) {
            bool window[3][3];

            // Take a 3x3 window around (x,y)
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    QColor color(image.pixel(x + i, y + j));
                    window[i + 1][j + 1] = (color != Qt::white);
                }
            }

            // Check horizontal pattern
            bool horizontal = window[1][0] && window[1][1] && window[1][2];
            // Cek vertical pattern
            bool vertical = window[0][1] && window[1][1] && window[2][1];
            // Cek diagonal pattern (left-right)
            bool diag1 = window[0][0] && window[1][1] && window[2][2];
            // Cek diagonal pattern (right-left
            bool diag2 = window[0][2] && window[1][1] && window[2][0];

            if (horizontal || vertical || diag1 || diag2) {
                purpleRects.push_back(QPoint(x, y));
            }
        }
    }
    // Draw a small rectangle of purple color to mark the candidate
    QPainter painter(&pixmap);
    painter.setBrush(QBrush(Qt::magenta, Qt::SolidPattern));
    painter.setPen(Qt::NoPen);

    for (const QPoint &pt : purpleRects) {
        painter.drawRect(pt.x() - 1, pt.y() - 1, 3, 3);
    }

    image = pixmap.toImage();
    pixmap.save("segment_detection_result.png");
    qDebug() << "File saved at:" << QDir::currentPath() + "/segment_detection_result.png";


    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up the pen and brush for drawing the points
    QPen pen(Qt::blue, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));

    // Draw a small circle at each stored point
    for (const QPoint& point : std::as_const(m_points)) {
        painter.drawEllipse(point, 3, 3);
    }

    if(isPaintLinesClicked){
        cout << "paint lines block is called" << endl;
        pen.setColor(Qt::red);
        pen.setWidth(4); // 4-pixel wide line
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        // Set the painter's pen to our custom pen.
        painter.setPen(pen);

        for(int i=0;i<m_points.size()-1;i+=2){
            //cout << m_points[i].x() << endl;
            painter.drawLine(m_points[i], m_points[i+1]);
        }
        isPaintLinesClicked = false;

        //return painter pen to blue
        pen.setColor(Qt::blue);
        painter.setPen(pen);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    // Add the mouse click position to our vector of points
    m_points.append(event->pos());
    // Trigger a repaint
    update();
}


