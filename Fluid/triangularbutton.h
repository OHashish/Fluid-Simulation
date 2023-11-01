#ifndef TRIANGULARBUTTON_H
#define TRIANGULARBUTTON_H

#include <QPushButton>

class TriangularButton : public QPushButton
{
    Q_OBJECT
public:
    TriangularButton(const QString &text, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // TRIANGULARBUTTON_H
