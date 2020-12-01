#include "widget.h"
#include "ui_widget.h"

#include <QVideoWidget>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QCamera *camera = new QCamera(this);

    camera->setViewfinder(ui->testWidget);
    camera->start();

}

Widget::~Widget()
{
    delete ui;
}
