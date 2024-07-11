#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptrPort = new QSerialPort;
    recive =new receving_data;
    fuze = new receving_for_fuze;
    time = new QTimer;
    Table_stretech();
    ui->tabWidget->setCurrentIndex(0);
    connect(recive,&receving_data::dataReceived_imu,this,MainWindow::handleReceivedData);
    connect(fuze,&receving_for_fuze::dataReceived_fuse,this,MainWindow::handle_fuze);
    connect(time,&QTimer::timeout,this,MainWindow::emptyy);
    ui->pdpucommandcombo->addItem("Motor Start");
    ui->pdpucommandcombo->addItem("Motor Stop");

}

MainWindow::~MainWindow()
{
    delete ui;
    delete recive;
    delete ptrPort;
    delete fuze;
    delete time;
}
void MainWindow::show_ports_available()
{

    QComboBox *comboBoxSerialPort4 = ui->PDPU_COMBO;



    comboBoxSerialPort4->clear();

    QStringList portNames = global_for_all().getAvailablePorts();


    comboBoxSerialPort4->addItems(portNames);
}

void MainWindow::serial_port_for_pdpu()
{

    QComboBox *comboBoxSerialPort = ui->PDPU_COMBO;
    QString selectedPortName = comboBoxSerialPort->currentText();

    ptrPort->setPortName(selectedPortName);
    ptrPort->setBaudRate(115200);
    ptrPort->setDataBits(QSerialPort::Data8);
    ptrPort->setFlowControl(QSerialPort::NoFlowControl);

    ptrPort->setParity(QSerialPort::NoParity);

    ptrPort->setStopBits(QSerialPort::OneStop);




    if (ptrPort->open(QIODevice::ReadWrite)) {
        QMessageBox::information(this,"Connected-Status","Serial Port Connected Successfully");


    }else{
        QMessageBox::information(this,"Connected-Status","No Port Connected");

    }

}

void MainWindow::on_PDPU_REFRESH_clicked()
{
    show_ports_available();
}

void MainWindow::on_PDPU_CONNECT_clicked()
{
    serial_port_for_pdpu();
    time->start(1000);



}


void MainWindow::on_SENDCOMMAND_clicked()
{
     time->start(1000);

    fuze->stop();
    recive->stop();


    recive->stoppedd = false;
    recive->start();

    recive->setPriority(QThread::HighestPriority);

    QMap<int, QString> indexToHexMap = {
        {0, "41435456"},
        {1, "33563356"},
        {2, "35564E2D"},
        {3, "35565231"},
        {4, "32344E48"},
        {5, "32345254"},
        {6, "50484E56"},
        {7, "504C4E52"},
        {8, "5048524A"},
        {9, "504C524E"},
        {10, "4241484B"},
        {11, "42414C4F"},
        {12, "43414D4F"},
        {13, "504E544D"},
        {14, "5052544D"},
        {15, "4254544D"},
        {16, "25987FC2"},
        {17, "259800BD"},



    };
    QString selectedCommandName = ui->pdpucommandcombo->currentText();

    int selectedIndex = ui->pdpucommandcombo->currentIndex();


    if (indexToHexMap.contains(selectedIndex)) {
        QString hexValue = indexToHexMap.value(selectedIndex);
        sendHexToSerialPort(hexValue);

        int row = ui->Pdpu_table->rowCount();
        ui->Pdpu_table->insertRow(row);


        QTableWidgetItem *commandItem = new QTableWidgetItem(selectedCommandName);
        ui->Pdpu_table->setItem(row, 0, commandItem);

        QTableWidgetItem *hexValueItem = new QTableWidgetItem("0x" + hexValue);
        ui->Pdpu_table->setItem(row, 1, hexValueItem);
    }

}

void MainWindow::sendHexToSerialPort(const QString& hexValue)
{

    QByteArray byteArray = QByteArray::fromHex(hexValue.toLatin1());
    ptrPort->write(byteArray);
}




void MainWindow::on_PDPU_DISCONNECT_clicked()
{
    if (ptrPort->isOpen()) {
        ptrPort->close();
        QMessageBox::information(this, "Port Disconnected", "Serial port has been disconnected.");
    } else {
        QMessageBox::information(this, "Port Disconnected", "Serial port is already closed.");
    }
}

