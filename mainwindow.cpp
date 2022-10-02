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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // std::unordered_map<std::string, std::pair<double, double> > airports;
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

        // QString str = stream.readAll();
        QString name, lat, lon;
        stream >> name >> lat >> lon;
        stream >> name >> lat >> lon;

        // ui->textBrowser->append(str);
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

void MainWindow::on_pushButton_2_clicked()
{
    std::pair<std::string, std::pair<double, double> > selectedAirport;
    std::vector<std::pair<double, std::string> > airportsDistance;
    double distance;

    QString str = ui->lineEdit->text();
    double radius = str.toDouble();
    std::string text = "";

    QItemSelectionModel *sModel = ui->tableWidget->selectionModel();

    foreach(QModelIndex index, sModel->selectedRows())
        std::cout << ui->tableWidget->item(index.row(), 0)->text().toStdString();

    QList<QTableWidgetItem *> selectedList = ui->tableWidget->selectedItems();
    std::cout << "selected list size: " << selectedList.count() << std::endl;
    if (selectedList.count() == 3)
        selectedAirport = std::make_pair(selectedList.at(0)->text().toStdString(), std::make_pair(selectedList.at(1)->text().toDouble(), selectedList.at(2)->text().toDouble()));
    // else
    //     exception!

    for(std::unordered_map<std::string, std::pair<double, double> >::iterator it = airports.begin(); it != airports.end(); it++) {
        distance = distanceFromLatLonInKm(it->second.first - selectedAirport.second.first, it->second.second - selectedAirport.second.second);
        airportsDistance.push_back(std::make_pair(distance, it->first));
    }
    std::cout << "selected airport: " << selectedAirport.first << " (" << selectedAirport.second.first << "; " << selectedAirport.second.second << ")\n";
    std::cout << "count: " << airportsDistance.size() << std::endl;
    std::sort(airportsDistance.begin(), airportsDistance.end());

    for (std::vector<std::pair<double, std::string> >::iterator it = airportsDistance.begin(); it != airportsDistance.end(); it++)
        std::cout << "airpirt: " << it->second << " distance: " << it->first << std::endl;

    std::vector<std::pair<double, std::string> >::iterator it = airportsDistance.begin() + 1;
    while (it != airportsDistance.end() && it->first <= (radius * radius)) {
        text += (it->second+ "\n");
        it++;
    }
    ui->textBrowser->clear();
    ui->textBrowser->append(QString::fromStdString(text));
}
