#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>

class Renderer : public QWidget
{
    Q_OBJECT
public:
    explicit Renderer(QWidget *parent = nullptr);
    ~Renderer();
    void paintEvent(QPaintEvent *);

public slots:
    void repaint(const QVector<QPointF>&);

private:
    QImage *image;
    QVector<QPointF> positions;
};

#endif // RENDERER_H
