/****************************************************************************
** Meta object code from reading C++ file 'dmeventthread.h'
**
** Created: Wed Oct 15 23:42:29 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dmeventthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dmeventthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DMEventThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   46,   57,   57, 0x05,
      58,   46,   57,   57, 0x05,
      90,   57,   57,   57, 0x05,
     102,   57,   57,   57, 0x05,
     112,  131,   57,   57, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DMEventThread[] = {
    "DMEventThread\0deviceRemoved(CMI_DEVICE_INFO*)\0"
    "deviceInfo\0\0deviceArrived(CMI_DEVICE_INFO*)\0"
    "suspended()\0resumed()\0switchChanged(int)\0"
    "eventID\0"
};

void DMEventThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DMEventThread *_t = static_cast<DMEventThread *>(_o);
        switch (_id) {
        case 0: _t->deviceRemoved((*reinterpret_cast< CMI_DEVICE_INFO*(*)>(_a[1]))); break;
        case 1: _t->deviceArrived((*reinterpret_cast< CMI_DEVICE_INFO*(*)>(_a[1]))); break;
        case 2: _t->suspended(); break;
        case 3: _t->resumed(); break;
        case 4: _t->switchChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DMEventThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DMEventThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_DMEventThread,
      qt_meta_data_DMEventThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DMEventThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DMEventThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DMEventThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DMEventThread))
        return static_cast<void*>(const_cast< DMEventThread*>(this));
    return QThread::qt_metacast(_clname);
}

int DMEventThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DMEventThread::deviceRemoved(CMI_DEVICE_INFO * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DMEventThread::deviceArrived(CMI_DEVICE_INFO * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DMEventThread::suspended()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void DMEventThread::resumed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void DMEventThread::switchChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
