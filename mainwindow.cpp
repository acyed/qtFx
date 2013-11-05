#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include "alienfx.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    regionIndex = 0;
    colorIndex = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_regionBox_currentIndexChanged(int index)
{
    printf("region: %i\n", index);
    regionIndex = index;
}

void MainWindow::on_colorBox_currentIndexChanged(int index)
{
    printf("color : %i\n", index);
    colorIndex = index;
}


void MainWindow::on_applyButton_clicked()
{
    unsigned int color =0;
    unsigned int region;

    switch (regionIndex) {
    case 0: //All
        region = 0x4BEF;
        break;
    case 1: //Numpad
        region = 0x0008;
        break;
    case 2: //Keyboard Right
        region = 0x0004;
        break;
    case 3: //Keyboard Center
        region = 0x0002;
        break;
    case 4: //Keybaord Left
        region = 0x0001;
        break;
    case 5: //Pipes
        region = 0x0060;
        break;
    case 6: //Logo
        region = 0x0100;
        break;
    case 7: //Trackpad
        region = 0x0200;
        break;
    case 8: //Status
        region = 0x4800;
        break;
    case 9: //Status
        region = 0x0080;
        break;

    default:
        region = 0x4BEF;
        break;
    }

    switch (colorIndex) {
    case 0: //Off
        color = 0x0000;
        break;
    case 1: //Royal
        color = 0x00F0;
        break;
    case 2: //Cornflower
        color = 0x06F0;
        break;
    case 3: //Turquise
        color = 0x0AF0;
        break;
    case 4: //Aqua
        color = 0x0F90;
        break;
    case 5: //Chive
        color = 0x3F40;
        break;
    case 6: //Grass
        color = 0x1D10;
        break;
    case 7: //Emerald
        color = 0x1F00;
        break;
    case 8: //Green
        color = 0x2F00;
        break;
    case 9: //Lime
        color = 0x4F00;
        break;
    case 10: //Lemon
        color = 0x7F00;
        break;
    case 11: //Marigold
        color = 0xCE00;
        break;
    case 12: //Burnt Orange
        color = 0xF800;
        break;
    case 13: //Pumpkin
        color = 0xF500;
        break;
    case 14: //Cherry
        color = 0xF000;
        break;
    case 15: //Hot Pink
        color = 0xF010;
        break;
    case 16: //Barbie Pink
        color = 0xF0B0;
        break;
    case 17: //Fuchsia
        color = 0xF0F0;
        break;
    case 18: //Grape
        color = 0x60F0;
        break;
    case 19: //Indigo
        color = 0x30F0;
        break;
    default:
        //Lets make it indigo
        color = 0x30F0;
        break;
    }

    usbTestCode(region, color);
}


