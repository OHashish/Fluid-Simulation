#ifndef TRIANGULARBUTTON_H
#define TRIANGULARBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QPolygonF>
#include <QPainterPath>
#include <QMouseEvent>

class TriangularButton : public QPushButton{
    Q_OBJECT
public:
    TriangularButton(const QString &text, QWidget *parent = nullptr,int rotate = 0);

    QPolygonF trianglePoints(int rotate);
    int rotate;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // TRIANGULARBUTTON_H
