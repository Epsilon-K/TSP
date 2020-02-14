#ifndef TSP_H
#define TSP_H

#include <QMainWindow>
#include <editorview.h>
#include <QColorDialog>


typedef unsigned long long  ull;

namespace Ui {
class TSP;
}

class TSP : public QMainWindow
{
    Q_OBJECT

public:
    explicit TSP(QWidget *parent = nullptr);
    ~TSP();
    void updatePinsInfo();
    ull factorial(int n);
    int getPinIndex(QString str);
    void removePinFromCB(QString str);
    void setButtonStyle();
    QVector<QLineF> getLines(Path p);
    long calcDistance(QVector<QLineF> lines);
    void showBestOfCurrGen();
    QString getOrderString(Path p);

    long manhattenDist(QPoint p1, QPoint p2){
            return absolute(p1.x() - p2.x()) + absolute(p1.y() - p2.y());
        }

    long absolute(long x){
        x = x < 0 ? x * -1 : x;
        return x;
    }

public slots:
    void mouseMoved(QPoint p);
    void nextGen();

private slots:
    void on_pushButton_clicked();

    void on_checkBox_clicked(bool checked);

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();


    void on_gaBtn_toggled(bool checked);

    void on_pushButton_5_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::TSP *ui;
    QVector<MapPin*> pins;
    QPixmap * mapPinNormal;
    QPixmap * mapPinLocked;

    int currGen = 0;
    bool newBest = false;
    QVector<Path> population;
    //qreal shortestInGen = INT32_MAX;
    Path shortestPathInGen;
    PathItem *bestPathItemInGen;
    QColor gaColor;

    Path customPath;
    PathItem customPathItem;
    QColor customColor;
};

#endif // TSP_H
