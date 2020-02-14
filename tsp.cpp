#include "tsp.h"
#include "ui_tsp.h"

TSP::TSP(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TSP)
{
    ui->setupUi(this);
    srand(unsigned (time(nullptr)));

    showMaximized();
    connect(ui->view, SIGNAL(signalMouseMoveEvent(QPoint)), this, SLOT(mouseMoved(QPoint)));

    mapPinNormal = new QPixmap(":/map pin.png");
    mapPinLocked =  new QPixmap(":/map pin locked.png");

    gaColor = QColor(90, 250, 110);
    setButtonStyle();

    customColor = QColor(90, 110, 250);
}

TSP::~TSP()
{
    delete ui;
}

void TSP::updatePinsInfo()
{
    ui->infoLabel->setText(QString::number(pins.size()) + " Nodes -> " +
                           QString::number(factorial(pins.size()-1)) + " possible paths");
}

ull TSP::factorial(int n){
    if(n <= 1) return 1;
    return ull(n) * factorial(n-1);
}

int TSP::getPinIndex(QString str)
{
    for(int i = 0; i < pins.size(); i++){
        if(pins.at(i)->name == str) return i;
    }
    qDebug() << "Error in TSP::getPinIndex()";
    return 0;
}

void TSP::removePinFromCB(QString str)
{
    for(int i = 0; i < ui->comboBox->count(); i++){
        if(ui->comboBox->itemText(i) == str){
            ui->comboBox->removeItem(i);
            return;
        }
    }
}

void TSP::setButtonStyle()
{
    QColor gaf = gaColor.value() > 127 ? QColor("black") : QColor("white");
    ui->pushButton_6->setStyleSheet("color:"+Utils::colorToString(gaf) +
                                    "background-color : " + Utils::colorToString(gaColor));
}

QVector<QLineF> TSP::getLines(Path p)
{
    QVector<QLineF> lines;
    for(int i = 0; i < p.order.size(); i++){
        QLineF line(pins[p.order[i]]->pos(),
                pins[p.order[i < p.order.size()-1 ? i+1 : 0]]->pos());
        lines.push_back(line);
    }
    return lines;
}

long TSP::calcDistance(QVector<QLineF> lines)
{
    long sum = 0;
    for(int i = 0; i < lines.size(); i++){
        //sum += lines[i].length();
        sum += manhattenDist(lines[i].p1().toPoint(), lines[i].p2().toPoint());
    }
    return sum;
}

void TSP::showBestOfCurrGen()
{
    if(newBest){
        // On Scene
        if(currGen > 0)
            bestPathItemInGen->deleteLater();
        bestPathItemInGen = new PathItem(getLines(shortestPathInGen), 0.6, gaColor, ui->view->scene());

        // on treeView
        QStringList ls; ls << QString::number(currGen) << QString::number(shortestPathInGen.length);
        ls << getOrderString(shortestPathInGen);
        QTreeWidgetItem * tr = new QTreeWidgetItem(ls);
        ui->treeWidget->insertTopLevelItem(0,tr);

        newBest = false;
    }
}

QString TSP::getOrderString(Path p)
{
    QString str;
    for(int i = 0; i < p.order.size(); i++){
        str += pins[p.order[i]]->name + ", ";
    }
    return str;
}

void TSP::mouseMoved(QPoint p)
{
    QPoint sp = ui->view->mapToScene(p).toPoint();
    ui->cursorLabel->setText("Mouse Cursor : (" + QString::number(sp.x()) + ", " +
                             QString::number(sp.y()) + ")");
}

void TSP::nextGen()
{
    currGen++;
    if(ui->gaBtn->isChecked()){
        // Generate Next Gen
        population[0] = shortestPathInGen;
        for(int i = 1; i < population.size(); i++){
            // Copy from the best
            population[i] = shortestPathInGen;
            // Mutate
            population[i].shuffle(rand()%(pins.size()/2));
        }

        // Calc Distances
        for(int i = 0; i < population.size(); i++){
            population[i].length = calcDistance(getLines(population[i]));
            if(population[i].length < shortestPathInGen.length){
                newBest = true;
                shortestPathInGen = population[i];
            }
        }

        showBestOfCurrGen();
        QTimer::singleShot(50,this,SLOT(nextGen()));
    }
}

