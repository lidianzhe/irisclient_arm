#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTime>
#include <QFile>
#include <QGraphicsScene>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QMutex>
//#include <QAudioOutput>
#include "cmirislib2.h"
#if defined(_EMALIB)
#include "emalib.h"
#endif
#include "cdatabase.h"
#include "dialogname.h"

/*
class QGraphicsScene;
class QImage;
class QGraphicsPixmapItem;
class QGraphicsEllipseItem;
class QGraphicsRectItem;
*/
class EventThread;
class DMEventThread;
class EmaEventThread;
class QextSerialPort;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	enum {
		DEMO_EYE_DISPLAY_WIDTH = 640,
		DEMO_EYE_DISPLAY_HEIGHT = 480,
		EYE_IMAGE_WIDTH = 640,
		EYE_IMAGE_HEIGHT = 480,
		EYE_IMAGE_SIZE = 307200
	};

    enum { // added in V1.0.4
        FACE_WIDTH = 360,
        FACE_HEIGHT = 480
    };

	enum {
		DISTANCE_BAR_PIXEL_PER_MM = 4,
		MAX_UNSELECTED_RECT = 16
	};

	enum {
		RIGHT_EYE = 0, // false
		LEFT_EYE = 1 // true
	};

private:
    //void startAudioPlaying(const QString & wavFile);
        void enrollLetfIrisPath(CMI_IMAGE_INFO *imageInfo, DBRecord *record, QString baseFilename);
        void enrollRightIrisPath(CMI_IMAGE_INFO *imageInfo, DBRecord *record, QString baseFilename);
        void enrollFaceImagePath(CMI_IMAGE_INFO *imageInfo, DBRecord *record, QString baseFilename);

private slots:
    //void finishedAudioPlaying(QAudio::State state);
	void open();
	void recog();
	void enroll();
	void close();
	void stop();

	void doEnroll(CMI_IMAGE_INFO *imageInfo);
	void doRecog(CMI_IMAGE_INFO *imageInfo);

	bool autoSave(CMI_IMAGE_INFO *imageInfo);
	bool unselectedImageSave(CMI_IMAGE_INFO *imageInfo);
	void save();

	// Event
	void doPreview(CMI_IMAGE_INFO *imageInfo);
	void doUnselectedImage(CMI_IMAGE_INFO *imageInfo);
	void doSelectedImages(CMI_IMAGE_INFO *imageInfo);
	void doEyeCaptureDone();
    void doIndicator(int indicatorValue);
    void doMatchedFaceImage(CMI_IMAGE_INFO *imageInfo);

	// DM Event
	void doDeviceRemoved(CMI_DEVICE_INFO *deviceInfo);
	void doDeviceArrived(CMI_DEVICE_INFO *deviceInfo);
	void doSuspended();
	void doResumed();
    void doSwitchChanged(int eventID);

    // EMA Event
#if defined(_EMALIB)
    void doDataReceived(EMA_EVENT *event);
#endif

	void setupDistanceBar();
	void setDistanceEllipse(bool isLeft, float distance);
	void setDistanceRect(bool isLeft, float distance);
	void setDistanceUnselectedRect(bool isLeft, float distance);
	void displayLiveImages(CMI_IMAGE_INFO *imageInfo);
	void displaySelectedImages(CMI_IMAGE_INFO *imageInfo, unsigned char *lfinalImage,
                               unsigned char *rfinalImage, bool showBoundary);

private:
    Ui::MainWindow *ui;

    //QFile m_audioFile;
    //QAudioOutput *m_audioOutput;

	CMI_HANDLE m_cmiHandle;
#if defined(_EMALIB)
    EMA_HANDLE m_emaHandle;
#endif
	CMI_IMAGE_INFO *m_imageInfo;
	bool m_isStart;
	bool m_isEnroll;

	unsigned char *m_applBuffer;
	int m_applBufferSize;

	int m_minWorkingDistance, m_maxWorkingDistance; // in mm
	int m_minOperatingDistance, m_maxOperatingDistance; // in mm
    int m_operatingRangeCenter, m_depthOfField;

    QGraphicsScene *m_rightScene, *m_leftScene, *m_faceScene;
    QImage *m_rightQimage, *m_leftQimage, *m_faceQimage;
    QGraphicsPixmapItem *m_rightPixmapItem, *m_leftPixmapItem, *m_facePixmapItem;
	QGraphicsEllipseItem *m_rightEllipseItem, *m_leftEllipseItem;

	QGraphicsScene *m_rightDistanceScene, *m_leftDistanceScene;
	QImage *m_rightDistanceQimage, *m_leftDistanceQimage;
	QGraphicsPixmapItem *m_rightDistancePixmapItem, *m_leftDistancePixmapItem;
	QGraphicsEllipseItem *m_rightDistanceEllipseItem, *m_leftDistanceEllipseItem;
	int m_distanceBarWidth, m_distanceBarHeight;
	QGraphicsRectItem *m_rightDistanceRectItem, *m_leftDistanceRectItem;
	QGraphicsRectItem *m_rightUnselected[MAX_UNSELECTED_RECT];
	QGraphicsRectItem *m_leftUnselected[MAX_UNSELECTED_RECT];

	QGraphicsEllipseItem *m_rightXYEllipseItem, *m_leftXYEllipseItem;

	int m_barRef;
	int m_unselectedLeftRectID;
	int m_unselectedRightRectID;

	QList<CMI_IMAGE_INFO *> m_unselectedImageList;

	float m_maxXYMovementEnrollment;
	float m_maxZMovementEnrollment;
	float m_maxXYMovementRecognition;
	float m_maxZMovementRecognition;

	int m_displayRate;
	int m_displayCount;

	EventThread *m_eventThread;
	DMEventThread *m_dmEventThread;
    EmaEventThread *m_emaEventThread;

	QList<CMI_DEVICE_INFO *> m_deviceInfoList;
    QList<CMI_DEVICE_INFO *> m_removedList;
    QList<bool> m_isRemovedStartedList;
    QString m_curOpenSerialNumber;
    QMutex m_dmMutex;

    int m_inDepthOfField;
    int m_outDepthOfField;

    CDataBase m_database;

	float m_hdThreshold;

    QextSerialPort *m_serialPort;
    QTime m_time;

#if defined(_EMALIB)
    EMA_EVENT m_curEmaConfig;
#endif
    bool m_isVocalCueEnabled;

    QString m_curPath;
};

#endif // MAINWINDOW_H