void MainWindow::Table_stretech()
{

    ui->Pdpu_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->Pdpu_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    // Set the font for the table header to bold and size 10
    QFont headerFont = ui->Pdpu_table->horizontalHeader()->font();
    headerFont.setBold(true);
    headerFont.setPointSize(10);
    ui->Pdpu_table->horizontalHeader()->setFont(headerFont);

    // Center-align the header text
    for (int i = 0; i < ui->Pdpu_table->columnCount(); ++i) {
        ui->Pdpu_table->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }

}
void MainWindow::handleReceivedData(const QByteArray &data)
{
    accumulatedData.clear();
    first12Bytes.clear();
    dataChunk.clear();
    last2Bytes.clear();
    Measured_Value.clear();

    accumulatedData.append(data);

    if (accumulatedData.size() == 5) {
        accumulatedData.chop(1);
        QString hexValue = QString::fromLatin1(accumulatedData.toHex().toUpper());

        union ulf1
        {
            unsigned long ul;
            float recving_data_voltages ;
        };


        ulf1 u1;
        std::string str1 = hexValue.toStdString();
        std::stringstream ss1(str1);
        ss1 >> std::hex >> u1.ul;
        float recving_data = u1.recving_data_voltages;

        qDebug()<<"float"<<recving_data<<endl;

        Measured_Value.append(QString::number(recving_data));
        populate_in_pdpu();


        QString combinedValues;
        for (const QString &value : Measured_Value) {
            combinedValues += value + " ";
        }


        combinedValues = combinedValues.trimmed();

        ui->PDPU_responce->setText(combinedValues);
        qDebug() << "Measured_Value vector contents:";
        for (int i = 0; i < Measured_Value.size(); ++i) {
            qDebug() << "Index:" << i << "Value:" << Measured_Value[i];
        }
        //populate_in_pdpu();




    }else{
        if(accumulatedData.size() == 4)
        {
            QString hexValue = QString::fromLatin1(accumulatedData.toHex().toUpper());
            bool ok;
            int intValue = hexValue.toInt(&ok, 16);
            if (ok) {
                qDebug()<<"integer vLUE"<<intValue<<endl;
                QString integer_value = QString::number(intValue);
                ui->PDPU_responce->setText(integer_value + "ms");
                Measured_Value.append(integer_value + "ms");
                populate_in_pdpu();

            }

        }else{
            if(accumulatedData.size() == 43){
                   accumulatedData.remove(0, 4);

                   if (accumulatedData.size() >= 39) {


                       for (int i = 0; i < 36; ++i) {
                           QByteArray chunk = accumulatedData.mid(i, 1);
                           QString hexValue = QString::fromLatin1(chunk.toHex().toUpper());
                           bool ok;
                           int intValue = hexValue.toInt(&ok, 16);
                           if (ok) {
                               int intvaluess = intValue-127;
                                double result = static_cast<double>(intvaluess) / 2.82;
                               // QString number = QString::number(result);
                                QString number = QString::number(result, 'f', 2);
                                Measured_Value.append(number);


                           }

                       }

                       chunk37 = accumulatedData.mid(36, 1);
                       QString hexValue37 = QString::fromLatin1(chunk37.toHex().toUpper());

                       bool ok;
                       int decimalValue37 = hexValue37.toInt(&ok, 16);

                       if (ok) {
                           double result37 = static_cast<double>(decimalValue37);
                           result_string37 = QString::number(result37);


                           Measured_Value.append(result_string37);


                       } else {
                           //qDebug() << "Conversion from hexadecimal to integer failed for chunk 37: " << hexValue37;
                       }

                       accumulatedData.remove(0, 2);




                   }
                   dataChunk.clear();
                   first12Bytes.clear();
                   accumulatedData.clear();
                   chunk37.clear();
                   populate_without_button();
                   populate_in_label_for_sensor();



            }else{
                if(accumulatedData.size() >= 52){
                    qDebug()<<"data_is_52"<<endl;
                    accumulatedData.remove(0, 4);


                    if (accumulatedData.size() >= 48) {



                        if (accumulatedData.size() >= 45) {
                            for (int j = 0; j < 12; ++j) {
                                chunk = accumulatedData.mid(j, 1);
                                QString hexValue = QString::fromLatin1(chunk.toHex().toUpper());


                                bool ok;
                                int intValue = hexValue.toInt(&ok, 16);
                                if (ok) {

                                        int intvaluess = intValue-127;
                                         double result = static_cast<double>(intvaluess) / 2.82;
                                        // QString number = QString::number(result);
                                         QString number = QString::number(result, 'f', 2);
                                         Measured_Value.append(number);



                                }
                            }

                            for (int k = 12; k < 15; ++k) {
                                chunk = accumulatedData.mid(k, 1);
                                QString hexValue = QString::fromLatin1(chunk.toHex().toUpper());
                                bool ok;
                                int intValue = hexValue.toInt(&ok, 16);
                                if (ok) {

                                    double dec = (intValue*3.3)/256;
                                    double roundedResult = std::round(dec * 100) / 100.0;
                                    QString number = QString::number(roundedResult);
                                    Measured_Value.append(number);

                                }
                            }

                            for (int k = 15; k < 27; ++k) {
                                chunk = accumulatedData.mid(k, 1);
                                QString hexValue = QString::fromLatin1(chunk.toHex().toUpper());
                                bool ok;
                                int intValue = hexValue.toInt(&ok, 16);
                                if (ok) {
                                    int intvaluess = intValue-127;
                                     double result = static_cast<double>(intvaluess) / 2.82;
                                      QString number = QString::number(result, 'f', 2);
                                     Measured_Value.append(number);

                                }

                            }
                            for (int k = 27; k < 30; ++k) {
                                chunk = accumulatedData.mid(k, 1);
                                QString hexValue = QString::fromLatin1(chunk.toHex().toUpper());
                                bool ok;
                                int intValue = hexValue.toInt(&ok, 16);
                                if (ok) {
                                    double dec = (intValue*3.3)/256;
                                    double roundedResult = std::round(dec * 100) / 100.0;
                                    QString number = QString::number(roundedResult);
                                    Measured_Value.append(number);

                                }
                            }
                            for (int k = 30; k < 42; ++k) {
                                chunk = accumulatedData.mid(k, 1);
                                QString hexValue = QString::fromLatin1(chunk.toHex().toUpper());
                                bool ok;
                                int intValue = hexValue.toInt(&ok, 16);
                                if (ok) {
                                    int intvaluess = intValue-127;
                                     double result = static_cast<double>(intvaluess) / 2.82;
                                     QString number = QString::number(result, 'f', 2);
                                     Measured_Value.append(number);

                                }


                            }

                            for (int k = 42; k < 45; ++k) {
                                chunk = accumulatedData.mid(k, 1);
                                QString hexValue = QString::fromLatin1(chunk.toHex().toUpper());
                                bool ok;
                                int intValue = hexValue.toInt(&ok, 16);
                                if (ok) {

                                    double dec = (intValue*3.3)/256;
                                    double roundedResult = std::round(dec * 100) / 100.0;
                                    QString number = QString::number(roundedResult);
                                    Measured_Value.append(number);

                                }
                            }
                        }


                        chunk37 = accumulatedData.mid(45, 1);
                        QString hexValue37 = QString::fromLatin1(chunk37.toHex().toUpper());

                        bool ok;
                        int decimalValue37 = hexValue37.toInt(&ok, 16);

                        if (ok) {
                            double result37 = static_cast<double>(decimalValue37);
                            result_string37 = QString::number(result37);


                            Measured_Value.append(result_string37);


                        } else {
                            //qDebug() << "Conversion from hexadecimal to integer failed for chunk 37: " << hexValue37;
                        }

                        accumulatedData.remove(0, 2);
                        dataChunk.clear();
                        first12Bytes.clear();
                        accumulatedData.clear();
                        chunk37.clear();
                        chunk.clear();
                        populate_in_label_for_dau();
                        populate_in_dau();

                    }


                }else{
                    if(accumulatedData.size() == 6){
                         Measured_Value.append("FAIL");
                          Measured_Value.last() = Measured_Value.last().toUpper();

                         populate_in_pdpu();

                    }
                }
            }

            }//80
        }
    }

