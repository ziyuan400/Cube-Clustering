#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <nlohmann/json.hpp>
#include <QtDataVisualization>
//using namespace QtDataVisualization;

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

void MainWindow::log(std::string str){
    ui->log->append(QString::fromStdString(str) );
}
QWidget* MainWindow::get_widget(std::string str){
    return NULL;
}
