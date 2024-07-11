/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[20];
    char stringdata0[384];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 16), // "dataReceived_imu"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 4), // "data"
QT_MOC_LITERAL(4, 34, 23), // "on_PDPU_REFRESH_clicked"
QT_MOC_LITERAL(5, 58, 23), // "on_PDPU_CONNECT_clicked"
QT_MOC_LITERAL(6, 82, 26), // "on_PDPU_DISCONNECT_clicked"
QT_MOC_LITERAL(7, 109, 22), // "on_SENDCOMMAND_clicked"
QT_MOC_LITERAL(8, 132, 28), // "on_send_fuze_command_clicked"
QT_MOC_LITERAL(9, 161, 21), // "on_SAVE_IMU_2_clicked"
QT_MOC_LITERAL(10, 183, 20), // "on_Stop_Pdpu_clicked"
QT_MOC_LITERAL(11, 204, 22), // "on_CLEAR_IMU_2_clicked"
QT_MOC_LITERAL(12, 227, 17), // "on_Delete_clicked"
QT_MOC_LITERAL(13, 245, 24), // "on_save_for_both_clicked"
QT_MOC_LITERAL(14, 270, 25), // "on_clear_for_btoh_clicked"
QT_MOC_LITERAL(15, 296, 24), // "on_send_for_both_clicked"
QT_MOC_LITERAL(16, 321, 24), // "on_stop_for_both_clicked"
QT_MOC_LITERAL(17, 346, 18), // "handleReceivedData"
QT_MOC_LITERAL(18, 365, 11), // "handle_fuze"
QT_MOC_LITERAL(19, 377, 6) // "emptyy"

    },
    "MainWindow\0dataReceived_imu\0\0data\0"
    "on_PDPU_REFRESH_clicked\0on_PDPU_CONNECT_clicked\0"
    "on_PDPU_DISCONNECT_clicked\0"
    "on_SENDCOMMAND_clicked\0"
    "on_send_fuze_command_clicked\0"
    "on_SAVE_IMU_2_clicked\0on_Stop_Pdpu_clicked\0"
    "on_CLEAR_IMU_2_clicked\0on_Delete_clicked\0"
    "on_save_for_both_clicked\0"
    "on_clear_for_btoh_clicked\0"
    "on_send_for_both_clicked\0"
    "on_stop_for_both_clicked\0handleReceivedData\0"
    "handle_fuze\0emptyy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  102,    2, 0x08 /* Private */,
       5,    0,  103,    2, 0x08 /* Private */,
       6,    0,  104,    2, 0x08 /* Private */,
       7,    0,  105,    2, 0x08 /* Private */,
       8,    0,  106,    2, 0x08 /* Private */,
       9,    0,  107,    2, 0x08 /* Private */,
      10,    0,  108,    2, 0x08 /* Private */,
      11,    0,  109,    2, 0x08 /* Private */,
      12,    0,  110,    2, 0x08 /* Private */,
      13,    0,  111,    2, 0x08 /* Private */,
      14,    0,  112,    2, 0x08 /* Private */,
      15,    0,  113,    2, 0x08 /* Private */,
      16,    0,  114,    2, 0x08 /* Private */,
      17,    1,  115,    2, 0x0a /* Public */,
      18,    1,  118,    2, 0x0a /* Public */,
      19,    0,  121,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void, QMetaType::QByteArray,    3,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataReceived_imu((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 1: _t->on_PDPU_REFRESH_clicked(); break;
        case 2: _t->on_PDPU_CONNECT_clicked(); break;
        case 3: _t->on_PDPU_DISCONNECT_clicked(); break;
        case 4: _t->on_SENDCOMMAND_clicked(); break;
        case 5: _t->on_send_fuze_command_clicked(); break;
        case 6: _t->on_SAVE_IMU_2_clicked(); break;
        case 7: _t->on_Stop_Pdpu_clicked(); break;
        case 8: _t->on_CLEAR_IMU_2_clicked(); break;
        case 9: _t->on_Delete_clicked(); break;
        case 10: _t->on_save_for_both_clicked(); break;
        case 11: _t->on_clear_for_btoh_clicked(); break;
        case 12: _t->on_send_for_both_clicked(); break;
        case 13: _t->on_stop_for_both_clicked(); break;
        case 14: _t->handleReceivedData((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 15: _t->handle_fuze((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 16: _t->emptyy(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MainWindow::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::dataReceived_imu)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::dataReceived_imu(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