void MainWindow::populate_without_button()
{

//    QTableWidget* tableWidget = ui->sensor_card;
//    int columnCount = 37; // Number of columns in the table
//    QFont font;
//    font.setBold(true);
//    font.setPointSize(10);
//    font.setFamily("Times New Roman");
//    int vectorIndex = 0;

//    while (vectorIndex < Measured_Value.size()) {
//        int row = tableWidget->rowCount();

//        tableWidget->setRowCount(row + 1);


//        for (int col = 0; col < columnCount && vectorIndex < Measured_Value.size(); ++col) {
//            QTableWidgetItem* item = new QTableWidgetItem();
//            item->setFont(font);
//            item->setTextAlignment(Qt::AlignCenter);

//            QString cellValue = Measured_Value[vectorIndex];
//            tableWidget->setItem(row, col, new QTableWidgetItem(cellValue));
//            vectorIndex++;
//        }
//    }
    QTableWidget* tableWidget = ui->sensor_card;
    int columnCount = 37; // Number of columns in the table
    QFont font;
    font.setBold(true);
    font.setPointSize(10);
    font.setFamily("Times New Roman");
    int vectorIndex = 0;

    while (vectorIndex < Measured_Value.size()) {
        int row = tableWidget->rowCount();

        tableWidget->setRowCount(row + 1);

        for (int col = 0; col < columnCount && vectorIndex < Measured_Value.size(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setFont(font);
            item->setTextAlignment(Qt::AlignCenter);

            QString cellValue = Measured_Value[vectorIndex];

            // Check if cellValue is "fail" and update all columns if true
            if (cellValue.toUpper() == "FAIL") {
                ui->label_x_sen->setText("FAIL");
                ui->label_y_sen->setText("FAIL");
                ui->label_z_sen->setText("FAIL");
                ui->label_Temp_sen->setText("FAIL");
                for (int i = 0; i < columnCount; ++i) {
                    tableWidget->setItem(row, i, new QTableWidgetItem("fail"));
                }
            } else {
                tableWidget->setItem(row, col, new QTableWidgetItem(cellValue));
            }

            vectorIndex++;
        }
    }

}

void MainWindow::populate_in_dau()
{
//    QTableWidget* tableWidget = ui->dau_table;
//    int columnCount = 46; // Number of columns in the table
//    QFont font;
//    font.setBold(true);
//    font.setPointSize(10);
//    font.setFamily("Times New Roman");
//    int vectorIndex = 0;

//    while (vectorIndex < Measured_Value.size()) {
//        int row = tableWidget->rowCount();
//        // Add a new row to the table
//        tableWidget->setRowCount(row + 1);

//        // Display all 46 values in the current row
//        for (int col = 0; col < columnCount && vectorIndex < Measured_Value.size(); ++col) {
//            QTableWidgetItem* item = new QTableWidgetItem();
//            item->setFont(font);
//            item->setTextAlignment(Qt::AlignCenter);

//            QString cellValue = Measured_Value[vectorIndex];
//            tableWidget->setItem(row, col, new QTableWidgetItem(cellValue));
//            vectorIndex++;
//        }
//    }
    QTableWidget* tableWidget = ui->dau_table;
    int columnCount = 46; // Number of columns in the table
    QFont font;
    font.setBold(true);
    font.setPointSize(10);
    font.setFamily("Times New Roman");
    int vectorIndex = 0;

    while (vectorIndex < Measured_Value.size()) {
        int row = tableWidget->rowCount();
        // Add a new row to the table
        tableWidget->setRowCount(row + 1);

        // Display all 46 values in the current row
        for (int col = 0; col < columnCount && vectorIndex < Measured_Value.size(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setFont(font);
            item->setTextAlignment(Qt::AlignCenter);

            QString cellValue = Measured_Value[vectorIndex];


            if (cellValue.toUpper() == "FAIL") {
                ui->label_x_dau->setText("FAIL");
                ui->label_y_dau->setText("FAIL");
                ui->label_z_dau->setText("FAIL");
                ui->label_Temp_dau->setText("FAIL");
                ui->label_adc1_dau->setText("FAIL");
                ui->label_adc2_dau->setText("FAIL");
                ui->label_adc3_dau->setText("FAIL");
                for (int i = 0; i < columnCount; ++i) {
                    tableWidget->setItem(row, i, new QTableWidgetItem("FAIL"));
                }
            } else {
                tableWidget->setItem(row, col, new QTableWidgetItem(cellValue));
            }

            vectorIndex++;
        }
    }


}


void MainWindow::populate_in_label_for_sensor()
{
    QFont font;
    font.setBold(true);
    font.setPointSize(10);
    font.setFamily("Times New Roman");


    int dataSize = Measured_Value.size();

    if (dataSize >= 37) {
        for (int i = 33; i <= 36; ++i) {
            int valueIndex = dataSize - 37 + i;

            if (valueIndex >= 0 && valueIndex < dataSize) {
                QString cellValue = Measured_Value[valueIndex];

                QLabel* label = nullptr;

                switch (i) {
                case 33:
                    label = ui->label_x_sen;
                    break;
                case 34:
                    label = ui->label_y_sen;
                    break;
                case 35:
                    label = ui->label_z_sen;
                    break;
                case 36:
                    label = ui->label_Temp_sen;
                    break;
                }

                if (label) {
                    label->setFont(font);
                    label->setAlignment(Qt::AlignCenter);
                    label->setText(cellValue);
                }
            }
        }
    }
}
void MainWindow::populate_in_label_for_dau()
{
    int dataSize = Measured_Value.size();

    if (dataSize >= 46) { // Check if you have enough data
        for (int i = 45; i >= 39; --i) { // Iterate from 45 down to 39
            if (i < dataSize) {
                QString cellValue = Measured_Value[i];

                // Assuming you have QLabel widgets named label_0 to label_6 in your UI
                QLabel* label = nullptr;
                switch (45 - i) {
                case 0:
                    label = ui->label_Temp_dau;
                    break;
                case 1:
                    label = ui->label_adc3_dau;
                    break;
                case 2:
                    label = ui->label_adc2_dau;
                    break;
                case 3:
                    label = ui->label_adc1_dau;
                    break;
                case 4:
                    label = ui->label_z_dau;
                    break;
                case 5:
                    label = ui->label_y_dau;
                    break;
                case 6:
                    label = ui->label_x_dau;
                    break;
                }

                if (label) {
                    label->setText(cellValue);
                } else {
                    qDebug() << "Label not found for index " << i;
                }
            } else {
                qDebug() << "Index " << i << " is out of range for the Measured_Value vector.";
            }
        }
    }

}


void MainWindow::handle_fuze(const QByteArray &data)
{

    acuumalted_fuze.clear();
    measured_value_fuze.clear();
    acuumalted_fuze.append(data);

    if(acuumalted_fuze.size() > 0){
        qDebug()<<"greater than  0"<<endl;

        QString hexValue = QString::fromLatin1(acuumalted_fuze.toHex().toUpper());
        qDebug()<<"hex_valuee____________"<<hexValue<<endl;

        measured_value_fuze.append(hexValue);
        ui->Fuze_responce->setText("0x" + hexValue);
        populate_infuze();

    }else{
        if(accumulatedData.size() == 6){
            measured_value_fuze.push_back("FAIL");
            populate_infuze();

        }
    }



}
void MainWindow::empty()
{
    QTableWidget* tableWidget = ui->Pdpu_table;
    int rowCount = tableWidget->rowCount();
    int columnToCheck = 2; // Column index 2 for the third column

    for (int rowIndex = 0; rowIndex < rowCount; ++rowIndex) {
        QTableWidgetItem* item = tableWidget->item(rowIndex, columnToCheck);

        // Check if the item in the specified column is empty
        if (!item || item->text().isEmpty()) {
            // If the item is not set or its text is empty, set "Not Available"
            QTableWidgetItem* notAvailableItem = new QTableWidgetItem("Not Available");
            tableWidget->setItem(rowIndex, columnToCheck, notAvailableItem);
        }
    }

}






void MainWindow::populate_in_pdpu()
{

    //    QTableWidget* tableWidget = ui->Pdpu_table;
    //    QFont font;
    //    font.setBold(true);
    //    font.setPointSize(10);

    //    int rowCount = tableWidget->rowCount();
    //    int columnToCheck = 2; // Column index to check (starting from 0)




    //        for (int rowIndex = 0; rowIndex < rowCount && rowIndex < Measured_Value.size(); ++rowIndex) {
    //            for (int existingRowIndex = 0; existingRowIndex < rowCount; ++existingRowIndex) {
    //                QTableWidgetItem* existingItem = tableWidget->item(existingRowIndex, columnToCheck);
    //                if (!existingItem) {
    //                    // If the item in the specified column is not set, create a new item
    //                    QTableWidgetItem* newItem = new QTableWidgetItem();
    //                    newItem->setFont(font);
    //                    newItem->setTextAlignment(Qt::AlignCenter);
    //                    newItem->setBackground(QBrush(Qt::green));
    //                    newItem->setForeground(QBrush(Qt::black));
    //                    newItem->setTextAlignment(Qt::AlignCenter);
    //                    tableWidget->setItem(existingRowIndex, columnToCheck, newItem);

    //                    QString cellValue = Measured_Value[rowIndex];
    //                    qDebug() << "cellll_valuee" << cellValue << endl;

    //                    newItem->setText(cellValue);
    //                    break; // Break after setting the value in the first empty row
    //                }
    //            }
    //        }
    /////////////////////////pr code//////////////////////////////////////////////
    QTableWidget* tableWidget = ui->Pdpu_table;
    QFont font;
    font.setBold(true);
    font.setPointSize(10);

    int rowCount = tableWidget->rowCount();
    int columnToCheck = 2; // Column index to check (starting from 0)

    for (int rowIndex = rowCount - 1; rowIndex >= 0 && rowIndex >= (rowCount - Measured_Value.size()); --rowIndex) {
        QTableWidgetItem* existingItem = tableWidget->item(rowIndex, columnToCheck);
        if (!existingItem) {
            // If the item in the specified column is not set, create a new item
            QTableWidgetItem* newItem = new QTableWidgetItem();
            newItem->setFont(font);
            newItem->setTextAlignment(Qt::AlignCenter);
            //newItem->setBackground(QBrush(Qt::green));
            newItem->setForeground(QBrush(Qt::black));
            newItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(rowIndex, columnToCheck, newItem);

            QString cellValue = Measured_Value[rowIndex - (rowCount - Measured_Value.size())];
            qDebug() << "cellll_valuee" << cellValue << endl;

            newItem->setText(cellValue);
            if(cellValue.toUpper() == "FAIL"){
                ui->PDPU_responce->setText("FAIL");
            }
            break; // Break after setting the value in the last row
        }
    }











}





//void MainWindow::populate_infuze()
//{
//    QTableWidget* tableWidget = ui->Pdpu_table;
//    QFont font;
//    font.setBold(true);
//    font.setPointSize(10);

//    int rowCount = tableWidget->rowCount();
//    int columnToCheck = 2; // Column index to check (starting from 0)








//        for (int rowIndex = 0; rowIndex < rowCount && rowIndex < measured_value_fuze.size(); ++rowIndex) {
//            for (int existingRowIndex = 0; existingRowIndex < rowCount; ++existingRowIndex) {
//                QTableWidgetItem* existingItem = tableWidget->item(existingRowIndex, columnToCheck);
//                if (!existingItem) {
//                    // If the item in the specified column is not set, create a new item
//                    QTableWidgetItem* newItem = new QTableWidgetItem();
//                    newItem->setFont(font);
//                    newItem->setTextAlignment(Qt::AlignCenter);
//                    newItem->setBackground(QBrush(Qt::green));
//                    newItem->setForeground(QBrush(Qt::black));
//                    newItem->setTextAlignment(Qt::AlignCenter);
//                    tableWidget->setItem(existingRowIndex, columnToCheck, newItem);

//                    QString cellValue = measured_value_fuze[rowIndex];
//                    qDebug() << "cellll_valuee" << cellValue << endl;

//                    newItem->setText(cellValue);
//                    break; // Break after setting the value in the first empty row
//                }
//            }
//        }

//}
void MainWindow::populate_infuze()
{
    QTableWidget* tableWidget = ui->Pdpu_table;
    QFont font;
    font.setBold(true);
    font.setPointSize(10);

    int rowCount = tableWidget->rowCount();
    int columnToCheck = 2; // Column index to check (starting from 0)

    for (int rowIndex = rowCount - 1; rowIndex >= 0 && rowIndex >= (rowCount - measured_value_fuze.size()); --rowIndex) {
        QTableWidgetItem* existingItem = tableWidget->item(rowIndex, columnToCheck);
        if (!existingItem) {
            // If the item in the specified column is not set, create a new item
            QTableWidgetItem* newItem = new QTableWidgetItem();
            newItem->setFont(font);
            newItem->setTextAlignment(Qt::AlignCenter);
            //newItem->setBackground(QBrush(Qt::green));
            newItem->setForeground(QBrush(Qt::black));
            newItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(rowIndex, columnToCheck, newItem);

            QString cellValue = measured_value_fuze[rowIndex - (rowCount - measured_value_fuze.size())];
            qDebug() << "cellll_valuee" << cellValue << endl;

            newItem->setText(cellValue);
            if(cellValue.toUpper() == "FAIL"){
                ui->Fuze_responce->setText("FAIL");


            }
            break; // Break after setting the value in the last row
        }
    }

    //    for (int rowIndex = 0; rowIndex < rowCount; ++rowIndex) {
    //            QTableWidgetItem* targetItem = tableWidget->item(rowIndex, columnToCheck);

    //            // Check if the target column is empty
    //            if (!targetItem || targetItem->text().isEmpty()) {
    //                // If the item in the specified column is not set or empty, set "Not Available"
    //                QTableWidgetItem* newItem = new QTableWidgetItem("Not Available");
    //                newItem->setFont(font);
    //                newItem->setTextAlignment(Qt::AlignCenter);
    //                newItem->setBackground(QBrush(Qt::red));
    //                newItem->setForeground(QBrush(Qt::black));
    //                newItem->setTextAlignment(Qt::AlignCenter);
    //                tableWidget->setItem(rowIndex, columnToCheck, newItem);
    //            }
    //        }
}

void MainWindow::emptyy(){

    QTableWidget* tableWidget = ui->Pdpu_table;
    int rowCount = tableWidget->rowCount();
    int columnToCheck = 2;
    QFont font;
    font.setBold(true);
    font.setPointSize(10);


    for (int rowIndex = 0; rowIndex < rowCount; ++rowIndex) {
        QTableWidgetItem* targetItem = tableWidget->item(rowIndex, columnToCheck);

        // Check if the target column is empty
        if (!targetItem || targetItem->text().isEmpty()) {
            // If the item in the specified column is not set or empty, set "Not Available"
            QTableWidgetItem* newItem = new QTableWidgetItem("Not Available");
            newItem->setFont(font);
            newItem->setTextAlignment(Qt::AlignCenter);
            newItem->setBackground(QBrush(Qt::red));
            newItem->setForeground(QBrush(Qt::black));
            newItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(rowIndex, columnToCheck, newItem);
        }
    }


}









void MainWindow::on_send_fuze_command_clicked()
{
     time->start(1000);
    fuze->stop();
    recive->stop();

    fuze->stopped = false;
    fuze->start();
    fuze->setPriority(QThread::HighestPriority);





    QMap<int, QString> indexToHexMap = {
        {0, "093501FFC252"}, //5
        {1, "093501FFC24E"}, //5
        {2, "093506FFC552"}, //25
        {3, "093506FFC54E"}, //25
        {4, "093502FFC152"}, //25
        {5, "093502FFC14E"}, //25
        {6, "093503FFC052"}, //4
        {7, "093503FFC04E"}, //4
        {8, "093504FFC752"}, //5
        {9, "093504FFC74E"}, //5
        {10, "093505FFC652"}, //4
        {11, "093505FFC64E"}, //4



    };
    QString selectedCommandName = ui->Fuze_combobox_->currentText();

    int selectedIndex = ui->Fuze_combobox_->currentIndex();


    if (indexToHexMap.contains(selectedIndex)) {
        QString hexValue = indexToHexMap.value(selectedIndex);
        sendHexToSerialPort(hexValue);

        int row = ui->Pdpu_table->rowCount();
        ui->Pdpu_table->insertRow(row);


        QTableWidgetItem *commandItem = new QTableWidgetItem(selectedCommandName);
        ui->Pdpu_table->setItem(row, 0, commandItem);

        QTableWidgetItem *hexValueItem = new QTableWidgetItem("0x" + hexValue);
        ui->Pdpu_table->setItem(row, 1, hexValueItem);
    }

}

void MainWindow::on_SAVE_IMU_2_clicked()
{
    saving_for_making_pdf();

}

QString MainWindow::generateHTMLReport_for_pdpu(QTableWidget *tableWidget3)
{
    QString htmlContent;

    QTextStream stream(&htmlContent);
    stream << "<html>\n"
           << "<head>\n"
           << "<meta Content=\"Text/html; charset=Windows-1251\">\n"
           << "<title>Your Report Title</title>\n"
           << "</head>\n"
           << "<body>\n"

              "<h1 align='center'>" + unit_name + "</h1>\n";


    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    QString formattedDate = currentDate.toString("dd/MM/yyyy");
    QString formattedTime = currentTime.toString("hh:mm AP");


    stream << "<p><b>Date: " << formattedDate << "</b></p>\n"
           << "<p><b>Time: " << formattedTime << "</b></p>\n"
           << "<p><b>Project-Name: " << Porject_name << "</b></p>\n"
           << "<p><b>Unit-Serial-No: " << serial_number << "</b></p>\n"
           << "<p><b>Unit-Name: " << unit_name << "</b></p>\n" // Add cable name
           << "<p><b>Test-Type: " << test_type << "</b></p>\n<br>"

           << "<table width='100%' border='1' cellspacing='0' cellpadding='2'>\n"
           << "<thead><tr bgcolor='#f0f0f0'>"
           << "<th>SerialNo</th>"                          //  << "<th style='font-size: 12px;'>SerialNo</th>"
           <<"<th>COMMAND NAME</th>"
          <<"<th>SEQUENCE</th>"
         <<"<th>RESPONCE</th>"

        << "</tr></thead>\n"
        << "<body>";
    int serialNo = 1;
    for (int row = 0; row < tableWidget3->rowCount(); ++row)
    {
        stream << "<tr>";
        // stream << "<td align='center' style='font-size: 10px;'>" << serialNo << "</td>";
        stream << "<td align='center'style='font-size: 10px;'>" << serialNo << "</td>";
        serialNo++;

        for (int col = 0; col < tableWidget3->columnCount(); ++col)
        {
            QTableWidgetItem *item = tableWidget3->item(row, col);
            QString cellValue = item ? item->text() : "";
            QString cellAlign = "center"; // Center-align cell values
            QString bgColor, textColor;



            // Apply background color and text color
            stream << "<td align='" << cellAlign << "' style='background-color:" << bgColor
                   << "; color:" << textColor << ";'>" << cellValue << "</td>";
        }

        stream << "</tr>\n";
    }



    stream << "</body>\n"
           << "</table>"
           << "</body>\n"
           << "</html>";

    return htmlContent;
}
QString MainWindow::generateHTMLFor_sensor_card(QTableWidget *tableWidget2)
{
    QString htmlContent;

    QTextStream stream(&htmlContent);
    stream << "<html>\n"
           << "<head>\n"
           << "<meta Content=\"Text/html; charset=Windows-1251\">\n"
           << "<title>Your Report Title</title>\n"
           << "</head>\n"
           << "<body>\n"
           << "<h1 align='center'>Sensor Card</h1>\n" // Add the h1 label in the middle
           << "<table width='100%' border='1' cellspacing='0' cellpadding='2'>\n"
           << "<thead><tr bgcolor='#f0f0f0'>"
           << "<th>Serial No</th>"
           << "<th>Sensor Type</th>"
           << "<th>A11x(g)</th>"
           << "<th>A11Y(g)</th>"
           << "<th>A11Z(g)</th>"
           << "<th>A12X(g)</th>"
           << "<th>A12Y(g)</th>"
           << "<th>A12Z(g)</th>"
           << "<th>A13X(g)</th>"
           << "<th>A13Y(g)</th>"
           << "<th>A13Z(g)</th>"
           << "<th>A14X(g)</th>"
           << "<th>A14Y(g)</th>"
           << "<th>A14Z(g)</th>"
           << "<th>A21X(g)</th>"
           << "<th>A21Y(g)</th>"
           << "<th>A21Z(g)</th>"
           << "<th>A22X(g)</th>"
           << "<th>A22Y(g)</th>"
           << "<th>A22Z(g)</th>"
           << "<th>A23X(g)</th>"
           << "<th>A23Y(g)</th>"
           << "<th>A23Z(g)</th>"
           << "<th>A24X(g)</th>"
           << "<th>A24Y(g)</th>"
           << "<th>A24Z(g)</th>"
           << "<th>A31X(g)</th>"
           << "<th>A31Y(g)</th>"
           << "<th>A31Z(g)</th>"
           << "<th>A32X(g)</th>"
           << "<th>A32Y(g)</th>"
           << "<th>A32Z(g)</th>"
           << "<th>A33X(g)</th>"
           << "<th>A33Y(g)</th>"
           << "<th>A33Z(g)</th>"
           << "<th>A34X(g)</th>"
           << "<th>A34Y(g)</th>"
           << "<th>A34Z(g)</th>"
           << "<th>Temp\xB0 C</th>"
           << "</tr></thead>\n"
           << "<tbody>";

    for (int row = 0; row < tableWidget2->rowCount(); ++row)
    {
        stream << "<tr>";

        // Add the serial number starting from 1
        stream << "<td align='center'>" << QString::number(row + 1) << "</td>";


        // Add the sensor type alternating between "Sensor 1" and "Sensor 2"
        QString sensorType =  "Sensor 1" ;
        stream << "<td align='center'>" << sensorType << "</td>";

        for (int col = 0; col < tableWidget2->columnCount(); ++col)
        {
            QTableWidgetItem *item = tableWidget2->item(row, col);
            QString cellValue = item ? item->text() : "";
            QString cellAlign = "center"; // Center-align cell values

            // Apply background color and text color (if needed)
            stream << "<td align='" << cellAlign << "'>" << cellValue << "</td>";
        }

        stream << "</tr>\n";
    }

    stream << "</tbody>\n"
           << "</table>\n"
           << "</body>\n"
           << "</html>";

    return htmlContent;
}

QString MainWindow::generateHTMLFor_dau(QTableWidget *tableWidget1)
{
    QString htmlContent;

       QTextStream stream(&htmlContent);
       stream << "<html>\n"
              << "<head>\n"
              << "<meta Content=\"Text/html; charset=Windows-1251\">\n"
              << "<title>Your Report Title</title>\n"
              << "<style>\n"
              << "h1 { font-family: 'Times New Roman', Times, serif; }\n"  // Add font family style
              << "</style>\n"
              << "</head>\n"
              << "<body>\n"
              << "<h1 align='center'>DAU</h1>\n" // Add the h1 label in the middle
              << "<table width='100%' border='1' cellspacing='0' cellpadding='2'>\n"
              << "<thead><tr bgcolor='#f0f0f0'>"
              << "<th>SerialNo</th>"
              << "<th>AC1X(g)</th>"
              << "<th>AC1Y(g)</th>"
              << "<th>AC1Z(g)</th>"
              << "<th>AC12X(g)</th>"
              << "<th>AC12Y(g)</th>"
              << "<th>AC12Z(g)</th>"
              << "<th>AC13X(g)</th>"
              << "<th>AC13Y(g)</th>"
              << "<th>AC13Z(g)</th>"
              << "<th>AC14X(g)</th>"
              << "<th>AC14Y(g)</th>"
              << "<th>AC14Z(g)</th>"
              << "<th>ADC11</th>"
              << "<th>ADC12</th>"
              << "<th>ADC13</th>"
              << "<th>AC21X(g)</th>"
              << "<th>AC21Y(g)</th>"
              << "<th>AC21Z(g)</th>"
              << "<th>AC22X(g)</th>"
              << "<th>AC22Y(g)</th>"
              << "<th>AC22Z(g)</th>"
              << "<th>AC23X(g)</th>"
              << "<th>AC23Y(g)</th>"
              << "<th>AC23Z(g)</th>"
              << "<th>AC24X(g)</th>"
              << "<th>AC24Y(g)</th>"
              << "<th>AC24Z(g)</th>"
              << "<th>ADC21</th>"
              << "<th>ADC22</th>"
              << "<th>ADC23</th>"
              << "<th>AC31X(g)</th>"
              << "<th>AC31Y(g)</th>"
              << "<th>AC31Z(g)</th>"
              << "<th>AC32X(g)</th>"
              << "<th>AC32Y(g)</th>"
              << "<th>AC32Z(g)</th>"
              << "<th>AC33X(g)</th>"
              << "<th>AC33Y(g)</th>"
              << "<th>AC33Z(g)</th>"
              << "<th>AC34X(g)</th>"
              << "<th>AC34Y(g)</th>"
              << "<th>AC34Z(g)</th>"
              << "<th>ADC31</th>"
              << "<th>ADC32</th>"
              << "<th>ADC33</th>"
              << "<th>Temp\xB0 C</th>"
              << "</tr></thead>\n"
              << "<tbody>";

       int serialNo = 1;
       for (int row = 0; row < tableWidget1->rowCount(); ++row)
       {
           stream << "<tr>";
           stream << "<td align='center'>" << serialNo << "</td>";
           serialNo++;

           for (int col = 0; col < tableWidget1->columnCount(); ++col)
           {
               QTableWidgetItem *item = tableWidget1->item(row, col);
               QString cellValue = item ? item->text() : "";
               QString cellAlign = "center"; // Center-align cell values

               // Apply background color and text color
               stream << "<td align='" << cellAlign << "'>" << cellValue << "</td>";
           }

           stream << "</tr>\n";
       }

       stream << "</tbody>\n"
              << "</table>\n"
              << "</body>\n"
              << "</html>";

       return htmlContent;


}


void MainWindow::convertHTMLToPDF_for_PDPU(const QString &htmlContent, const QString &pdfFilePath)
{
    qDebug() << "inside pdf maker" << endl;

    // Create a QTextDocument and set the HTML content
    QTextDocument textDocument;
    textDocument.setHtml(htmlContent);

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(pdfFilePath);

    printer.setOrientation(QPrinter::Landscape);


    printer.setPaperSize(QPrinter::Custom);
    printer.setPaperSize(QSizeF(11.0, 17.0), QPrinter::Inch); // Custom page size in inches
    textDocument.print(&printer);

    // Check if the PDF was generated successfully
    if (printer.printerState() == QPrinter::Error)
    {
        qDebug() << "Failed to generate PDF.";
    }
    else
    {
        QMessageBox::information(this, "Status", "PDF Generated Successfully");
    }
}





void MainWindow::on_Stop_Pdpu_clicked()
{
    measured_value_fuze.clear();
    another_vectore_fuze.clear();
    Another_Vector.clear();
    Measured_Value.clear();
    fuze->stop();
    recive->stop();
    time->stop();

}

void MainWindow::on_CLEAR_IMU_2_clicked()
{
    measured_value_fuze.clear();
    another_vectore_fuze.clear();
    Another_Vector.clear();
    Measured_Value.clear();
    ui->PDPU_responce->clear();
    ui->Fuze_responce->clear();
    ui->Pdpu_table->clearContents();
    ui->Pdpu_table->setRowCount(0);

}

void MainWindow::on_Delete_clicked()
{
    QTableWidget* tableWidget = ui->Pdpu_table;

    if (tableWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(this, "No Row Selected", "Please select one or more rows to delete.");
        return;
    }


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete the selected rows?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {

        QList<int> selectedRows;

        foreach (QTableWidgetItem* item, tableWidget->selectedItems())
        {
            int row = item->row();
            if (!selectedRows.contains(row))
            {
                selectedRows.append(row);
            }
        }


        std::sort(selectedRows.begin(), selectedRows.end(), std::greater<int>());


        foreach (int row, selectedRows)
        {
            tableWidget->removeRow(row);
        }
    }
}

void MainWindow::saving_for_making_pdf()

{
    QString cabble = ui->PDPU_unit_name->text();
    QString sr = ui->PDPU_serial->text();
    QString pr = ui->PDPU_NAME->text();

    Porject_name = pr;
    serial_number = sr;
    unit_name = cabble; // Store the unit name for later use

    QString new_test = ui->PDPU_type->currentText();
    test_type = new_test;
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    // Construct the PDF file name using the unit name and timestamp
    QString pdfFileName = "D:/Test-report/" + cabble + "-command-report" + timestamp + ".pdf";

    // Show a file dialog to allow the user to choose where to save the PDF
    QString pdfFilePath = QFileDialog::getSaveFileName(this, "Save PDF File", pdfFileName, "PDF Files (*.pdf)");


    if (!pdfFilePath.isEmpty()) // Check if the user canceled the dialog
    {
        // Assuming ui->MY_table is your QTableWidget
        QString htmlContent = generateHTMLReport_for_pdpu(ui->Pdpu_table);
        QString htmlContentTable2 = generateHTMLFor_dau(ui->dau_table);
        QString htmlContentTable3 = generateHTMLFor_sensor_card(ui->sensor_card);
        QString combinedHtmlContent = htmlContent + htmlContentTable2 + htmlContentTable3;
        convertHTMLToPDF_for_PDPU(combinedHtmlContent, pdfFilePath);
    }
}



void MainWindow::on_save_for_both_clicked()
{
    saving_for_making_pdf();
}

void MainWindow::on_clear_for_btoh_clicked()
{


    ui->dau_table->clearContents();
    ui->dau_table->setRowCount(0);
    ui->sensor_card->clearContents();
    ui->sensor_card->setRowCount(0);

    ui->label_x_sen->clear();
    ui->label_y_sen->clear();
    ui->label_z_sen->clear();
    ui->label_Temp_sen->clear();

    ui->label_x_dau->clear();
    ui->label_y_dau->clear();
    ui->label_z_dau->clear();
    ui->label_Temp_dau->clear();

    ui->label_adc1_dau->clear();
    ui->label_adc2_dau->clear();
    ui->label_adc3_dau->clear();


}

void MainWindow::on_send_for_both_clicked()
 {
    recive->stop();
    recive->stoppedd = false;
    recive->start();

    QMap<int, QString> indexToHexMap = {
        {0, "171913FF"},
        {1, "171914FF"},




    };
    QString selectedCommandName = ui->Combo_for_dau_sensor->currentText();

    int selectedIndex = ui->Combo_for_dau_sensor->currentIndex();


    if (indexToHexMap.contains(selectedIndex)) {
        QString hexValue = indexToHexMap.value(selectedIndex);
        sendHexToSerialPort(hexValue);

    }





}

void MainWindow::on_stop_for_both_clicked()
{
    measured_value_fuze.clear();
    another_vectore_fuze.clear();
    Another_Vector.clear();
    Measured_Value.clear();
    fuze->stop();
    recive->stop();
}
