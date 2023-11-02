#include "triangularbutton.h"
#include <QPainter>
#include <QPolygonF>
#include <QPainterPath>
#include <QMouseEvent>
TriangularButton::TriangularButton(const QString &text, QWidget *parent, int rotate)
    : QPushButton(text, parent),rotate(rotate)
{

}

void TriangularButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPolygonF triangle = trianglePoints(rotate);

    QPainterPath path;
    path.addPolygon(triangle);


    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(palette().button()); // Set the background color

    painter.drawPolygon(triangle);
    painter.drawText(rect(), Qt::AlignCenter, text());
}

void TriangularButton::mousePressEvent(QMouseEvent *event) {
    QPolygonF triangle = trianglePoints(rotate);

    if (triangle.containsPoint(event->pos(), Qt::OddEvenFill)) {
        // The mouse position is inside the triangle
        QPushButton::mousePressEvent(event);
    } else {
        // The mouse position is outside the triangle
        event->ignore();
    }
}

void TriangularButton::mouseReleaseEvent(QMouseEvent *event) {
    QPolygonF triangle = trianglePoints(rotate);

    if (triangle.containsPoint(event->pos(), Qt::OddEvenFill)) {
        // The mouse position is inside the triangle
        QPushButton::mouseReleaseEvent(event);
    } else {
        // The mouse position is outside the triangle
        event->ignore();
    }

}

QPolygonF TriangularButton::trianglePoints(int rotate){
    QPolygonF triangle;
    if (rotate == 90){
        triangle << QPointF(width()/2, height()/2);
        triangle << QPointF(width(),height());
        triangle << QPointF(width(),0);
    }else if (rotate == 180){
        triangle << QPointF( width()/2,0);
        triangle << QPointF(0, height());
        triangle << QPointF(width(),height());
    }else if (rotate == 270){
        triangle << QPointF(0, 0);
        triangle << QPointF(0, height());
        triangle << QPointF(width()/2, height()/2);
    }else{
        triangle << QPointF(0, 0);
        triangle << QPointF(width(), 0);
        triangle << QPointF(width()/2, height());
    }

    return triangle;
}
