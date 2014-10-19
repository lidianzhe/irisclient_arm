/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Oct 15 23:42:22 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   18,   18,   18, 0x08,
      19,   18,   18,   18, 0x08,
      27,   18,   18,   18, 0x08,
      36,   18,   18,   18, 0x08,
      44,   18,   18,   18, 0x08,
      51,   77,   18,   18, 0x08,
      87,   77,   18,   18, 0x08,
     112,   77,  138,   18, 0x08,
     143,   77,  138,   18, 0x08,
     180,   18,   18,   18, 0x08,
     187,   77,   18,   18, 0x08,
     214,   77,   18,   18, 0x08,
     249,   77,   18,   18, 0x08,
     283,   18,   18,   18, 0x08,
     302,  319,   18,   18, 0x08,
     334,   77,   18,   18, 0x08,
     370,  404,   18,   18, 0x08,
     415,  404,   18,   18, 0x08,
     449,   18,   18,   18, 0x08,
     463,   18,   18,   18, 0x08,
     475,  496,   18,   18, 0x08,
     504,  531,   18,   18, 0x08,
     537,   18,   18,   18, 0x08,
     556,  587,   18,   18, 0x08,
     603,  587,   18,   18, 0x08,
     631,  587,   18,   18, 0x08,
     669,   77,   18,   18, 0x08,
     704,  778,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0open()\0\0recog()\0enroll()\0"
    "close()\0stop()\0doEnroll(CMI_IMAGE_INFO*)\0"
    "imageInfo\0doRecog(CMI_IMAGE_INFO*)\0"
    "autoSave(CMI_IMAGE_INFO*)\0bool\0"
    "unselectedImageSave(CMI_IMAGE_INFO*)\0"
    "save()\0doPreview(CMI_IMAGE_INFO*)\0"
    "doUnselectedImage(CMI_IMAGE_INFO*)\0"
    "doSelectedImages(CMI_IMAGE_INFO*)\0"
    "doEyeCaptureDone()\0doIndicator(int)\0"
    "indicatorValue\0doMatchedFaceImage(CMI_IMAGE_INFO*)\0"
    "doDeviceRemoved(CMI_DEVICE_INFO*)\0"
    "deviceInfo\0doDeviceArrived(CMI_DEVICE_INFO*)\0"
    "doSuspended()\0doResumed()\0"
    "doSwitchChanged(int)\0eventID\0"
    "doDataReceived(EMA_EVENT*)\0event\0"
    "setupDistanceBar()\0setDistanceEllipse(bool,float)\0"
    "isLeft,distance\0setDistanceRect(bool,float)\0"
    "setDistanceUnselectedRect(bool,float)\0"
    "displayLiveImages(CMI_IMAGE_INFO*)\0"
    "displaySelectedImages(CMI_IMAGE_INFO*,unsigned char*,unsigned char*,bo"
    "ol)\0"
    "imageInfo,lfinalImage,rfinalImage,showBoundary\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->open(); break;
        case 1: _t->recog(); break;
        case 2: _t->enroll(); break;
        case 3: _t->close(); break;
        case 4: _t->stop(); break;
        case 5: _t->doEnroll((*reinterpret_cast< CMI_IMAGE_INFO*(*)>(_a[1]))); break;
        case 6: _t->doRecog((*reinterpret_cast< CMI_IMAGE_INFO*(*)>(_a[1]))); break;
        case 7: { bool _r = _t->autoSave((*reinterpret_cast< CMI_IMAGE_INFO*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->unselectedImageSave((*reinterpret_cast< CMI_IMAGE_INFO*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: _t->save(); break;
        case 10: _t->doPreview((*reinterpret_cast< CMI_IMAGE_INFO*(*)>(_a[1]))); break;
        case 11: _t->doUnselectedImage((*reinterpret_cast< CMI_IMAGE_INFO*(*)>(_a[1]))); break;
        case 12: _t->doSelectedImages((*reinterpret_cast< CMI_IMAGE_INFO*(*)>(_a[1]))); break;
        case 13: _t->doEyeCaptureDone(); break;
        case 14: _t->doIndicator((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->doMatchedFaceImage((*reinterpret_cast< CMI_IMAGE_INFO*(*)>(_a[1]))); break;
        case 16: _t->doDeviceRemoved((*reinterpret_cast< CMI_DEVICE_INFO*(*)>(_a[1]))); break;
        case 17: _t->doDeviceArrived((*reinterpret_cast< CMI_DEVICE_INFO*(*)>(_a[1]))); break;
        case 18: _t->doSuspended(); break;
        case 19: _t->doResumed(); break;
        case 20: _t->doSwitchChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->doDataReceived((*reinterpret_cast< EMA_EVENT*(*)>(_a[1]))); break;
        case 22: _t->setupDistanceBar(); break;
        case 23: _t->setDistanceEllipse((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 24: _t->setDistanceRect((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 25: _t->setDistanceUnselectedRect((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 26: _t->displayLiveImages((*reinterpret_cast< CMI_IMAGE_INFO*(*)>(_a[1]))); break;
        case 27: _t->displaySelectedImages((*reinterpret_cast< CMI_IMAGE_INFO*(*)>(_a[1])),(*reinterpret_cast< unsigned char*(*)>(_a[2])),(*reinterpret_cast< unsigned char*(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
