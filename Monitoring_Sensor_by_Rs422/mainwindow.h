#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPrinter>
#include <QFile>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <cmath>
#include <QVector>
#include <sstream>
#include <cstdlib>
#include <QProcess>
#include <QtGlobal>
#include <iostream>
#include <QPageSize>
#include <QScrollBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QTextDocument>
#include <QElapsedTimer>
#include <QTextStream>

#include <sstream>
#include <iomanip>
#include <QTimer>
#include "receving_data.h"
#include "global_for_all.h"
#include "receving_for_fuze.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void show_ports_available();
    void serial_port_for_pdpu();
    void Table_stretech();
    void sendHexToSerialPort(const QString& hexValue);
    void populate_in_pdpu();
    void populate_infuze();
    void populate_in_dau();
    void populate_in_label_for_sensor();
    void populate_in_label_for_dau();
    void empty();
    void populate_without_button();
    void saving_for_making_pdf();
     QByteArray accumulatedData,dataChunk,first12Bytes,last2Bytes,dataPacket,chunk37,chunkdau,chunk;
     QByteArray acuumalted_fuze;
     QVector<QString> Measured_Value,Another_Vector ,measured_value_fuze,another_vectore_fuze;
     int Time_elap_hour , Time_elap_min , Time_elap_sec,startIndex ,decimalValue;
     QString result_string ,currentCommand,data, result_string2,result_string37,result_string119,unit_name,test_type,serial_number,Porject_name,Lable_text,hexValues;
     void convertHTMLToPDF_for_PDPU(const QString &htmlContent, const QString &pdfFilePath);
     QString generateHTMLReport_for_pdpu(QTableWidget *tableWidget3);
      QString generateHTMLFor_sensor_card(QTableWidget *tableWidget2);
       QString generateHTMLFor_dau(QTableWidget *tableWidget1);
    ~MainWindow();

private slots:
    void on_PDPU_REFRESH_clicked();

    void on_PDPU_CONNECT_clicked();

    void on_PDPU_DISCONNECT_clicked();

    void on_SENDCOMMAND_clicked();
    void on_send_fuze_command_clicked();

    void on_SAVE_IMU_2_clicked();

    void on_Stop_Pdpu_clicked();

    void on_CLEAR_IMU_2_clicked();

    void on_Delete_clicked();



    void on_save_for_both_clicked();

    void on_clear_for_btoh_clicked();

    void on_send_for_both_clicked();

    void on_stop_for_both_clicked();

signals:
     void  dataReceived_imu(const QByteArray &data);

public slots:
     void handleReceivedData(const QByteArray &data);
     void handle_fuze(const QByteArray &data);
     void emptyy();

private:
    Ui::MainWindow *ui;
    receving_data *recive;
    receving_for_fuze *fuze;
    QTimer *time;
};

#endif // MAINWINDOW_H
