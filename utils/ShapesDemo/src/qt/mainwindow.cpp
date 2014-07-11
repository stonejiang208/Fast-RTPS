/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS ShapesDemo is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/
#include "eprosimashapesdemo/qt/mainwindow.h"
#include "eprosimashapesdemo/qt/publishdialog.h"
#include "eprosimashapesdemo/qt/subscribedialog.h"
#include "eprosimashapesdemo/qt/optionsdialog.h"
#include "ui_mainwindow.h"
#include "eprosimashapesdemo/qt/UpdateThread.h"

#include "eprosimashapesdemo/shapesdemo/ShapePublisher.h"
#include "eprosimashapesdemo/shapesdemo/ShapeSubscriber.h"

#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_shapesDemo(this),
    mp_writeThread(NULL)
{
    ui->setupUi(this);
    ui->areaDraw->setShapesDemo(this->getShapesDemo());

    mp_writeThread = new UpdateThread(this,1);
    mp_writeThread->setMainW(this);

    m_pubsub = new QStandardItemModel(0,6,this); //2 Rows and 3 Columns
    m_pubsub->setHorizontalHeaderItem(0, new QStandardItem(QString("Topic")));
    m_pubsub->setHorizontalHeaderItem(1, new QStandardItem(QString("Color")));
    m_pubsub->setHorizontalHeaderItem(2, new QStandardItem(QString("Size")));
    m_pubsub->setHorizontalHeaderItem(3, new QStandardItem(QString("Type")));
    m_pubsub->setHorizontalHeaderItem(4, new QStandardItem(QString("Reliable")));
    m_pubsub->setHorizontalHeaderItem(5, new QStandardItem(QString("History")));
    m_pubsub->setHorizontalHeaderItem(6, new QStandardItem(QString("Durability")));
    m_pubsub->setHorizontalHeaderItem(7, new QStandardItem(QString("Liveliness")));

    ui->tableEndpoint->setModel(m_pubsub);
    ui->tableEndpoint->setColumnWidth(0,70);
    ui->tableEndpoint->setColumnWidth(1,80);
    ui->tableEndpoint->setColumnWidth(2,50);
    ui->tableEndpoint->setColumnWidth(3,50);
    ui->tableEndpoint->setColumnWidth(4,65);
    ui->tableEndpoint->setColumnWidth(5,70);
    ui->tableEndpoint->setColumnWidth(6,110);
    ui->tableEndpoint->setColumnWidth(7,110);


    this->m_shapesDemo.init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bt_publish_clicked()
{
    PublishDialog* pd = new PublishDialog(this->getShapesDemo(),this);
    pd->show();
    mp_writeThread->start();

}

void MainWindow::quitThreads()
{
  mp_writeThread->quit();
}



void MainWindow::on_bt_subscribe_clicked()
{
    SubscribeDialog* pd = new SubscribeDialog(this->getShapesDemo(),this);
    pd->show();

}


void MainWindow::writeNewSamples()
{
   // cout << "MOVING TIMER "<<endl;
    this->m_shapesDemo.moveAllShapes();
    this->m_shapesDemo.writeAll();
}

void MainWindow::on_actionPreferences_triggered()
{
    OptionsDialog* od = new OptionsDialog(this->getShapesDemo(),this);
    od->show();
}

 void MainWindow::updateInterval(uint32_t ms)
 {
     this->mp_writeThread->updateInterval(ms);
 }

void MainWindow::on_actionStart_triggered()
{
    this->m_shapesDemo.init();
}

void MainWindow::on_actionStop_triggered()
{
    this->m_shapesDemo.stop();
    m_pubsub->removeRows(0,m_pubsub->rowCount());
    update();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::addPublisherToTable(ShapePublisher* spub)
{
    QList<QStandardItem*> items;
    items.append(new QStandardItem(spub->m_shape.getShapeQStr()));
    items.append(new QStandardItem(QString(spub->m_shape.m_mainShape.getColorStr().c_str())));
    items.append(new QStandardItem(QString("%1").arg(spub->m_shape.m_mainShape.m_size)));
    items.append(new QStandardItem("Pub"));
    if(spub->m_attributes.qos.m_reliability.kind == RELIABLE_RELIABILITY_QOS)
        items.append(new QStandardItem("True"));
    else
         items.append(new QStandardItem("False"));
    items.append(new QStandardItem(QString("%1").arg(spub->m_attributes.topic.historyQos.depth)));
    if(spub->m_attributes.qos.m_durability.kind == VOLATILE_DURABILITY_QOS)
        items.append(new QStandardItem("VOLATILE"));
    else
        items.append(new QStandardItem("TRANSIENT_LOCAL"));

    if(spub->m_attributes.qos.m_liveliness.kind == AUTOMATIC_LIVELINESS_QOS)
        items.append(new QStandardItem("AUTOMATIC"));
    else if(spub->m_attributes.qos.m_liveliness.kind == MANUAL_BY_PARTICIPANT_LIVELINESS_QOS)
        items.append(new QStandardItem("MANUAL_PARTICIPANT"));
    else
        items.append(new QStandardItem("MANUAL_TOPIC"));
    m_pubsub->appendRow(items);

}

void MainWindow::addSubscriberToTable(ShapeSubscriber* ssub)
{
    QList<QStandardItem*> items;
    items.append(new QStandardItem(ssub->m_shape.getShapeQStr()));
    items.append(new QStandardItem("---"));
    items.append(new QStandardItem("---"));
    items.append(new QStandardItem("Sub"));

    if(ssub->m_attributes.qos.m_reliability.kind == RELIABLE_RELIABILITY_QOS)
        items.append(new QStandardItem("True"));
    else
         items.append(new QStandardItem("False"));

    items.append(new QStandardItem(QString("%1").arg(ssub->m_attributes.topic.historyQos.depth)));

    if(ssub->m_attributes.qos.m_durability.kind == VOLATILE_DURABILITY_QOS)
        items.append(new QStandardItem("VOLATILE"));
    else
        items.append(new QStandardItem("TRANSIENT_LOCAL"));

    if(ssub->m_attributes.qos.m_liveliness.kind == AUTOMATIC_LIVELINESS_QOS)
        items.append(new QStandardItem("AUTOMATIC"));
    else if(ssub->m_attributes.qos.m_liveliness.kind == MANUAL_BY_PARTICIPANT_LIVELINESS_QOS)
        items.append(new QStandardItem("MANUAL_PARTICIPANT"));
    else
        items.append(new QStandardItem("MANUAL_TOPIC"));
    m_pubsub->appendRow(items);
}