void TSP::on_pushButton_clicked()
{
    // ADD Map Pin
    bool checked = ui->checkBox->isChecked();
    MapPin *pin = new MapPin(checked ? mapPinLocked : mapPinNormal, pins.size()+1);
    ui->view->scene()->addItem(pin);
    pin->setPos(0,0);
    pin->centerLabel();
    pin->setFlag(QGraphicsItem::ItemIsMovable, !checked);
    pin->setFlag(QGraphicsItem::ItemIsSelectable, !checked);
    pins.append(pin);

    updatePinsInfo();

    // Add pin to starting nodes list
    ui->comboBox->addItem(pin->name);
    if(ui->comboBox->count() == 1) ui->comboBox->setCurrentIndex(0);

    ui->gaBtn->setEnabled(pins.size() > 3);
}

void TSP::on_checkBox_clicked(bool checked)
{
    // Lock / UnLock Nodes
    for(int i = 0; i < pins.size(); i++){
        pins[i]->setFlag(QGraphicsItem::ItemIsMovable, !checked);
        pins[i]->setFlag(QGraphicsItem::ItemIsSelectable, !checked);
        pins[i]->setPixmap(checked ? *mapPinLocked : *mapPinNormal);
    }

    // and Disable ADD/Del Btns
    ui->pushButton->setDisabled(checked);
    ui->pushButton_2->setDisabled(checked);
    ui->comboBox->setDisabled(checked);
}

void TSP::on_pushButton_2_clicked()
{
    // Remove Selected Nodes
    QList<QGraphicsItem *> sl = ui->view->scene()->selectedItems();
    for(int i = 0; i < sl.size(); i++){
        MapPin *p = (MapPin*)sl[i];
        QString pName = p->name;
        removePinFromCB(pName);
        pins.removeAt(getPinIndex(pName));
        p->deleteLater();
    }

    updatePinsInfo();

    ui->gaBtn->setDisabled(pins.size() < 4);
}

void TSP::on_pushButton_6_clicked()
{
    // Change GA_Color
    gaColor = QColorDialog::getColor();
    setButtonStyle();
}

void TSP::on_gaBtn_toggled(bool checked)
{
    // Lock All Nodes
    ui->checkBox->setChecked(checked);
    on_checkBox_clicked(checked);

    if(checked){
        ui->gaBtn->setText("Stop");
        // Start
        // Clear
        on_pushButton_5_clicked();  // clear list

        int stInd = getPinIndex(ui->comboBox->currentText());

        QVector<int> p(pins.size());
        p[0] = stInd;
        for(int j = 1; j < p.size(); j++){
            p[j] = pins[j]->name == ui->comboBox->currentText() ? 0 : j;
        }
        Path path(p); shortestPathInGen = path;
        for (int i = 0; i < ui->spinBox->value(); i++) {
            Path np(path);
            np.shuffle(np.order.size());
            population.push_back(np);
            //np.print();
        }

        // Calc Distances
        for(int i = 0; i < population.size(); i++){
            population[i].length = calcDistance(getLines(population[i]));
            if(population[i].length < shortestPathInGen.length){
                newBest = true;
                shortestPathInGen = population[i];
            }
        }

        showBestOfCurrGen();
        QTimer::singleShot(60,this,SLOT(nextGen()));
    }else{
        // Stopping the algorithm
        ui->gaBtn->setText("Re-Start GA Algorithm");
    }

}

void TSP::on_pushButton_5_clicked()
{
    // Clear Population and TreeView
    population.clear();
    ui->treeWidget->clear();

    // Clear Graphics
    if(currGen != 0)
    bestPathItemInGen->deleteLater();
    currGen = 0;
}

void TSP::on_horizontalSlider_valueChanged(int value)
{
    ui->view->pixmapBG->setOpacity(qreal(value)/100);
}

void TSP::on_lineEdit_textChanged(const QString &arg1)
{
    QStringList ls = arg1.split(" ", QString::SkipEmptyParts);

    if(ls.size() > pins.size()) return;
    if(ls.size() < 2){
        customPathItem.deleteAllLines();
         return;
    }
    QVector<int> p;
    for(int i = 0; i < ls.size(); i++){
        p.push_back(getPinIndex(ls[i]));
    }
    customPath = Path(p);
    customPath.length = calcDistance(getLines(customPath));

    customPathItem.Replace(getLines(customPath), 0.6, customColor, ui->view->scene(), 2);
    ui->label_2->setText("Custom Path Length : " + QString::number(customPath.length));
}
