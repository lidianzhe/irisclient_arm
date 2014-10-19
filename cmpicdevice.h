#ifndef CMPICDEVICE_H
#define CMPICDEVICE_H

#include "cmpiclib.h"
#include <QObject>

class CImageInfo;

class CMPICDevice : public QObject, public CMPICLib
{
	// Q_OBJECT macro inserted to use "signal" features of Qt
	Q_OBJECT

public:
	CMPICDevice();

	// The following pure virtual function need to be implemented.
public:
	void eventHandle(EventCode eventCode, CImageInfo *imageInfo);

	// User interface and graphical display should be handled in MainWindow.cpp in Qt
signals:
	void eventReceived(int eventCode, CImageInfo *imageInfo);
};

#endif // CMPICDEVICE_H
