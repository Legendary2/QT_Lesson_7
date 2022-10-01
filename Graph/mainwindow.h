#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QHash>
#include <functional>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ItemEventFilter : public QGraphicsItem
{
public:
    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget* = 0) override;
protected:
    bool sceneEventFilter(QGraphicsItem* object, QEvent* event) override;
private:
    bool move(QGraphicsItem* object, QEvent* event);
    bool rotate(QGraphicsItem *watched, QEvent *event);
private:
    bool canMove = false;
    bool canRotate = false;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    enum Figure {CIRCLE, RECTANGLE, STAR, ELLIPS, TRIANGLE, SQUARE, RHOMBUS};
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject* object, QEvent* event) override;
private:
    QPolygonF GetStar(qint32 x, qint32 y) const;
    QPolygonF GetTriangle(qint32 x, qint32 y) const;
    QRectF GetEllipse(qint32 x, qint32 y) const;
    QRectF GetCircle(qint32 x, qint32 y) const;
    QRectF GetRect(qint32 x, qint32 y) const;
    QRectF GetSquare(qint32 x, qint32 y) const;
    QPolygonF GetRhombus(qint32 x, qint32 y) const;

    Figure GetFigureType() const;
    QPen GetPen() const;
    QBrush GetBrash() const;

    void CreateFigure(int x, int y);

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QGraphicsView view;
    qint8 figureIndex;
    QHash <qint32, std::function<void(int,int)>> figureMakers;
};
#endif // MAINWINDOW_H
