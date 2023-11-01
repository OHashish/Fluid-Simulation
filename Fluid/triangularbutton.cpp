#include "triangularbutton.h"
#include <QPainter>
#include <QPolygonF>
#include <QPainterPath>

TriangularButton::TriangularButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
}

void TriangularButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPolygonF triangle;

//    triangle << QPointF(0, height());
//    triangle << QPointF(width() / 2, 0);
//    triangle << QPointF(width(), height());

    triangle << QPointF(0, 0);
    triangle << QPointF(width(), 0);
    triangle << QPointF(width()/2, height());

    QPainterPath path;
    path.addPolygon(triangle);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(palette().button()); // Set the background color
//    painter.fillPath(path, palette().button());

    painter.drawPolygon(triangle);
    painter.drawText(rect(), Qt::AlignCenter, text());
}
