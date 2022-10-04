#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <cmath>
#include "tree.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    int i = 0;
    QFile file(":/data/airports_short.csv");
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);

        QString name, lat, lon;
        stream >> name >> lat >> lon;
        stream >> name >> lat >> lon;
        while (name.size()) {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());

            QTableWidgetItem *itemName = new QTableWidgetItem(name);
            itemName->setFlags(itemName->flags() &  ~Qt::ItemIsEditable);
            QTableWidgetItem *itemLat = new QTableWidgetItem(lat);
            itemLat->setFlags(itemLat->flags() &  ~Qt::ItemIsEditable);
            QTableWidgetItem *itemLon = new QTableWidgetItem(lon);
            itemLon->setFlags(itemLon->flags() &  ~Qt::ItemIsEditable);

            ui->tableWidget->setItem(i, 0, itemName);
            ui->tableWidget->setItem(i, 1, itemLat);
            ui->tableWidget->setItem(i, 2, itemLon);
            airports.insert(std::make_pair(name.toStdString(), std::make_pair(lat.toDouble(), lon.toDouble())));
            stream >> name >> lat >> lon;
            i++;
        }
        file.close();
    }
    ui->pushButton->setEnabled(false);
}

double deg2rad(double deg) {
  return deg * (M_PI/180);
}

double distanceFromLatLonInKm(double lat, double lon) {
  double a, c, R = 6371; // Radius of the earth in km

  a = sin(deg2rad(lat) / 2) * sin(deg2rad(lat) / 2) + cos(0) * cos(deg2rad(lat)) * sin(deg2rad(lon) / 2) * sin(deg2rad(lon) / 2);
  c = 2 * atan(sqrt(a) / sqrt(1 - a));
  return R * c;
}

std::vector<std::pair<std::string, std::pair<double, double> > > MainWindow::findNearest(std::pair<std::string, std::pair<double, double> > selectedAirport, double radius) {
    double distance;
    std::vector<std::pair<std::string, std::pair<double, double> > > nearestAirports;

    for (std::unordered_map<std::string, std::pair<double, double> >::iterator it = airports.begin(); it != airports.end(); it++) {
        distance = distanceFromLatLonInKm(it->second.first - selectedAirport.second.first, it->second.second - selectedAirport.second.second);
        if (distance <= (radius * radius))
            nearestAirports.push_back(*it);
    }
    return nearestAirports;
}

void MainWindow::on_pushButton_2_clicked()
{
    std::pair<std::string, std::pair<double, double> > selectedAirport;
    std::vector<std::pair<std::string, std::pair<double, double> > > nearesrAirports;
    std::string text = "";
    QString str = ui->lineEdit->text();
    double radius = str.toDouble();


//    QItemSelectionModel *sModel = ui->tableWidget->selectionModel();

//    foreach(QModelIndex index, sModel->selectedRows())
//        std::cout << ui->tableWidget->item(index.row(), 0)->text().toStdString();

    QList<QTableWidgetItem *> selectedList = ui->tableWidget->selectedItems();
    if (selectedList.count() == 3)
        selectedAirport = std::make_pair(selectedList.at(0)->text().toStdString(), std::make_pair(selectedList.at(1)->text().toDouble(), selectedList.at(2)->text().toDouble()));
    // else
    //     exception!


    nearesrAirports = findNearest(selectedAirport, radius);
    std::cout << "size neares: " << nearesrAirports.size() << std::endl;
    for (size_t i = 0; i < nearesrAirports.size(); i++)
            text += (nearesrAirports[i].first + "\n");
    if (text == "")
        text = "no airports";
    ui->textBrowser->clear();
    ui->textBrowser->append(QString::fromStdString(text));
}

/*bool check() {

}*/

void MainWindow::on_pushButton_3_clicked()
{
    Tree tree;
    std::pair<std::string, std::pair<double, double> > airportFrom;
    std::pair<std::string, std::pair<double, double> > airportTo;
    // std::vector<std::pair<std::string, std::pair<double, double> > > nearest;

    std::vector<std::string> path;
    QString airport1, airport2;
    double M;
    Node* node, nearest;
    bool finded = false;

    airport1 = ui->lineEdit_3->text();
    airport2 = ui->lineEdit_4->text();
    M = ui->lineEdit_2->text().toDouble();
    std::cout << "M: " << M << " findde: " << finded << std::endl;

    for (std::unordered_map<std::string, std::pair<double, double> >::iterator it = airports.begin(); it != airports.end(); it++) {
        if (it->first == airport1.toStdString())
            airportFrom = *it;
        else if (it->first == airport1.toStdString())
            airportTo = *it;
    }

    tree._root = tree.createNode(airportFrom.first, airportFrom.second.first, airportFrom.second.second);
    node = tree._root;
    std::cout << "node name: " << node->_name << std::endl;
    while (!finded) {
        nearest = findNearest(node, M);
        for (size_t i = 0; i < nearest.size(); i++) {
            if (nearest[i].first == airportTo.first) {
                finded;
                while (node) {
                    path.push_back(node->_name);
                    node = node->_parent;
                }
                break ;
            }
            node->_children.push_back(nearest[i]);
            node->_children[i]->_parent = node;
        }
        node->_children;
    }

    for (size_t i = path.size() - 1; i >= 0; i--) {
        ui->textBrowser_2->append(path[i]);
        if(i)
            ui->textBrowser_2->append("->");
    }
}
