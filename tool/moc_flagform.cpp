/****************************************************************************
** Meta object code from reading C++ file 'flagform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "flagform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'flagform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FlagForm_t {
    QByteArrayData data[15];
    char stringdata[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_FlagForm_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_FlagForm_t qt_meta_stringdata_FlagForm = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 7),
QT_MOC_LITERAL(2, 17, 0),
QT_MOC_LITERAL(3, 18, 7),
QT_MOC_LITERAL(4, 26, 7),
QT_MOC_LITERAL(5, 34, 8),
QT_MOC_LITERAL(6, 43, 9),
QT_MOC_LITERAL(7, 53, 11),
QT_MOC_LITERAL(8, 65, 9),
QT_MOC_LITERAL(9, 75, 19),
QT_MOC_LITERAL(10, 95, 14),
QT_MOC_LITERAL(11, 110, 4),
QT_MOC_LITERAL(12, 115, 12),
QT_MOC_LITERAL(13, 128, 13),
QT_MOC_LITERAL(14, 142, 13)
    },
    "FlagForm\0prevImg\0\0nextImg\0deleImg\0"
    "resetImg\0reviseImg\0skipAttrImg\0submitImg\0"
    "dataGetFromPicBoard\0QList<QPoint>&\0"
    "data\0skipToImgIdx\0estractToPath\0"
    "findImageName\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FlagForm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08,
       3,    0,   70,    2, 0x08,
       4,    0,   71,    2, 0x08,
       5,    0,   72,    2, 0x08,
       6,    0,   73,    2, 0x08,
       7,    0,   74,    2, 0x08,
       8,    0,   75,    2, 0x08,
       9,    1,   76,    2, 0x08,
      12,    0,   79,    2, 0x08,
      13,    0,   80,    2, 0x08,
      14,    0,   81,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FlagForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FlagForm *_t = static_cast<FlagForm *>(_o);
        switch (_id) {
        case 0: _t->prevImg(); break;
        case 1: _t->nextImg(); break;
        case 2: _t->deleImg(); break;
        case 3: _t->resetImg(); break;
        case 4: _t->reviseImg(); break;
        case 5: _t->skipAttrImg(); break;
        case 6: _t->submitImg(); break;
        case 7: _t->dataGetFromPicBoard((*reinterpret_cast< QList<QPoint>(*)>(_a[1]))); break;
        case 8: _t->skipToImgIdx(); break;
        case 9: _t->estractToPath(); break;
        case 10: _t->findImageName(); break;
        default: ;
        }
    }
}

const QMetaObject FlagForm::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FlagForm.data,
      qt_meta_data_FlagForm,  qt_static_metacall, 0, 0}
};


const QMetaObject *FlagForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FlagForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FlagForm.stringdata))
        return static_cast<void*>(const_cast< FlagForm*>(this));
    return QDialog::qt_metacast(_clname);
}

int FlagForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
