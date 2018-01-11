#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cls_displaymodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionStart_triggered()
{
    ui->openGLWidget->makeCurrent();
    cls_DisplayModel* v_disp_model = new cls_DisplayModel(ui->openGLWidget);
    v_disp_model->GenerateBox();
    ui->openGLWidget->SetModel(v_disp_model);
    ui->openGLWidget->update();
}
