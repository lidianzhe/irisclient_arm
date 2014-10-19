#include "mainwindow.h"
#include "eventthread.h"
#include "dmeventthread.h"
#include "emaeventthread.h"
#include "cdatabase.h"
#include "ui_mainwindow.h"
#include "cmicolorlib.h"

#include "dialogname.h"
#include "ui_dialogname.h"

#include <QtDebug>
#include <QMessageBox>
#include <QSound>
#include <QDir>

#if defined(__linux__)
#define Sleep(x) usleep(x*1000)
#endif

using namespace std;

#define BACKGROUND_COLOR 0x3077ffff
#define TRIPLE(a) qRgb((a), (a), (a))

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Acezne Iris Client");
	// Initialize max Movement values
	m_maxXYMovementEnrollment = 0.4;
	m_maxZMovementEnrollment = 4.0;
    m_maxXYMovementRecognition = 1.0;
    m_maxZMovementRecognition = 12.0;

	connect(ui->pushButton_Open, SIGNAL(clicked()), this, SLOT(open()));
	connect(ui->pushButton_Recog, SIGNAL(clicked()), this, SLOT(recog()));
	connect(ui->pushButton_Enroll, SIGNAL(clicked()), this, SLOT(enroll()));
	connect(ui->pushButton_Stop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui->pushButton_Close, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui->pushButton_Save, SIGNAL(clicked()), this, SLOT(save()));

	m_imageInfo = NULL;
	m_curOpenSerialNumber.clear();
	m_applBuffer = NULL;

	m_unselectedLeftRectID = 0;
	m_unselectedRightRectID = 0;

	m_rightScene = new QGraphicsScene();
	m_leftScene = new QGraphicsScene();
	ui->graphicsView_RightEye->setScene(m_rightScene);
	ui->graphicsView_LeftEye->setScene(m_leftScene);

    m_faceScene = new QGraphicsScene();
    ui->graphicsView_Face->setScene(m_faceScene);

	m_rightPixmapItem = new QGraphicsPixmapItem();
	m_rightScene->addItem(m_rightPixmapItem);
	m_leftPixmapItem = new QGraphicsPixmapItem();
	m_leftScene->addItem(m_leftPixmapItem);

    m_facePixmapItem = new QGraphicsPixmapItem();
    m_faceScene->addItem(m_facePixmapItem);

	// Iris Boundary
	m_rightEllipseItem = new QGraphicsEllipseItem(m_rightPixmapItem);
	m_rightEllipseItem->setPen(QPen(QColor(QString("#00ff00"))));
	m_rightEllipseItem->setZValue(1.0);
	m_leftEllipseItem = new QGraphicsEllipseItem(m_leftPixmapItem);
	m_leftEllipseItem->setPen(QPen(QColor(QString("#00ff00"))));
	m_leftEllipseItem->setZValue(1.0);

	m_rightQimage = new QImage(DEMO_EYE_DISPLAY_WIDTH, DEMO_EYE_DISPLAY_HEIGHT, QImage::Format_ARGB32);
	m_leftQimage = new QImage(DEMO_EYE_DISPLAY_WIDTH, DEMO_EYE_DISPLAY_HEIGHT, QImage::Format_ARGB32);
    m_faceQimage = new QImage(FACE_WIDTH, FACE_HEIGHT, QImage::Format_ARGB32);

	// Distance Indicator
	m_rightDistanceScene = new QGraphicsScene();
	m_leftDistanceScene = new QGraphicsScene();
	ui->graphicsView_RightEyeDistance->setScene(m_rightDistanceScene);
	ui->graphicsView_LeftEyeDistance->setScene(m_leftDistanceScene);

	m_rightDistancePixmapItem = new QGraphicsPixmapItem();
	m_rightDistanceScene->addItem(m_rightDistancePixmapItem);
	m_leftDistancePixmapItem = new QGraphicsPixmapItem();
	m_leftDistanceScene->addItem(m_leftDistancePixmapItem);

    m_distanceBarWidth = ui->graphicsView_RightEyeDistance->width()-4;
    m_distanceBarHeight = ui->graphicsView_RightEyeDistance->height()-4;

	m_rightDistanceEllipseItem = new QGraphicsEllipseItem(m_rightDistancePixmapItem);
	m_rightDistanceEllipseItem->setBrush(QBrush(QColor(QString("#ffffa0"))));
	m_rightDistanceEllipseItem->setPen(QPen(QColor(QString("#ffffa0"))));
	m_rightDistanceEllipseItem->setZValue(1.0);

	m_leftDistanceEllipseItem = new QGraphicsEllipseItem(m_leftDistancePixmapItem);
	m_leftDistanceEllipseItem->setBrush(QBrush(QColor(QString("#ffffa0"))));
	m_leftDistanceEllipseItem->setPen(QPen(QColor(QString("#ffffa0"))));
	m_leftDistanceEllipseItem->setZValue(1.0);

	m_rightXYEllipseItem = new QGraphicsEllipseItem(m_rightPixmapItem);
	m_rightXYEllipseItem->setBrush(QBrush(QColor(QString("#00ff00"))));
	m_rightXYEllipseItem->setPen(QPen(QColor(QString("#00ff00"))));
	m_rightXYEllipseItem->setZValue(1.0);

	m_leftXYEllipseItem = new QGraphicsEllipseItem(m_leftPixmapItem);
	m_leftXYEllipseItem->setBrush(QBrush(QColor(QString("#00ff00"))));
	m_leftXYEllipseItem->setPen(QPen(QColor(QString("#00ff00"))));
	m_leftXYEllipseItem->setZValue(1.0);

	m_rightDistanceRectItem = new QGraphicsRectItem(m_rightDistancePixmapItem);
	m_rightDistanceRectItem->setBrush(QBrush(QColor(QString("#ff0000"))));
	m_rightDistanceRectItem->setPen(QPen(QColor(QString("#ffffff"))));
	m_rightDistanceRectItem->setZValue(3.0);

	m_leftDistanceRectItem = new QGraphicsRectItem(m_leftDistancePixmapItem);
	m_leftDistanceRectItem->setBrush(QBrush(QColor(QString("#ff0000"))));
	m_leftDistanceRectItem->setPen(QPen(QColor(QString("#ffffff"))));
	m_leftDistanceRectItem->setZValue(3.0);

	m_rightDistanceQimage = new QImage(m_distanceBarWidth, m_distanceBarHeight, QImage::Format_ARGB32);
	m_leftDistanceQimage = new QImage(m_distanceBarWidth, m_distanceBarHeight, QImage::Format_ARGB32);

	for (int i = 0; i < MAX_UNSELECTED_RECT; i++) {
		m_leftUnselected[i] = new QGraphicsRectItem(m_leftDistancePixmapItem);
		m_leftUnselected[i]->setBrush(QBrush(QColor(QString("#a0a0a0"))));
		m_leftUnselected[i]->setPen(QPen(QColor(QString("#ffffff"))));
		m_leftUnselected[i]->setZValue(2.0);

		m_rightUnselected[i] = new QGraphicsRectItem(m_rightDistancePixmapItem);
		m_rightUnselected[i]->setBrush(QBrush(QColor(QString("#a0a0a0"))));
		m_rightUnselected[i]->setPen(QPen(QColor(QString("#ffffff"))));
		m_rightUnselected[i]->setZValue(2.0);
	}

	m_displayRate = 1;
	m_displayCount = 0;
	m_isEnroll = false;

	m_hdThreshold = 0.15;

	m_dmEventThread = NULL;
	m_eventThread = NULL;
    m_emaEventThread = NULL;

	if (cmi_createCMIris(CMI_DMX_EMX_MODEL, &m_cmiHandle) == CMI_SUCCESS) {
		m_dmEventThread = new DMEventThread(NULL, m_cmiHandle);
		if (m_dmEventThread) {
			connect(m_dmEventThread, SIGNAL(deviceRemoved(CMI_DEVICE_INFO *)),
					this, SLOT(doDeviceRemoved(CMI_DEVICE_INFO *)));
			connect(m_dmEventThread, SIGNAL(deviceArrived(CMI_DEVICE_INFO *)),
					this, SLOT(doDeviceArrived(CMI_DEVICE_INFO *)));
			connect(m_dmEventThread, SIGNAL(suspended()), this, SLOT(doSuspended()));
			connect(m_dmEventThread, SIGNAL(resumed()), this, SLOT(doResumed()));

            connect(m_dmEventThread, SIGNAL(switchChanged(int)),
                    this, SLOT(doSwitchChanged(int)));
            m_dmEventThread->start();
		}

		m_eventThread = new EventThread(NULL, m_cmiHandle);
		if (m_eventThread) {
			connect(m_eventThread, SIGNAL(preview(CMI_IMAGE_INFO *)),
					this, SLOT(doPreview(CMI_IMAGE_INFO *)));
			connect(m_eventThread, SIGNAL(unselectedImage(CMI_IMAGE_INFO *)),
					this, SLOT(doUnselectedImage(CMI_IMAGE_INFO *)));
			connect(m_eventThread, SIGNAL(selectedImages(CMI_IMAGE_INFO *)),
					this, SLOT(doSelectedImages(CMI_IMAGE_INFO *)));
			connect(m_eventThread, SIGNAL(eyeCaptureDone()), this, SLOT(doEyeCaptureDone()));
            connect(m_eventThread, SIGNAL(indicator(int)), this, SLOT(doIndicator(int)));
            connect(m_eventThread, SIGNAL(matchedFaceImages(CMI_IMAGE_INFO*)),
                    this, SLOT(doMatchedFaceImage(CMI_IMAGE_INFO *)));
		}
	}
	else {
		ui->statusBar->showMessage(QString("Error! Cannot create CMIRISLib!"));
		// diable all buttons
		ui->pushButton_Open->setEnabled(false);
		ui->pushButton_Recog->setEnabled(false);
		ui->pushButton_Enroll->setEnabled(false);
		ui->pushButton_Stop->setEnabled(false);
		ui->pushButton_Close->setEnabled(false);
		ui->pushButton_Save->setEnabled(false);
		ui->checkBox_Inch->setEnabled(false);
	}

    m_inDepthOfField = ui->spinBox_EnrolInDepthOfField->value();
    m_outDepthOfField = ui->spinBox_EnrolOutDepthOfField->value();

#if defined(_EMALIB)
    if (ema_createEMA(&m_emaHandle) == EMA_SUCCESS) {
        m_emaEventThread = new EmaEventThread(NULL, m_emaHandle);
        if (m_emaEventThread) {
            connect(m_emaEventThread, SIGNAL(dataReceived(EMA_EVENT *)),
                    this, SLOT(doDataReceived(EMA_EVENT *)));
            m_emaEventThread->start();
        }
    }
#endif

	m_isStart = false;
	m_deviceInfoList.clear();


#if defined(_EXYNOS_ARM)
    m_curPath = "/usr/local/share/AizheTech";
#else
    m_curPath = QDir::currentPath();
#endif
}

MainWindow::~MainWindow()
{
	if (m_rightScene) delete m_rightScene;
	if (m_leftScene) delete m_leftScene;
    if (m_faceScene) delete m_faceScene;

	if (m_rightQimage) delete m_rightQimage;
	if (m_leftQimage) delete m_leftQimage;
    if (m_faceQimage) delete m_faceQimage;

	if (m_rightDistanceScene) delete m_rightDistanceScene;
	if (m_leftDistanceScene) delete m_leftDistanceScene;

	if (m_rightDistanceQimage) delete m_rightDistanceQimage;
	if (m_leftDistanceQimage) delete m_leftDistanceQimage;

    if (m_dmEventThread && m_dmEventThread->isRunning()) {
        m_dmEventThread->abort();
        m_dmEventThread->wait();
    }
#if defined(_EMALIB)
    if (m_emaEventThread && m_emaEventThread->isRunning()) {
        m_emaEventThread->abort();
        m_emaEventThread->wait();
    }
#endif

	if (m_eventThread) delete m_eventThread;
	if (m_dmEventThread) delete m_dmEventThread;
#if defined(_EMALIB)
    if (m_emaEventThread) delete m_emaEventThread;
#endif

	cmi_closeDevice(m_cmiHandle);
	cmi_destroyCMIris(m_cmiHandle);
#if defined(_EMALIB)
    ema_close(m_emaHandle);
    ema_destroyEMA(m_emaHandle);
#endif

	if (m_applBuffer) {
		delete [] m_applBuffer;
		m_applBuffer = NULL;
	}

	if (m_imageInfo) delete m_imageInfo;

	for (int i = 0; i < m_unselectedImageList.size(); i++) {
		delete m_unselectedImageList.at(i);
	}
	m_unselectedImageList.clear();

	for (int i = 0; i < m_deviceInfoList.size(); i++) {
		delete m_deviceInfoList.at(i);
	}
	m_deviceInfoList.clear();

    delete ui;
}

void MainWindow::enrollLetfIrisPath(CMI_IMAGE_INFO *imageInfo, DBRecord *record, QString baseFilename){

    int width, height;
    unsigned char *image;
    float distance;
    int i, j, k;
    int irisCenterX, irisCenterY, irisRadius;

    // Left Iris Image
    image = imageInfo->leftImage;
    if (image) {
        width = imageInfo->leftWidth;
        height = imageInfo->leftHeight;

        distance = imageInfo->leftDistance/1000.;
        irisCenterX = imageInfo->leftIrisCenterX;
        irisCenterY = imageInfo->leftIrisCenterY;
        irisRadius = imageInfo->leftIrisRadius;

        QImage *qImage = new QImage(width, height, QImage::Format_Indexed8);
        QVector<QRgb> colorTable;
        for (i=0; i < 256; i++) {
            colorTable.append(qRgb(i, i, i));
        }
        qImage->setColorTable(colorTable);

        for (j = 0; j < height; j++) {
            k = j*width;
            for (i = 0; i < width; i++) {
                qImage->setPixel(i, j, image[k+i]);
            }
        }

        QString leyeFilename = "";
        leyeFilename = QString("%1/EnrolledImages/%2_LEYE_C%3_%4_R%5.bmp").arg(m_curPath)
                .arg(baseFilename).arg(irisCenterX).arg(irisCenterY).arg(irisRadius);
        record->setLeftIrisPath(leyeFilename);

        bool leyeOK;
        leyeOK = qImage->save(leyeFilename, 0, 100); // highest image quality
        if (!leyeOK) {
            ui->statusBar->showMessage(QString("Error! fail to save %1.").arg(leyeFilename));
            return;
        }

        delete qImage;
    }
}

void MainWindow::enrollRightIrisPath(CMI_IMAGE_INFO *imageInfo, DBRecord *record, QString baseFilename){

    int width, height;
    unsigned char *image;
    float distance;
    int i, j, k;
    int irisCenterX, irisCenterY, irisRadius;

    // Right Iris Image
    image = imageInfo->rightImage;
    if (image) {
        width = imageInfo->rightWidth;
        height = imageInfo->rightHeight;
        distance = imageInfo->rightDistance/1000.;
        irisCenterX = imageInfo->rightIrisCenterX;
        irisCenterY = imageInfo->rightIrisCenterY;
        irisRadius = imageInfo->rightIrisRadius;

        QImage *qImage = new QImage(width, height, QImage::Format_Indexed8);
        QVector<QRgb> colorTable;
        for (i=0; i < 256; i++) {
            colorTable.append(qRgb(i, i, i));
        }
        qImage->setColorTable(colorTable);

        for (j = 0; j < height; j++) {
            k = j*width;
            for (i = 0; i < width; i++) {
                qImage->setPixel(i, j, image[k+i]);
            }
        }

        QString reyeFilename = "";
        reyeFilename = QString("%1/EnrolledImages/%2_REYE_C%3_%4_R%5.bmp").arg(m_curPath)
                .arg(baseFilename).arg(irisCenterX).arg(irisCenterY).arg(irisRadius);
        record->setRightIrisPath(reyeFilename);

        bool reyeOK;
        reyeOK = qImage->save(reyeFilename, 0, 100); // highest image quality
        if (!reyeOK) {
            ui->statusBar->showMessage(QString("Error! fail to save %1.").arg(reyeFilename));
            return;
        }

        delete qImage;
    }
}

void MainWindow::enrollFaceImagePath(CMI_IMAGE_INFO *imageInfo, DBRecord *record, QString baseFilename){

    int width, height;
    unsigned char *image;

    width = imageInfo->faceWidth;
    height = imageInfo->faceHeight;
    image = imageInfo->faceImage;

    int width2 = width/2;
    int height2 = height/2;

    int i, j, k;

    if (image) {
        m_faceQimage->fill(BACKGROUND_COLOR);

        if (width == CMI_FACE_FULL_WIDTH && height == CMI_FACE_FULL_HEIGHT) {
            qDebug() << "=faceResolution" << imageInfo->faceResolution;
            if (imageInfo->faceResolution == CMI_FACE_BAYER_FULL_RESOLUTION) { // color full resolution image
                uchar *colorImage = new uchar [3*width2*height2];

                CMIC_HANDLE cmicHandle;

                cmic_createCMIColor(&cmicHandle);

                // colorImage size is the half of the original Bayer image size
                cmic_colorConversion(cmicHandle, image, width, height, colorImage);
                cmic_destroyCMIColor(cmicHandle);

                int ind;
                for (j = 0; j < height2; j++) {
                    k = 3*j*width2;
                    for (i = 0; i < width2; i++) {
                        ind = k + 3*i;
                        m_faceQimage->setPixel(i, j,
                                               qRgb(colorImage[ind], colorImage[ind+1], colorImage[ind+2]));
                    }
                }
                delete [] colorImage;
            }
            else { // Mono
                for (j = 0; j < height; j+=2) {
                    k = j*width;
                    for (i = 0; i < width; i+=2) {
                        m_faceQimage->setPixel(i/2, j/2, TRIPLE(image[k+i]));
                    }
                }
            }
        }
        else if (width == 360 && height == 480) {
            for (j = 0; j < height; j++) {
                k = j*width;
                for (i = 0; i < width; i++) {
                    m_faceQimage->setPixel(i, j, TRIPLE(image[k+i]));
                }
            }
        }

        QString faceFilename = "";
        faceFilename = QString("%1/EnrolledImages/%2_FACE.bmp").arg(m_curPath).arg(baseFilename);
        record->setFaceImagePath(faceFilename);

        bool faceOK;
        faceOK = m_faceQimage->save(faceFilename, 0, 100); // highest image quality
        if (!faceOK){
            ui->statusBar->showMessage(QString("Error! fail to save %1.").arg(faceFilename));
            return;
        }
    }
}

void MainWindow::open() {
	ui->statusBar->clearMessage();
	if (m_imageInfo) {
		delete m_imageInfo;
		m_imageInfo = NULL;
    }

    int dbret = m_database.open();

    if (dbret) {
        ui->statusBar->showMessage(QString("Error! Cannot open Enroll.db! Error code = %1").arg(dbret));
		return;
    }

	int curIndex =  ui->comboBox_SerialNumbers->currentIndex();
	if (curIndex < 0) {
        ui->statusBar->showMessage("No device to open");
		return;
	}

	QString curStr = ui->comboBox_SerialNumbers->currentText();

	CMI_DEVICE_INFO *dvInfo = NULL;
	for (int i = 0; i < m_deviceInfoList.size(); i++) {
		if (curStr == QString(m_deviceInfoList.at(i)->serialNumber)) {
			dvInfo = m_deviceInfoList.at(i);
		}
	}

    if (ui->checkBox_UsePSD->isChecked()) {
        cmi_setPSDEnabled(m_cmiHandle, CMI_TRUE);
    }
    else {
        cmi_setPSDEnabled(m_cmiHandle, CMI_FALSE);
    }

    // For color face image capture, set to CMI_FACE_FULL_RESOLUTION
    // This feature is only supported in Windows and should be called
    // before cmi_openDevice()

#if defined(WIN32)
    if (cmi_setFaceCaptureMode(m_cmiHandle, CMI_FACE_FULL_RESOLUTION) != CMI_SUCCESS) {
        ui->statusBar->showMessage("Error in cmi_setFaceCaptureMode()!");
        return;
    }
#else
    if (cmi_setFaceCaptureMode(m_cmiHandle, CMI_FACE_HALF_RESOLUTION) != CMI_SUCCESS) {
        ui->statusBar->showMessage("Error in cmi_setFaceCaptureMode()!");
        return;
    }
#endif

    int ret = cmi_openDevice(m_cmiHandle, dvInfo);

	switch(ret) {

    case CMI_ERROR_INVALID_HANDLE:
        ui->statusBar->showMessage("Error! Invalid Handle");
        break;

    case CMI_ERROR_IN_ARGUMENTS:
        ui->statusBar->showMessage("Error! open argument is NULL");
        break;

    case CMI_ERROR_DEVICE_OPENED:
        ui->statusBar->showMessage("Device has already opened!");
        break;

    case CMI_ERROR_CANNOT_FIND_DEVICE:
        ui->statusBar->showMessage("Error! Cannot find the device!");
        break;

    case CMI_ERROR_FAIL_TO_OPEN_IMAGER_DEVICE:
        ui->statusBar->showMessage("Error! Fail to open imager device. Please check the connections");
        break;

    // Do not need to call close()
    case CMI_ERROR_CANNOT_ALLOC_MEMORY:
        ui->statusBar->showMessage("Error! Cannot allocate Memory.");
        break;

    case CMI_ERROR_EEPROM_READ_TIMEOUT:
        ui->statusBar->showMessage("Error! Cannot read data from the device.");
        close(); // must be called
        break;

    case CMI_ERROR_EEPROM_VERSION_INVALID:
        ui->statusBar->showMessage("Error! Device and S/W versions are not matched.");
        close(); // must be called
        break;

    case CMI_ERROR_CANNOT_FIND_HAARCASCADE_FILES:
        ui->statusBar->showMessage("Error! Fail to find haarcasecades file");
        break;

    case CMI_ERROR_FAIL_TO_OPEN_MOTOR_DEVICE:
        ui->statusBar->showMessage("Error! Fail to open motor device. Please check the connections");
        break;

	case CMI_SUCCESS:

		m_curOpenSerialNumber = curStr;
		ui->label_ModelName->setText(QString(dvInfo->modelName));
        ui->label_FirmwareRev->setText(QString(dvInfo->firmwareRev));

        cmi_getOperatingRangeCenter(m_cmiHandle, &m_operatingRangeCenter);
        cmi_getInOutDepthOfField(m_cmiHandle, &m_inDepthOfField, &m_outDepthOfField);
        m_minOperatingDistance = m_operatingRangeCenter - m_inDepthOfField;
        m_maxOperatingDistance = m_operatingRangeCenter + m_outDepthOfField;
        m_minWorkingDistance = m_operatingRangeCenter - 60; // 270mm
        m_maxWorkingDistance = m_operatingRangeCenter + 60; // 390mm

		// Display the operating range and distance bar in the window
		setupDistanceBar();

        // set color of image area as Background color
		m_rightQimage->fill(BACKGROUND_COLOR);
		m_leftQimage->fill(BACKGROUND_COLOR);
		QPixmap pixmap;
		m_rightPixmapItem->setPixmap(pixmap.fromImage(*m_rightQimage));
		m_leftPixmapItem->setPixmap(pixmap.fromImage(*m_leftQimage));
		ui->graphicsView_RightEye->show();
		ui->graphicsView_LeftEye->show();

		m_eventThread->start();
		m_isStart = false;

        int level;
        cmi_mir_getLicenseLevel(m_cmiHandle, &level);
        ui->statusBar->showMessage(QString("Device opened successfully! Level=%1").arg(level));
        if (level == 1) ui->label_MirlinLevel->setText("1");
        else if (level == 0) ui->label_MirlinLevel->setText("0");
        else ui->label_MirlinLevel->setText("");

#if defined(__linux__)
        system(QString("aplay %1/recognized.wav").arg(m_curPath).toStdString().c_str());
#else
        QSound::play("./recognized.wav");
#endif
		break;
	}

    if (ret != CMI_SUCCESS) return;

#if defined(_EMALIB)
    ret = ema_open(m_emaHandle);

    switch(ret) {

    case EMA_ERROR_INVALID_HANDLE:
        ui->statusBar->showMessage("Error! Invalid Handle");
        break;

    case EMA_ERROR_IN_ARGUMENTS:
        ui->statusBar->showMessage("Error! open argument is NULL");
        break;

    case EMA_ERROR_ALREADY_OPENED:
        ui->statusBar->showMessage("Device has already opened!");
        break;

    case EMA_ERROR_OPEN_FAILED:
        ui->statusBar->showMessage("Error! Cannot find the device!");
        break;

    // Do not need to call close()
    case EMA_ERROR_CANNOT_ALLOC_MEMORY:
        ui->statusBar->showMessage("Error! Cannot allocate Memory.");
        break;

    case EMA_SUCCESS:
        break;
    }
#endif
}

void MainWindow::recog() {
	ui->statusBar->clearMessage();
	ui->label_Name->setText("");
	ui->label_LeftHD->setText("");
	ui->label_RightHD->setText("");
    ui->label_LeftEyeDistance->setText(" ");
    ui->label_RightEyeDistance->setText(" ");

	if (m_imageInfo) {
		delete m_imageInfo;
		m_imageInfo = NULL;
	}
	for (int i = 0; i < m_unselectedImageList.size(); i++) {
		delete m_unselectedImageList.at(i);
	}
	m_unselectedImageList.clear();

	// Should set the target intensity before m_mpicDevice->start()
	unsigned short targetIntensity;
	targetIntensity = ui->spinBox_TargetBrightness->value();
	cmi_setTargetIntensity(m_cmiHandle, targetIntensity);

	// Set Max XY and Z Movement
	float maxXYMovement, maxZMovement;
	maxXYMovement = ui->doubleSpinBox_MaxXYMovement->value();
	cmi_setMaxXYMovement(m_cmiHandle, (int)(maxXYMovement*1000. + 0.5));

	maxZMovement = ui->doubleSpinBox_MaxZMovement->value();
	cmi_setMaxZMovement(m_cmiHandle, (int)(maxZMovement*1000. + 0.5));

	m_displayRate = ui->spinBox_DisplayRate->value();
	m_displayCount = 0;


    cmi_getOperatingRangeCenter(m_cmiHandle, &m_operatingRangeCenter);
    m_inDepthOfField = ui->spinBox_RecogInDepthOfField->value();
    m_outDepthOfField = ui->spinBox_RecogOutDepthOfField->value();
    m_minWorkingDistance = m_operatingRangeCenter - 60; // 270mm
    m_maxWorkingDistance = m_operatingRangeCenter + 60; // 390mm
    if (cmi_setInOutDepthOfField(m_cmiHandle, m_inDepthOfField, m_outDepthOfField) == CMI_SUCCESS) {
        m_minOperatingDistance = m_operatingRangeCenter - m_inDepthOfField;
        m_maxOperatingDistance = m_operatingRangeCenter + m_outDepthOfField;
        setupDistanceBar();
    }

    // Set which eye to capture
    int whichEye = CMI_BOTH_EYES;
    if (ui->radioButton_RecogBothEyes->isChecked()) whichEye = CMI_BOTH_EYES;
    else if (ui->radioButton_RecogEitherEye->isChecked()) whichEye = CMI_EITHER_EYE;
    else if (ui->radioButton_RecogLeftEye->isChecked()) whichEye = CMI_LEFT_EYE;
    else if (ui->radioButton_RecogRightEye->isChecked()) whichEye = CMI_RIGHT_EYE;
    if (cmi_setWhichEye(m_cmiHandle, whichEye) != CMI_SUCCESS) {
        ui->statusBar->showMessage("Error in cmi_setWhichEye()!");
        return;
    }

    // Either eye mode max frame diff setting
    int maxFrmaeDiffInEitherEyeMode;
    maxFrmaeDiffInEitherEyeMode = ui->spinBox_MaxFrameDiffInEitherEyeMode->value();
    cmi_setMaxFrameDiffInEitherEyeMode(m_cmiHandle, maxFrmaeDiffInEitherEyeMode);

    // Indicator event and vocal cues
    if (ui->checkBox_EnableIndicatorEvent->isChecked()) {
        cmi_setIndicatorEventEnabled(m_cmiHandle, CMI_TRUE);
    }
    else {
        cmi_setIndicatorEventEnabled(m_cmiHandle, CMI_FALSE);
    }

    if (ui->checkBox_EnableVovalCues->isChecked()) {
        m_isVocalCueEnabled = true;
        int firstTimeInterval = ui->spinBox_VocalCueFirstTimeInterval->value();
        int nextTimeInterval = ui->spinBox_VocalCueNextTimeInterval->value();
        cmi_setVoiceCueTimeInterval(m_cmiHandle, firstTimeInterval, nextTimeInterval);
    }
    else {
        m_isVocalCueEnabled = false;
    }

    // Return to the initial motor position after the images are captured
    cmi_setMotorReturnToInitEnabled(m_cmiHandle, CMI_TRUE);

	// Indicator setting
	int red, green, brightGreen, blue;
	red = ui->spinBox_Red->value();
	green = ui->spinBox_Green->value();
	brightGreen = ui->spinBox_BrightGreen->value();
	blue = ui->spinBox_Blue->value();

	cmi_setIndicatorBrightness(m_cmiHandle, red, green, brightGreen, blue);

	m_hdThreshold = ui->doubleSpinBox_HDThreshold->value();

	m_isEnroll = false;

	///////////////////////////////////////////////////////////////////////////////////////////
	// cmi_getBufferSize() should be called to find out the buffer size for image storage
	///////////////////////////////////////////////////////////////////////////////////////////
	if (!m_applBuffer) { // if m_applBuffer == NULL, cmi_getBufferSize() has never been called after open()
		// preview frame buffer size
        cmi_getBufferSize(m_cmiHandle, 10, 0, &m_applBufferSize);
		m_applBuffer = new unsigned char [m_applBufferSize];
	}

	DWORD ret = cmi_startCapture(m_cmiHandle, m_applBufferSize, m_applBuffer);

	switch(ret) {

	case CMI_ERROR_INVALID_HANDLE:
        ui->statusBar->showMessage("Error! Invalid Handle");
		break;

	case CMI_ERROR_CALL_GET_BUFFER_SIZE_FIRST:
        ui->statusBar->showMessage("Error! call cmi_getBufferSize() first!");
		break;

	case CMI_ERROR_IN_ARGUMENTS:
        ui->statusBar->showMessage("Error! open argument is NULL");
		break;

	case CMI_ERROR_BUFFER_SIZE_TOO_SMALL:
        ui->statusBar->showMessage("Error! buffer size is too small!");
		break;

	case CMI_ERROR_DEVICE_CLOSED:
		if (m_applBuffer) {
			delete [] m_applBuffer;
			m_applBuffer = NULL;
		}
        ui->statusBar->showMessage("Device has not opened yet! Please open first");
		break;

	case CMI_ERROR_DEVICE_STARTED:
		ui->statusBar->showMessage("Device has already started!");
		break;

	case CMI_ERROR_EEPROM_VERSION_INVALID:
        ui->statusBar->showMessage("Error! Device and S/W versions are not matched.");
		close(); // must be called
		break;

	case CMI_ERROR_FAIL_TO_SEND_COMMAND:
        ui->statusBar->showMessage("Error! Fail to send command to device.");
		close(); // must be called
		break;

	case CMI_SUCCESS:
		m_isStart = true;
		ui->statusBar->showMessage("Capture started!");
		break;

	}

	if (ret != CMI_SUCCESS) return;

	// Set up display
	// For captured but not selected image ID - for display in the distance bar
	m_unselectedLeftRectID = 0;
	m_unselectedRightRectID = 0;

	m_leftEllipseItem->hide();
	m_rightEllipseItem->hide();

	m_leftDistanceEllipseItem->hide();
	m_rightDistanceEllipseItem->hide();

	m_rightXYEllipseItem->hide();
	m_leftXYEllipseItem->hide();

	m_leftDistanceRectItem->hide();
	m_rightDistanceRectItem->hide();

	for (int i = 0; i < MAX_UNSELECTED_RECT; i++) {
		m_leftUnselected[i]->hide();
		m_rightUnselected[i]->hide();
	}

	m_rightQimage->fill(BACKGROUND_COLOR);
	m_leftQimage->fill(BACKGROUND_COLOR);

	QPixmap pixmap;
	m_rightPixmapItem->setPixmap(pixmap.fromImage(*m_rightQimage));
	m_leftPixmapItem->setPixmap(pixmap.fromImage(*m_leftQimage));

    ui->label_IPD->setText(" ");
    ui->label_LeftIrisSize->setText(" ");
	ui->label_RightIrisSize->setText(" ");

	ui->graphicsView_RightEye->show();
	ui->graphicsView_LeftEye->show();
}

void MainWindow::enroll() {
	ui->statusBar->clearMessage();
	ui->label_Name->setText("");
	ui->label_LeftHD->setText("");
	ui->label_RightHD->setText("");

	if (m_imageInfo) {
		delete m_imageInfo;
		m_imageInfo = NULL;
	}
	for (int i = 0; i < m_unselectedImageList.size(); i++) {
		delete m_unselectedImageList.at(i);
	}
	m_unselectedImageList.clear();

    // Indicator event and vocal cues
    if (ui->checkBox_EnableIndicatorEvent->isChecked()) {
        cmi_setIndicatorEventEnabled(m_cmiHandle, CMI_TRUE);
    }
    else {
        cmi_setIndicatorEventEnabled(m_cmiHandle, CMI_FALSE);
    }

    if (ui->checkBox_EnableVovalCues->isChecked()) {
        m_isVocalCueEnabled = true;
        int firstTimeInterval = ui->spinBox_VocalCueFirstTimeInterval->value();
        int nextTimeInterval = ui->spinBox_VocalCueNextTimeInterval->value();
        cmi_setVoiceCueTimeInterval(m_cmiHandle, firstTimeInterval, nextTimeInterval);
    }
    else {
        m_isVocalCueEnabled = false;
    }

    // Do not return to the initial motor position after the images are captured
    cmi_setMotorReturnToInitEnabled(m_cmiHandle, CMI_FALSE);

	// Should set the target intensity before m_mpicDevice->start()
	unsigned short targetIntensity;
	targetIntensity = ui->spinBox_TargetBrightness->value();
	cmi_setTargetIntensity(m_cmiHandle, targetIntensity);

	// Set Max XY and Z Movement
	float maxXYMovement, maxZMovement;
	maxXYMovement = ui->doubleSpinBox_MaxXYMovement->value();
	cmi_setMaxXYMovement(m_cmiHandle, (int)(maxXYMovement*1000. + 0.5));

	maxZMovement = ui->doubleSpinBox_MaxZMovement->value();
	cmi_setMaxZMovement(m_cmiHandle, (int)(maxZMovement*1000. + 0.5));

	m_displayRate = ui->spinBox_DisplayRate->value();
	m_displayCount = 0;

    cmi_getOperatingRangeCenter(m_cmiHandle, &m_operatingRangeCenter);
    m_inDepthOfField = ui->spinBox_EnrolInDepthOfField->value();
    m_outDepthOfField = ui->spinBox_EnrolOutDepthOfField->value();
    m_minWorkingDistance = m_operatingRangeCenter - 60; // 270mm
    m_maxWorkingDistance = m_operatingRangeCenter + 60; // 390mm
    if (cmi_setInOutDepthOfField(m_cmiHandle, m_inDepthOfField, m_outDepthOfField) == CMI_SUCCESS) {
        m_minOperatingDistance = m_operatingRangeCenter - m_inDepthOfField;
        m_maxOperatingDistance = m_operatingRangeCenter + m_outDepthOfField;
        setupDistanceBar();
    }

    // Set which eye to capture
    int whichEye = CMI_BOTH_EYES;
    if (ui->radioButton_EnrolBothEyes->isChecked()) whichEye = CMI_BOTH_EYES;
    else if (ui->radioButton_EnrolEitherEye->isChecked()) whichEye = CMI_EITHER_EYE;
    else if (ui->radioButton_EnrolLeftEye->isChecked()) whichEye = CMI_LEFT_EYE;
    else if (ui->radioButton_EnrolRightEye->isChecked()) whichEye = CMI_RIGHT_EYE;
    if (cmi_setWhichEye(m_cmiHandle, whichEye) != CMI_SUCCESS) {
        ui->statusBar->showMessage("Error in cmi_setWhichEye()!");
        return;
    }

    // Either eye mode max frame diff setting
    int maxFrmaeDiffInEitherEyeMode;
    maxFrmaeDiffInEitherEyeMode = ui->spinBox_MaxFrameDiffInEitherEyeMode->value();
    cmi_setMaxFrameDiffInEitherEyeMode(m_cmiHandle, maxFrmaeDiffInEitherEyeMode);

	// Indicator setting
	int red, green, brightGreen, blue;
	red = ui->spinBox_Red->value();
	green = ui->spinBox_Green->value();
	brightGreen = ui->spinBox_BrightGreen->value();
	blue = ui->spinBox_Blue->value();

	cmi_setIndicatorBrightness(m_cmiHandle, red, green, brightGreen, blue);

	m_isEnroll = true;

	///////////////////////////////////////////////////////////////////////////////////////////
	// cmi_getBufferSize() should be called to find out the buffer size for image storage
	///////////////////////////////////////////////////////////////////////////////////////////
	if (!m_applBuffer) { // if m_applBuffer == NULL, cmi_getBufferSize() has never been called after open()
		// preview frame buffer size
        cmi_getBufferSize(m_cmiHandle, 10, 0, &m_applBufferSize);
		m_applBuffer = new unsigned char [m_applBufferSize];
	}

	DWORD ret = cmi_startCapture(m_cmiHandle, m_applBufferSize, m_applBuffer);

	switch(ret) {

	case CMI_ERROR_INVALID_HANDLE:
        ui->statusBar->showMessage("Error! Invalid Handle");
		break;

	case CMI_ERROR_CALL_GET_BUFFER_SIZE_FIRST:
        ui->statusBar->showMessage("Error! call cmi_getBufferSize() first!");
		break;

	case CMI_ERROR_IN_ARGUMENTS:
        ui->statusBar->showMessage("Error! open argument is NULL");
		break;

	case CMI_ERROR_BUFFER_SIZE_TOO_SMALL:
        ui->statusBar->showMessage("Error! buffer size is too small!");
		break;

	case CMI_ERROR_DEVICE_CLOSED:
		if (m_applBuffer) {
			delete [] m_applBuffer;
			m_applBuffer = NULL;
		}
        ui->statusBar->showMessage("Device has not opened yet! Please open first");
		break;

	case CMI_ERROR_DEVICE_STARTED:
		ui->statusBar->showMessage("Device has already started!");
		break;

	case CMI_ERROR_EEPROM_VERSION_INVALID:
        ui->statusBar->showMessage("Error! Device and S/W versions are not matched.");
		close(); // must be called
		break;

	case CMI_ERROR_FAIL_TO_SEND_COMMAND:
        ui->statusBar->showMessage("Error! Fail to send command to device.");
		close(); // must be called
		break;

	case CMI_SUCCESS:
		m_isStart = true;
		ui->statusBar->showMessage("Capture started!");
		break;

	}

	if (ret != CMI_SUCCESS) return;

	// Set up display
	// For captured but not selected image ID - for display in the distance bar
	m_unselectedLeftRectID = 0;
	m_unselectedRightRectID = 0;

	m_leftEllipseItem->hide();
	m_rightEllipseItem->hide();

	m_leftDistanceEllipseItem->hide();
	m_rightDistanceEllipseItem->hide();

	m_rightXYEllipseItem->hide();
	m_leftXYEllipseItem->hide();

	m_leftDistanceRectItem->hide();
	m_rightDistanceRectItem->hide();

	for (int i = 0; i < MAX_UNSELECTED_RECT; i++) {
		m_leftUnselected[i]->hide();
		m_rightUnselected[i]->hide();
	}

	m_rightQimage->fill(BACKGROUND_COLOR);
	m_leftQimage->fill(BACKGROUND_COLOR);

	QPixmap pixmap;
	m_rightPixmapItem->setPixmap(pixmap.fromImage(*m_rightQimage));
	m_leftPixmapItem->setPixmap(pixmap.fromImage(*m_leftQimage));

    ui->label_IPD->setText(" ");
    ui->label_LeftIrisSize->setText(" ");
	ui->label_RightIrisSize->setText(" ");
    ui->label_LeftEyeDistance->setText(" ");
    ui->label_RightEyeDistance->setText(" ");

	ui->graphicsView_RightEye->show();
	ui->graphicsView_LeftEye->show();
}

void MainWindow::stop() {
    ui->statusBar->clearMessage();

	int ret = cmi_stopCapture(m_cmiHandle);

	switch(ret) {

	case CMI_ERROR_INVALID_HANDLE:
        ui->statusBar->showMessage("Error! Invalid Handle");
		break;

	case CMI_ERROR_DEVICE_CLOSED:
        ui->statusBar->showMessage("Device has not opened yet!");
		m_isStart = false;
		break;

	case CMI_ERROR_DEVICE_STOPPED:
		ui->statusBar->showMessage("Capture has not started yet!");
		break;

	case CMI_ERROR_FAIL_TO_SEND_COMMAND:
		ui->statusBar->showMessage("Error! Fail to send command to device.");
		m_isStart = false;
		break;

	case CMI_SUCCESS:
		m_isStart = false;
		ui->statusBar->showMessage("Capture stopped!");
		break;
	}
}

void MainWindow::close() {
	ui->statusBar->clearMessage();
	ui->label_Name->setText("");
	ui->label_LeftHD->setText("");
	ui->label_RightHD->setText("");

	// Stop first when capture is still running
	if (m_isStart) stop();
	cmi_closeDevice(m_cmiHandle);

#if defined(_EMALIB)
    ema_close(m_emaHandle);
#endif

	m_eventThread->abort();
    m_eventThread->wait();

	if (m_applBuffer) {
		delete [] m_applBuffer;
		m_applBuffer = NULL;
	}

	ui->statusBar->showMessage("Device closed!");

	// Reset the display
	m_leftEllipseItem->hide();
	m_rightEllipseItem->hide();

	m_leftDistanceEllipseItem->hide();
	m_rightDistanceEllipseItem->hide();

	m_rightXYEllipseItem->hide();
	m_leftXYEllipseItem->hide();

	m_leftDistanceRectItem->hide();
	m_rightDistanceRectItem->hide();

	m_rightQimage->fill(0xffffffff);
	m_leftQimage->fill(0xffffffff);

	for (int i = 0; i < MAX_UNSELECTED_RECT; i++) {
		m_leftUnselected[i]->hide();
		m_rightUnselected[i]->hide();
	}

	QPixmap pixmap;
	m_rightPixmapItem->setPixmap(pixmap.fromImage(*m_rightQimage));
	m_leftPixmapItem->setPixmap(pixmap.fromImage(*m_leftQimage));

	ui->graphicsView_RightEye->show();
    ui->graphicsView_LeftEye->show();
	ui->label_LeftIrisSize->setText(" ");
	ui->label_RightIrisSize->setText(" ");
	ui->label_IPD->setText(" ");

	m_database.close();
}


void MainWindow::doEnroll(CMI_IMAGE_INFO *imageInfo) {

    if (imageInfo->leftImage == NULL && imageInfo->rightImage == NULL) {
        ui->statusBar->showMessage("Error! No Enroll images!");
		return;
	}

	unsigned char *lfinalImage = imageInfo->leftImage;
	unsigned char *rfinalImage = imageInfo->rightImage;

    unsigned char *leftEnrolTemplate = new unsigned char [CMI_MIR_ENROL_TEMPLATE_SIZE];
    unsigned char *rightEnrolTemplate = new unsigned char [CMI_MIR_ENROL_TEMPLATE_SIZE];

    int ret = cmi_mir_getEnrolTemplates(m_cmiHandle, leftEnrolTemplate, rightEnrolTemplate,
                              CMI_MIR_ENROL_TEMPLATE_SIZE, imageInfo, CMI_MIR_SHOW_SEGMENTATION);

    if (ret != CMI_SUCCESS &&
            ret != CMI_MIR_LEFT_TEMPLATE_GENERATED_ONLY &&
            ret != CMI_MIR_RIGHT_TEMPLATE_GENERATED_ONLY) {
        ui->statusBar->showMessage(QString("Error! cmi_mir_getEnrolTemplates() failed! %1").arg(ret));
        return;
    }
    displaySelectedImages(imageInfo, lfinalImage, rfinalImage, false);

    DialogName dialogname;
    QString name;
    bool ok;

    if (ok = dialogname.exec()) {
        name = dialogname.name();
    }

    if (ok && !name.isEmpty()) {
        DBRecord record;
        QByteArray qba;
        record.setId(m_database.recordListSize());
        record.setName(name);

        if (ret == CMI_MIR_LEFT_TEMPLATE_GENERATED_ONLY) {
            for (int i = 0; i < CMI_MIR_ENROL_TEMPLATE_SIZE; i++) {
                rightEnrolTemplate[i] = 0;
            }
        }
        else if (ret == CMI_MIR_RIGHT_TEMPLATE_GENERATED_ONLY) {
            for (int i = 0; i < CMI_MIR_ENROL_TEMPLATE_SIZE; i++) {
                leftEnrolTemplate[i] = 0;
            }
        }

        qba = QByteArray((const char *)leftEnrolTemplate, CMI_MIR_ENROL_TEMPLATE_SIZE);
        record.setLeftIrisTemplate(qba);

        qba = QByteArray((const char *)rightEnrolTemplate, CMI_MIR_ENROL_TEMPLATE_SIZE);
        record.setRightIrisTemplate(qba);

        delete [] leftEnrolTemplate;
        delete [] rightEnrolTemplate;

        ///////////////////////////////////////////////////////////////////////////////////////
        //<Enroll Image Path & Save Image in the Path>/////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////
        QDir imageDir(QString("%1/EnrolledImages").arg(m_curPath));
        if (!imageDir.exists()) {
            bool ok = imageDir.mkdir(QString("%1/EnrolledImages").arg(m_curPath));
            if (!ok) {
                ui->statusBar->showMessage(QString("Error! Cannot create %1/EnrolledImages directory.").arg(m_curPath));
                return;
            }
        }

        QString baseFilename = QDateTime::currentDateTime().
                toString("yyyy.MM.dd_hh.mm.ss");

        enrollLetfIrisPath(imageInfo, &record, baseFilename);
        enrollRightIrisPath(imageInfo, &record, baseFilename);
        enrollFaceImagePath(imageInfo, &record, baseFilename);
        ////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////

        if (!m_database.insert(record)) {
            ui->statusBar->showMessage("Error! fail to enroll templates!");
        }
        else { // success
#if defined(__linux__)
            system(QString("aplay %1/enrollCompleted.wav").arg(m_curPath).toStdString().c_str());
#else
            QSound::play("./enrollCompleted.wav");
#endif

            ui->statusBar->showMessage(QString("%1 is successfully enrolled!").arg(name));
        }
    }
	else {
        ui->statusBar->showMessage("Fail to Enroll.\nPlease retry!");
	}
}

void MainWindow::doRecog(CMI_IMAGE_INFO *imageInfo) {

	QMessageBox msgBox;

    qDebug() << "doRecog Start" << m_time.restart() << "ms";

    if (imageInfo->leftImage == NULL && imageInfo->rightImage == NULL) {
        ui->statusBar->showMessage("Error! Match Images are NULL!");
		return;
	}

    unsigned char *lfinalImage = imageInfo->leftImage;
    unsigned char *rfinalImage = imageInfo->rightImage;

    displaySelectedImages(imageInfo, lfinalImage, rfinalImage, true);
    qDebug() << "Displayed" << m_time.restart() << "ms";

    unsigned char *leftMatchTemplate = new unsigned char [CMI_MIR_MATCH_ROT_13_TEMPLATE_SIZE];
    unsigned char *rightMatchTemplate = new unsigned char [CMI_MIR_MATCH_ROT_13_TEMPLATE_SIZE];

    int ret = cmi_mir_getMatchTemplates(m_cmiHandle, leftMatchTemplate, rightMatchTemplate,
                              CMI_MIR_MATCH_ROT_13_TEMPLATE_SIZE, imageInfo, CMI_MIR_SHOW_SEGMENTATION);
    qDebug() << "Match Template generation" << m_time.restart() << "ms";


    float minLeftHD = 0.5, minRightHD = 0.5, minSumHD = 1.0;
    int minId = -1;

    DBRecord *record;
    float lhammingDistance, rhammingDistance;
    int lret, rret, i;

    if (ret == CMI_MIR_LEFT_TEMPLATE_GENERATED_ONLY) {
        for (i = 0; i < m_database.recordListSize(); i++) {
            record = m_database.m_recordList.at(i);
            lret = cmi_mir_compareTemplate(m_cmiHandle, (unsigned char *)record->leftIrisTemplate().data(),
                                          leftMatchTemplate, CMI_MIR_MATCH_ROT_13_TEMPLATE_SIZE,
                                          &lhammingDistance);

            if (lret == CMI_SUCCESS) {
                if (lhammingDistance < minLeftHD) {
                    minLeftHD = lhammingDistance;
                    if (lhammingDistance < m_hdThreshold) {
                        minId = i;
                    }
                }
            }
        }

    }
    else if (ret == CMI_MIR_RIGHT_TEMPLATE_GENERATED_ONLY) {
        for (i = 0; i < m_database.recordListSize(); i++) {
            record = m_database.m_recordList.at(i);
            rret = cmi_mir_compareTemplate(m_cmiHandle, (unsigned char *)record->rightIrisTemplate().data(),
                                          rightMatchTemplate, CMI_MIR_MATCH_ROT_13_TEMPLATE_SIZE,
                                          &rhammingDistance);

            if (rret == CMI_SUCCESS) {
                if (rhammingDistance < minRightHD) {
                    minRightHD = rhammingDistance;
                    if (rhammingDistance < m_hdThreshold) {
                        minId = i;
                    }
                }
            }
        }

    }
    else if (ret == CMI_SUCCESS) {
        for (i = 0; i < m_database.recordListSize(); i++) {
            record = m_database.m_recordList.at(i);
            lret = cmi_mir_compareTemplate(m_cmiHandle, (unsigned char *)record->leftIrisTemplate().data(),
                                          leftMatchTemplate, CMI_MIR_MATCH_ROT_13_TEMPLATE_SIZE,
                                          &lhammingDistance);
            rret = cmi_mir_compareTemplate(m_cmiHandle, (unsigned char *)record->rightIrisTemplate().data(),
                                          rightMatchTemplate, CMI_MIR_MATCH_ROT_13_TEMPLATE_SIZE,
                                          &rhammingDistance);

            if (lret == CMI_SUCCESS && lhammingDistance < minLeftHD) minLeftHD = lhammingDistance;
            if (rret == CMI_SUCCESS && rhammingDistance < minRightHD) minRightHD = rhammingDistance;

            if (lret == CMI_SUCCESS || rret == CMI_SUCCESS ) {
                qDebug() << "Match Result" << i << lhammingDistance << rhammingDistance;

                if ((lret == CMI_SUCCESS && lhammingDistance < m_hdThreshold) ||
                        (rret == CMI_SUCCESS && rhammingDistance < m_hdThreshold)) {
                    // match found
                    if (minId == -1) {
                        minId = i;
                        if (lret == CMI_SUCCESS) minLeftHD = lhammingDistance;
                        else minLeftHD = 0.5;
                        if (rret == CMI_SUCCESS) minRightHD = rhammingDistance;
                        else minRightHD = 0.5;
                        minSumHD = minLeftHD + minRightHD;
                    }
                    else {
                        if (lhammingDistance + rhammingDistance < minSumHD) {
                            minId = i;
                            if (lret == CMI_SUCCESS) minLeftHD = lhammingDistance;
                            else minLeftHD = 0.5;
                            if (rret == CMI_SUCCESS) minRightHD = rhammingDistance;
                            else minRightHD = 0.5;
                            minSumHD = minLeftHD + minRightHD;
                        }
                    }
                }
            }
        }
    }
    else {
        ui->statusBar->showMessage("Fail to generate Iris Template.Please retry!");
#if defined(__linux__)
        system(QString("aplay %1/notRecognized.wav").arg(m_curPath).toStdString().c_str());
#else
        QSound::play("./notRecognized.wav");
#endif
        if(ui->checkBox_ContinuousRecog->isChecked()) {
            int intervalSec = ui->spinBox_Interval->value();
            ui->pushButton_Recog->animateClick(intervalSec*1000);
        }
        delete [] leftMatchTemplate;
        delete [] rightMatchTemplate;
        return;
    }

    if (minId == -1) {
        ui->label_LeftHD->setText(QString("%1").arg(minLeftHD, 0, 'g', 2));
        ui->label_RightHD->setText(QString("%1").arg(minRightHD, 0, 'g', 2));

        ui->label_Name->setText("No match found");
        if (ui->checkBox_PopUpMatchWindow->isChecked()) {
            msgBox.setFont(QFont("Calibri", 36));
            msgBox.setText("No match was found!");
            msgBox.exec();
        }

#if defined(__linux__)
        system(QString("aplay %1/notRecognized.wav").arg(m_curPath).toStdString().c_str());
#else
        QSound::play("./notRecognized.wav");
#endif
    }
    else {
#if defined(_EMALIB)
        EMA_EVENT emaEvent;
        emaEvent.cbSize = sizeof(EMA_EVENT);

        emaEvent.eventType = EMA_EVENT_TYPE_RELAY_WRITE;

        emaEvent.relayInterval = 30; // unit: 0.1sec

        emaEvent.relayValue = EMA_EVENT_VALUE_RELAY_ABNORMAL;

        int ret = ema_writeEvent(m_emaHandle, &emaEvent);
#endif

        record = m_database.m_recordList.at(minId);
        ui->label_Name->setText(record->name());
        ui->label_LeftHD->setText(QString("%1").arg(minLeftHD, 0, 'g', 2));
        ui->label_RightHD->setText(QString("%1").arg(minRightHD, 0, 'g', 2));

        if (ui->checkBox_PopUpMatchWindow->isChecked()) {
            msgBox.setFont(QFont("Calibri", 48));
            msgBox.setText(QString("%1 is identified!").arg(record->name()));
            msgBox.exec();
        }

#if defined(__linux__)
        system("aplay /usr/local/share/AizheTech/recognized.wav");
#else
        QSound::play("./recognized.wav");
#endif


        if(!record->faceImagePath().isNull())
        {
            //////////////////////////////////////////////////////////////////////////////////////
            //<Matched Face Image Output>/////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////
            int width, height;

            QImage *dbFaceImage = new QImage(FACE_WIDTH, FACE_HEIGHT, QImage::Format_ARGB32);
            dbFaceImage->load(record->faceImagePath(), NULL);

            //Face Image
            width = dbFaceImage->width();
            height = dbFaceImage->height();

            int i, j;

            for (j = 0; j < height; j++) {
                for (i = 0; i < width; i++) {
                    if (i%2 == 0 && j%2 == 0) {
                        m_rightQimage->setPixel(i/2+460, j/2+240, dbFaceImage->pixel(i, j));
                    }
                }
            }

            QPixmap pixmap;
            m_rightPixmapItem->setPixmap(pixmap.fromImage(*m_rightQimage));
            ui->graphicsView_RightEye->show();
            //////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////
        }

    }

	if(ui->checkBox_ContinuousRecog->isChecked()) {
		int intervalSec = ui->spinBox_Interval->value();
		ui->pushButton_Recog->animateClick(intervalSec*1000);
	}
    qDebug() << "doRecog End" << m_time.restart() << "ms";
    delete [] leftMatchTemplate;
    delete [] rightMatchTemplate;
}


bool MainWindow::autoSave(CMI_IMAGE_INFO *imageInfo) {

	QString baseFilename = QDateTime::currentDateTime().
						   toString("yyyy.MM.dd_hh.mm.ss");
	QString leyeFilename = "";
    QString reyeFilename = "";
    QString faceFilename = "";
    bool leyeOK, reyeOK, faceOK;
	QString msg;

	int width, height;
	unsigned char *image;
	float distance;
	int i, j, k;
	int irisCenterX, irisCenterY, irisRadius;

    QDir imageDir(QString("%1/Images").arg(m_curPath));
    if (!imageDir.exists()) {
        bool ok = imageDir.mkdir(QString("%1/Images").arg(m_curPath));
        if (!ok) {
            ui->statusBar->showMessage(QString("Error! Cannot create %1/Images directory.").arg(m_curPath));
            return false;
        }
    }

	// Left Iris Image
	image = imageInfo->leftImage;
	if (image) {
		width = imageInfo->leftWidth;
		height = imageInfo->leftHeight;

		distance = imageInfo->leftDistance/1000.;
		irisCenterX = imageInfo->leftIrisCenterX;
		irisCenterY = imageInfo->leftIrisCenterY;
		irisRadius = imageInfo->leftIrisRadius;

		QImage *qImage = new QImage(width, height, QImage::Format_Indexed8);
		QVector<QRgb> colorTable;
		for (i=0; i < 256; i++) {
			colorTable.append(qRgb(i, i, i));
		}
		qImage->setColorTable(colorTable);

		for (j = 0; j < height; j++) {
			k = j*width;
			for (i = 0; i < width; i++) {
				qImage->setPixel(i, j, image[k+i]);
			}
		}

        leyeFilename = QString("%1/%2/%3_LEYE_C%4_%5_R%6.bmp").arg(m_curPath).
                arg("Images").arg(baseFilename)
				.arg(irisCenterX).arg(irisCenterY)
				.arg(irisRadius);
		leyeOK = qImage->save(leyeFilename, 0, 100); // highest image quality
        if (leyeOK) msg = QString("%1 is saved.").arg(leyeFilename);
		else msg = QString("Fail to save %1.").arg(leyeFilename);

		delete qImage;
	}

	// Right Iris Image
	image = imageInfo->rightImage;
	if (image) {
		width = imageInfo->rightWidth;
		height = imageInfo->rightHeight;
		distance = imageInfo->rightDistance/1000.;
		irisCenterX = imageInfo->rightIrisCenterX;
		irisCenterY = imageInfo->rightIrisCenterY;
		irisRadius = imageInfo->rightIrisRadius;

		QImage *qImage = new QImage(width, height, QImage::Format_Indexed8);
		QVector<QRgb> colorTable;
		for (i=0; i < 256; i++) {
			colorTable.append(qRgb(i, i, i));
		}
		qImage->setColorTable(colorTable);

		for (j = 0; j < height; j++) {
			k = j*width;
			for (i = 0; i < width; i++) {
				qImage->setPixel(i, j, image[k+i]);
			}
		}

        reyeFilename = QString("%1/%2/%3_REYE_C%4_%5_R%6.bmp").arg(m_curPath).
                arg("Images").arg(baseFilename)
				.arg(irisCenterX).arg(irisCenterY)
				.arg(irisRadius);
		reyeOK = qImage->save(reyeFilename, 0, 100); // highest image quality
        if (reyeOK) msg.append(QString("  %1 is saved.").arg(reyeFilename));
		else msg.append(QString("  Fail to save %1.").arg(reyeFilename));

		delete qImage;
	}

    // Face Image
    image = imageInfo->faceImage;
    if (image) {
        width = imageInfo->faceWidth;
        height = imageInfo->faceHeight;

        QImage *qImage = new QImage(width, height, QImage::Format_Indexed8);
        QVector<QRgb> colorTable;
        for (i=0; i < 256; i++) {
            colorTable.append(qRgb(i, i, i));
        }
        qImage->setColorTable(colorTable);

        for (j = 0; j < height; j++) {
            k = j*width;
            for (i = 0; i < width; i++) {
                qImage->setPixel(i, j, image[k+i]);
            }
        }

        faceFilename = QString("%1/Images/%2_FACE.bmp").arg(m_curPath).arg(baseFilename);
        faceOK = qImage->save(faceFilename, 0, 100); // highest image quality
        if (faceOK) msg.append(QString("  %1 is saved.").arg(faceFilename));
        else msg.append(QString("  Fail to save %1.").arg(faceFilename));

        delete qImage;

        if (imageInfo->faceResolution == 17) { // color
            faceFilename = QString("%1/Images/%2_FACE_Color.bmp").arg(m_curPath).arg(baseFilename);
            faceOK = m_faceQimage->save(faceFilename, 0, 100); // highest image quality
            if (faceOK) msg.append(QString("  %1 is saved.").arg(faceFilename));
            else msg.append(QString("  Fail to save %1.").arg(faceFilename));
        }
    }

	ui->statusBar->showMessage(msg);

	return (leyeOK || reyeOK);
}

bool MainWindow::unselectedImageSave(CMI_IMAGE_INFO *imageInfo) {

	QString baseFilename = QDateTime::currentDateTime().
						   toString("yyyy.MM.dd_hh.mm.ss");
	QString leyeFilename = "";
	QString reyeFilename = "";
	bool ok;

	int width, height;
	unsigned char *image;
	float distance;
	int i, j, k;

    QDir imageDir(QString("%1/Images").arg(m_curPath));
    if (!imageDir.exists()) {
        bool ok = imageDir.mkdir(QString("%1/Images").arg(m_curPath));
        if (!ok) {
            ui->statusBar->showMessage(QString("Error! Cannot create %1/Images directory.").arg(m_curPath));
            return false;
        }
    }

	// Left Iris Image
	if (imageInfo->leftImage) {
		image = imageInfo->leftImage;
		width = imageInfo->leftWidth;
		height = imageInfo->leftHeight;
		image = imageInfo->leftImage;
		distance = imageInfo->leftDistance/1000.;

		QImage *qImage = new QImage(width, height, QImage::Format_Indexed8);
		QVector<QRgb> colorTable;
		for (i=0; i < 256; i++) {
			colorTable.append(qRgb(i, i, i));
		}
		qImage->setColorTable(colorTable);

		for (j = 0; j < height; j++) {
			k = j*width;
			for (i = 0; i < width; i++) {
				qImage->setPixel(i, j, image[k+i]);
			}
        }

        leyeFilename = QString("%1/%2/%3_U_LEYE_D%4.bmp").arg(m_curPath).arg("Images").
				arg(baseFilename).arg((int)(distance+0.5));
		ok = qImage->save(leyeFilename, 0, 100); // highest image quality
		delete qImage;

	}
	else if (imageInfo->rightImage){ // Right Iris Image
		image = imageInfo->rightImage;
		width = imageInfo->rightWidth;
		height = imageInfo->rightHeight;
		image = imageInfo->rightImage;
		distance = imageInfo->rightDistance/1000.;

		QImage *qImage = new QImage(width, height, QImage::Format_Indexed8);
		QVector<QRgb> colorTable;
		for (i=0; i < 256; i++) {
			colorTable.append(qRgb(i, i, i));
		}
		qImage->setColorTable(colorTable);

		for (j = 0; j < height; j++) {
			k = j*width;
			for (i = 0; i < width; i++) {
				qImage->setPixel(i, j, image[k+i]);
			}
		}

        reyeFilename = QString("%1/%2/%3_U_REYE_D%4.bmp").arg(m_curPath).
                   arg("Images").arg(baseFilename).arg((int)(distance+0.5));
		ok = qImage->save(reyeFilename, 0, 100); // highest image quality
		delete qImage;
	}

	return ok;
}

void MainWindow::save() {

	ui->statusBar->clearMessage();

    if (!m_imageInfo) {
        ui->statusBar->showMessage("Please capture first before save!");
		return;
	}

	for (int i = 0; i < m_unselectedImageList.size(); i++) {
		bool ok = unselectedImageSave(m_unselectedImageList.at(i));
        if (!ok) {
            ui->statusBar->showMessage("Error! Cannot save unselected images.");
		}
	}

    if (!autoSave(m_imageInfo)) {
        ui->statusBar->showMessage("Error! Fail to save the selected images.");
	}

	QString msg = ui->statusBar->currentMessage();
	msg.append(QString("  %1 unselected images are saved.").arg(m_unselectedImageList.size()));
	ui->statusBar->showMessage(msg);
}

void MainWindow::doPreview(CMI_IMAGE_INFO *imageInfo) {
	//qDebug() << "doPreview";
	if (m_isStart) displayLiveImages(imageInfo);
	delete imageInfo;
}

void MainWindow::doUnselectedImage(CMI_IMAGE_INFO *imageInfo) {
	m_unselectedImageList << imageInfo;
	qDebug() << "Number of unselected images" << m_unselectedImageList.size();
	if (imageInfo->leftDistance > 0) {
		setDistanceUnselectedRect(LEFT_EYE, imageInfo->leftDistance/1000.);
	}
	else if (imageInfo->rightDistance > 0) {
		setDistanceUnselectedRect(RIGHT_EYE, imageInfo->rightDistance/1000.);
	}
}

void MainWindow::doSelectedImages(CMI_IMAGE_INFO *imageInfo) {
	m_isStart = false;

	if (m_isEnroll) doEnroll(imageInfo);
	else doRecog(imageInfo);

	if (m_imageInfo) delete m_imageInfo;
	m_imageInfo = imageInfo; // save for save()
}

void MainWindow::doEyeCaptureDone() { // play capture sound
    m_time.start();
    qDebug() << "Captured";

#if defined(__linux__)
    system(QString("aplay %1/captureSound.wav").arg(m_curPath).toStdString().c_str());
#else
    QSound::play("./captureSound.wav");
    //Sleep(500);
#endif
}

void MainWindow::doIndicator(int indicatorValue) {
    if (indicatorValue == CMI_INDICATOR_COLOR_OFF)
        ui->label_IndColor->setText("Off");
    else if (indicatorValue == CMI_INDICATOR_COLOR_RED)
        ui->label_IndColor->setText("Red");
    else if (indicatorValue == CMI_INDICATOR_COLOR_GREEN)
        ui->label_IndColor->setText("Green");
    else if (indicatorValue == CMI_INDICATOR_COLOR_BLUE)
        ui->label_IndColor->setText("Blue");
    else if (indicatorValue == CMI_VOICE_COME_CLOSER) {
        if (m_isVocalCueEnabled) {
#if defined(__linux__)
            system(QString("aplay %1/PleaseComeCloser.wav").arg(m_curPath).toStdString().c_str());
#else
            QSound::play("./PleaseComeCloser.wav");
            Sleep(500);
#endif
        }
    }
    else if (indicatorValue == CMI_VOICE_MOVE_BACK) {
        if (m_isVocalCueEnabled) {
#if defined(__linux__)
            system(QString("aplay %1/PleaseMoveBack.wav").arg(m_curPath).toStdString().c_str());
#else
            QSound::play("./PleaseMoveBack.wav");
            Sleep(500);
#endif
        }
    }
}

void MainWindow::doMatchedFaceImage(CMI_IMAGE_INFO *imageInfo) {
    m_isStart = false;

    int width, height, i, j, k;
    int displayWidth, displayHeight;
    unsigned char *faceImage;
    QPixmap pixmap;
    QMessageBox msgBox;
    DBRecord *record;
    QImage *dbFaceImage;

#if defined(__linux__)
    system(QString("aplay %1/recognized.wav").arg(m_curPath).toStdString().c_str());
#else
        QSound::play("./recognized.wav");
#endif

    //////////////////////////////////////////////////////////////////////////////
    //<Current Face Image>////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    width = imageInfo->faceWidth;
    height = imageInfo->faceHeight;
    faceImage = imageInfo->faceImage;

    if (faceImage) { // width : 180, height : 320
        m_rightQimage->fill(BACKGROUND_COLOR);

        displayWidth = DEMO_EYE_DISPLAY_WIDTH >= width ? width : DEMO_EYE_DISPLAY_WIDTH;
        displayHeight = DEMO_EYE_DISPLAY_HEIGHT >= height ? height : DEMO_EYE_DISPLAY_HEIGHT;

        // Image Display
        for (j = 0; j < displayHeight; j++) {
            k = j*width;
            for (i = 0; i < displayWidth; i++) {
                m_rightQimage->setPixel(i, j, TRIPLE(faceImage[k+i]));
            }
        }

        m_rightPixmapItem->setPixmap(pixmap.fromImage(*m_rightQimage));
        ui->graphicsView_RightEye->show();
    }

    //////////////////////////////////////////////////////////////////////////////
    //<Enrolled Face Image>///////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////
    record = m_database.m_recordList.at(imageInfo->ipd);

    dbFaceImage = new QImage(FACE_WIDTH, FACE_HEIGHT, QImage::Format_ARGB32);
    dbFaceImage->load(record->faceImagePath(), NULL);

    //Face Image
    width = dbFaceImage->width();
    height = dbFaceImage->height();

    displayWidth = DEMO_EYE_DISPLAY_WIDTH >= width ? width : DEMO_EYE_DISPLAY_WIDTH;
    displayHeight = DEMO_EYE_DISPLAY_HEIGHT >= height ? height : DEMO_EYE_DISPLAY_HEIGHT;

    // Image Display
    for (j = 0; j < displayHeight; j++) {
        k = j*width;
        for (i = 0; i < displayWidth; i++) {
            m_leftQimage->setPixel(i, j, dbFaceImage->pixel(i, j));
        }
    }

    m_leftPixmapItem->setPixmap(pixmap.fromImage(*m_leftQimage));
    ui->graphicsView_LeftEye->show();
    //////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////

    if (m_isEnroll) ui->label_Name->setText(QString("Already Enrolled : %1\n%2% Face Matched!").arg(record->name()).arg(imageInfo->rollAngleUncertainty));
    else {
        ui->label_Name->setText(QString("Success : %1\n%2% Face Matched!").arg(record->name()).arg(imageInfo->rollAngleUncertainty));

#if defined(_EMALIB)
        EMA_EVENT emaEvent;
        emaEvent.cbSize = sizeof(EMA_EVENT);

        emaEvent.eventType = EMA_EVENT_TYPE_RELAY_WRITE;

        emaEvent.relayInterval = 30; // unit: 0.1sec

        emaEvent.relayValue = EMA_EVENT_VALUE_RELAY_ABNORMAL;

        int ret = ema_writeEvent(m_emaHandle, &emaEvent);
#endif
    }

    if (ui->checkBox_PopUpMatchWindow->isChecked()) {
        msgBox.setFont(QFont("Calibri", 48));
        msgBox.setText(QString("%1 is identified!").arg(record->name()));
        msgBox.exec();
    }

    if (m_imageInfo) delete m_imageInfo;
    m_imageInfo = imageInfo; // save for save()

    if(ui->checkBox_ContinuousRecog->isChecked()) {
        int intervalSec = ui->spinBox_Interval->value();
        ui->pushButton_Recog->animateClick(intervalSec*1000);
    }
}

void MainWindow::doDeviceRemoved(CMI_DEVICE_INFO *deviceInfo) {
    QMutexLocker locker(&m_dmMutex);

    QString removedSN(deviceInfo->serialNumber);
    int i;

    int curIndex = ui->comboBox_SerialNumbers->currentIndex();
    QString comboSN;
    bool isOPenDeviceRemoved = false;

    if (curIndex >= 0) {
        comboSN = ui->comboBox_SerialNumbers->currentText();

        if (comboSN == removedSN) {
            if (removedSN == m_curOpenSerialNumber) {
                m_isRemovedStartedList << m_isStart;
                close();
                m_curOpenSerialNumber.clear();
                comboSN.clear();
                ui->statusBar->showMessage(QString("USB Removed. SN:%1 removed").arg(removedSN));
                isOPenDeviceRemoved = true;
            }
            else {
                ui->statusBar->showMessage(QString("SN:%1 removed").arg(removedSN));
            }
        }
        else {
            ui->statusBar->showMessage(QString("SN:%1 removed").arg(removedSN));
        }

        for (i = 0; i < m_deviceInfoList.size(); i++) {
            if (removedSN == QString(m_deviceInfoList.at(i)->serialNumber)) {
                if (isOPenDeviceRemoved) {
                    m_removedList << m_deviceInfoList.at(i);
                }
                else {
                    delete m_deviceInfoList.at(i);
                }
                m_deviceInfoList.removeAt(i);
                break;
            }
        }

        ui->comboBox_SerialNumbers->clear();
        int index = 0;
        for (i = 0; i < m_deviceInfoList.size(); i++) {
            ui->comboBox_SerialNumbers->addItem(QString(m_deviceInfoList.at(i)->serialNumber));
            if (QString(m_deviceInfoList.at(i)->serialNumber) == comboSN) {
                index = i;
            }
        }
        ui->comboBox_SerialNumbers->setCurrentIndex(index);
    }
    delete deviceInfo;
}

void MainWindow::doDeviceArrived(CMI_DEVICE_INFO *deviceInfo) {
    QMutexLocker locker(&m_dmMutex);

	QString arrivedSN(deviceInfo->serialNumber);
	int i;

	int curIndex = ui->comboBox_SerialNumbers->currentIndex();
	QString comboSN;

	if (curIndex >= 0) {
		comboSN = ui->comboBox_SerialNumbers->currentText();
	}

	m_deviceInfoList << deviceInfo;

	ui->comboBox_SerialNumbers->clear();
	int index = 0;

    bool openArrivedDevice = false;
    bool wasStarted = false;

    if (m_curOpenSerialNumber.size() == 0 && m_removedList.size() > 0) {
        for (i = 0; i < m_removedList.size(); i++) {
            if (QString(m_removedList.at(i)->serialNumber) == arrivedSN) {
                comboSN = arrivedSN;
                delete m_removedList.at(i);
                m_removedList.removeAt(i);
                wasStarted = m_isRemovedStartedList.at(i);
                m_isRemovedStartedList.removeAt(i);
                openArrivedDevice = true;
                break;
            }
        }
    }

	for (i = 0; i < m_deviceInfoList.size(); i++) {
		ui->comboBox_SerialNumbers->addItem(QString(m_deviceInfoList.at(i)->serialNumber));
		if (QString(m_deviceInfoList.at(i)->serialNumber) == comboSN) {
			index = i;
		}
	}
	ui->comboBox_SerialNumbers->setCurrentIndex(index);

	ui->statusBar->showMessage(QString("SN:%1 is arrived").arg(arrivedSN));

    if (ui->checkBox_AutoStart->isChecked()) {
        open();
        Sleep(200);
        recog();
    }
    else {
        if (openArrivedDevice) { // removed and arrived
            open();
            Sleep(200);
            if (wasStarted) recog();
        }
    }
}

void MainWindow::doSuspended() {
    QMutexLocker locker(&m_dmMutex);

	if (m_curOpenSerialNumber != QString("")) {
		close();
		ui->statusBar->showMessage(QString("SN:%1 closed").arg(m_curOpenSerialNumber));
	}
}

void MainWindow::doResumed() {
    QMutexLocker locker(&m_dmMutex);

	ui->statusBar->showMessage("Resumed.");
}

void MainWindow::doSwitchChanged(int eventID) {

    if (eventID == CMI_EVENT_TAMPER_SWITCH_OPEN) {
        ui->label_ActionEvent->setText("Tamper Switch Open");
    }
    else if (eventID == CMI_EVENT_TAMPER_SWITCH_CLOSED) {
        ui->label_ActionEvent->setText("Tamper Switch Closed");
    }
    else if (eventID == CMI_EVENT_MODE_KEY_PRESSED) {
        ui->label_ActionEvent->setText("Mode Key Pressed");
    }
    else if (eventID == CMI_EVENT_MODE_KEY_RELEASED) {
        ui->label_ActionEvent->setText("Mode Key Released");
    }

    else if (eventID == CMI_EVENT_FUNC_KEY_1_PRESSED) {
        ui->label_ActionEvent->setText("Key 1 Pressed");
    }
    else if (eventID == CMI_EVENT_FUNC_KEY_1_RELEASED) {
        ui->label_ActionEvent->setText("Key 1 Released");
    }
    else if (eventID == CMI_EVENT_FUNC_KEY_2_PRESSED) {
        ui->label_ActionEvent->setText("Key 2 Pressed");
    }
    else if (eventID == CMI_EVENT_FUNC_KEY_2_RELEASED) {
        ui->label_ActionEvent->setText("Key 2 Released");
    }
    else if (eventID == CMI_EVENT_FUNC_KEY_3_PRESSED) {
        ui->label_ActionEvent->setText("Key 3 Pressed");
    }
    else if (eventID == CMI_EVENT_FUNC_KEY_3_RELEASED) {
        ui->label_ActionEvent->setText("Key 3 Released");
    }
    else if (eventID == CMI_EVENT_FUNC_KEY_4_PRESSED) {
        ui->label_ActionEvent->setText("Key 4 Pressed");
    }
    else if (eventID == CMI_EVENT_FUNC_KEY_4_RELEASED) {
        ui->label_ActionEvent->setText("Key 4 Released");
    }

    else if (eventID == CMI_EVENT_MOVE_UP_PRESSED) {
        ui->label_ActionEvent->setText("Move up Pressed");
    }
    else if (eventID == CMI_EVENT_MOVE_UP_RELEASED) {
        ui->label_ActionEvent->setText("Move up Released");
    }
    else if (eventID == CMI_EVENT_MOVE_DOWN_PRESSED) {
        ui->label_ActionEvent->setText("Move down Pressed");
    }
    else if (eventID == CMI_EVENT_MOVE_DOWN_RELEASED) {
        ui->label_ActionEvent->setText("Move down Released");
    }

}

#if defined(_EMALIB)
void MainWindow::doDataReceived(EMA_EVENT *event) {

    int numOfBytes;
    QByteArray ba;

    qDebug() << "==== EVENT TYPE" << event->eventType;

    switch (event->eventType) {

    case EMA_EVENT_TYPE_CLOSE:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_CLOSE");
        break;

    case EMA_EVENT_TYPE_OPEN:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_OPEN");
        break;

    case EMA_EVENT_TYPE_WIEGAND_READ_DATA:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_WIEGAND_READ_DATA");
        break;

    case EMA_EVENT_TYPE_WIEGAND_WRITE_DATA:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_WIEGAND_WRITE_DATA");
        break;

    case EMA_EVENT_TYPE_WIEGAND_SET_CONFIG:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_WIEGAND_SET_CONFIG");
        break;

    case EMA_EVENT_TYPE_WIEGAND_GET_CONFIG:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_WIEGAND_GET_CONFIG");
        break;

    case EMA_EVENT_TYPE_WIEGAND_AUTO_DETECT:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_WIEGAND_AUTO_DETECT");
        break;

    case EMA_EVENT_TYPE_GPI12_READ:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_GPI12_READ");
        break;

    case EMA_EVENT_TYPE_GPO1_WRITE:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_GPO1_WRITE");
        break;

    case EMA_EVENT_TYPE_GPO2_WRITE:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_GPO2_WRITE");
        break;

    case EMA_EVENT_TYPE_GPO12_WRITE:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_GPO12_WRITE");
        break;

    case EMA_EVENT_TYPE_RELAY_WRITE:
        ui->label_EMAEvent->setText("EMA_EVENT_TYPE_RELAY_WRITE");
        break;

    }

    return;
}
#endif

void MainWindow::setupDistanceBar() {

    // DISTANCE_BAR_PIXEL_PER_MM = 4: 1mm is 4pixels in the distance bar
    int redHeight = DISTANCE_BAR_PIXEL_PER_MM*(m_minOperatingDistance - m_minWorkingDistance);
    int greenHeight = DISTANCE_BAR_PIXEL_PER_MM*(m_maxOperatingDistance - m_minOperatingDistance);
    int blueHeight = DISTANCE_BAR_PIXEL_PER_MM*(m_maxWorkingDistance - m_maxOperatingDistance);
    m_barRef = DISTANCE_BAR_PIXEL_PER_MM*(m_operatingRangeCenter - m_minWorkingDistance);

    int red = redHeight; // end of red
    int green = red + greenHeight; // end of green
    int blue = green + blueHeight; // end of green

    int i, j;

    for (i = 0; i < m_distanceBarWidth; i++) {
        for (j = 0; j < red; j++) {
            m_rightDistanceQimage->setPixel(i, j, qRgb(255, 0, 0));
            m_leftDistanceQimage->setPixel(i, j, qRgb(255, 0, 0));
        }
        for (j = red; j < green; j++) {
            m_rightDistanceQimage->setPixel(i, j, qRgb(0, 255, 0));
            m_leftDistanceQimage->setPixel(i, j, qRgb(0, 255, 0));
        }
        for (j = green; j < blue; j++) {
            m_rightDistanceQimage->setPixel(i, j, qRgb(0, 100, 255));
            m_leftDistanceQimage->setPixel(i, j, qRgb(0, 100, 255));
        }
    }

    QPixmap pixmap;
    m_rightDistancePixmapItem->setPixmap(pixmap.fromImage(*m_rightDistanceQimage));
    m_leftDistancePixmapItem->setPixmap(pixmap.fromImage(*m_leftDistanceQimage));

    ui->graphicsView_RightEyeDistance->show();
    ui->graphicsView_LeftEyeDistance->show();
}

void MainWindow::setDistanceEllipse(bool isLeft, float distance) {

	int ypos = m_barRef + ((int)(distance + 0.5) - m_operatingRangeCenter)*
			   DISTANCE_BAR_PIXEL_PER_MM;
	// cannot draw outside of distance bar graphics
	ypos = (ypos < m_distanceBarWidth) ? m_distanceBarWidth : ypos;
	ypos = (ypos > (m_distanceBarHeight - m_distanceBarWidth)) ?
		   (m_distanceBarHeight - m_distanceBarWidth) : ypos;

	if (isLeft) {
		m_leftDistanceEllipseItem->setRect(0.0, 0.0, m_distanceBarWidth*3/4, m_distanceBarWidth*1/2);
		m_leftDistanceEllipseItem->setPos(m_distanceBarWidth/8, ypos - m_distanceBarWidth*1/4);
		m_leftDistanceEllipseItem->show();
	}
	else {
		m_rightDistanceEllipseItem->setRect(0.0, 0.0, m_distanceBarWidth*3/4, m_distanceBarWidth*1/2);
		m_rightDistanceEllipseItem->setPos(m_distanceBarWidth/8, ypos -  m_distanceBarWidth*1/4);
		m_rightDistanceEllipseItem->show();
	}
}

void MainWindow::setDistanceRect(bool isLeft, float distance) {

	int ypos = m_barRef + ((int)(distance + 0.5) - m_operatingRangeCenter)*
			   DISTANCE_BAR_PIXEL_PER_MM;
	// cannot draw outside of distance bar graphics
	ypos = (ypos < m_distanceBarWidth) ? m_distanceBarWidth : ypos;
	ypos = (ypos > (m_distanceBarHeight - m_distanceBarWidth)) ?
		   (m_distanceBarHeight - m_distanceBarWidth) : ypos;

	if (isLeft) {
		m_leftDistanceRectItem->setRect(0.0, 0.0, m_distanceBarWidth*4/5, m_distanceBarWidth*1/4);
		m_leftDistanceRectItem->setPos(m_distanceBarWidth/10, ypos - m_distanceBarWidth*1/8);

		m_leftDistanceRectItem->show();
	}
	else {
		m_rightDistanceRectItem->setRect(0.0, 0.0, m_distanceBarWidth*4/5, m_distanceBarWidth*1/4);
		m_rightDistanceRectItem->setPos(m_distanceBarWidth/10, ypos - m_distanceBarWidth*1/8);
		m_rightDistanceRectItem->show();
	}
}

void MainWindow::setDistanceUnselectedRect(bool isLeft, float distance) {

	int id;
	int ypos = m_barRef + ((int)(distance + 0.5) - m_operatingRangeCenter)*
			   DISTANCE_BAR_PIXEL_PER_MM;
	// cannot draw outside of distance bar graphics
	ypos = (ypos < m_distanceBarWidth) ? m_distanceBarWidth : ypos;
	ypos = (ypos > (m_distanceBarHeight - m_distanceBarWidth)) ?
		   (m_distanceBarHeight - m_distanceBarWidth) : ypos;

	//qDebug() << "UNSELECTED_RECT" << isLeft << distance;
	if (isLeft) {
		if (m_unselectedLeftRectID >= MAX_UNSELECTED_RECT) return;
		id = m_unselectedLeftRectID;
		m_leftUnselected[id]->setRect(0.0, 0.0, m_distanceBarWidth*3/4, m_distanceBarWidth*1/5);
		m_leftUnselected[id]->setPos(m_distanceBarWidth/8, ypos - m_distanceBarWidth*1/10);
		m_leftUnselected[id]->show();
		m_unselectedLeftRectID++;
	}
	else {
		if (m_unselectedRightRectID >= MAX_UNSELECTED_RECT) return;
		id = m_unselectedRightRectID;
		m_rightUnselected[id]->setRect(0.0, 0.0, m_distanceBarWidth*3/4, m_distanceBarWidth*1/5);
		m_rightUnselected[id]->setPos(m_distanceBarWidth/8, ypos - m_distanceBarWidth*1/10);
		m_rightUnselected[id]->show();
		m_unselectedRightRectID++;
	}
}

void MainWindow::displayLiveImages(CMI_IMAGE_INFO *imageInfo) {

	int width, height;
	unsigned char *image;
	float distance;
	int i, j, k;
	int x1, y1;
	QPixmap pixmap;

	// Left Iris Image
	width = imageInfo->leftWidth;
	height = imageInfo->leftHeight;
	image = imageInfo->leftImage;
    distance = imageInfo->leftDistance/1000.;

	if (image) {
        qDebug() << "Left Distance" << distance;
		if (ui->checkBox_Display->isChecked()) {
			if (m_displayCount%m_displayRate == 0) {
				m_leftQimage->fill(BACKGROUND_COLOR);

				// live image size is always less than 640x480
				x1 = (DEMO_EYE_DISPLAY_WIDTH - width)/2;
				y1 = (DEMO_EYE_DISPLAY_HEIGHT - height)/2;
				if (x1 < 0 || y1 < 0) return;

				for (j = 0; j < height; j++) {
					k = j*width;
					for (i = 0; i < width; i++) {
						m_leftQimage->setPixel(i+x1, j+y1, TRIPLE(image[k+i]));
					}
				}
				m_leftPixmapItem->setPixmap(pixmap.fromImage(*m_leftQimage));
				ui->graphicsView_LeftEye->show();
			}
		}

		int CPUUsage;
		if (distance > 0) setDistanceEllipse(LEFT_EYE, distance);
		if (imageInfo->avgProcessingTime == 0) CPUUsage = 1;
		else {
			CPUUsage = 100*imageInfo->avgProcessingTime/33;
		}
		//ui->label_CPUUsage->setText(QString("%1%").arg(CPUUsage));

		if (ui->checkBox_DisplayIrisInLive->isChecked()) {
			int irisCenterX, irisCenterY, irisRadius, irisDiameter;
			double xpos, ypos;

			if (imageInfo->leftIrisCenterX > 0) {
				irisCenterX = imageInfo->leftIrisCenterX;
				irisCenterY = imageInfo->leftIrisCenterY;
				irisRadius = imageInfo->leftIrisRadius;
				irisDiameter = irisRadius*2;
				xpos = irisCenterX - irisRadius + x1;
				ypos = irisCenterY - irisRadius + y1;

				if (xpos >= 0 && ypos >= 0 && xpos + irisDiameter < DEMO_EYE_DISPLAY_WIDTH
						&& ypos + irisDiameter < DEMO_EYE_DISPLAY_HEIGHT) {
					m_leftEllipseItem->setRect(0.0, 0.0, irisDiameter, irisDiameter);
					m_leftEllipseItem->setPos(xpos, ypos);

					if (distance > m_maxOperatingDistance) {
						m_leftEllipseItem->setPen(QPen(QColor(QString("#0000ff")), 3));
					}
					else if (distance <= m_minOperatingDistance) {
						m_leftEllipseItem->setPen(QPen(QColor(QString("#ff0000")), 3));
					}
					else {
						m_leftEllipseItem->setPen(QPen(QColor(QString("#00ff00")), 3));
					}
					m_leftEllipseItem->show();
				}
			}
			else {
				m_leftEllipseItem->hide();
			}
		}

		cmi_releasePreviewBuffer(m_cmiHandle, image);
	}

	// Right Iris Image
	width = imageInfo->rightWidth;
	height = imageInfo->rightHeight;
	image = imageInfo->rightImage;
	distance = imageInfo->rightDistance/1000.;

	if (image) {
        qDebug() << "Right Distance" << distance;
		if (ui->checkBox_Display->isChecked()) {
			if (m_displayCount%m_displayRate == 0) {
				m_rightQimage->fill(BACKGROUND_COLOR);

				// live image size is always less than 640x480
				x1 = (DEMO_EYE_DISPLAY_WIDTH - width)/2;
				y1 = (DEMO_EYE_DISPLAY_HEIGHT - height)/2;
				if (x1 < 0 || y1 < 0) return;

				for (j = 0; j < height; j++) {
					k = j*width;
					for (i = 0; i < width; i++) {
						m_rightQimage->setPixel(i+x1, j+y1, TRIPLE(image[k+i]));
					}
				}
				m_rightPixmapItem->setPixmap(pixmap.fromImage(*m_rightQimage));
				ui->graphicsView_RightEye->show();
			}
			m_displayCount++;
		}

		if (distance > 0) setDistanceEllipse(RIGHT_EYE, distance);

		if (ui->checkBox_DisplayIrisInLive->isChecked()) {
			int irisCenterX, irisCenterY, irisRadius, irisDiameter;
			double xpos, ypos;

			if (imageInfo->rightIrisCenterX > 0) {
				irisCenterX = imageInfo->rightIrisCenterX;
				irisCenterY = imageInfo->rightIrisCenterY;
				irisRadius = imageInfo->rightIrisRadius;
				irisDiameter = irisRadius*2;
				xpos = irisCenterX - irisRadius + x1;
				ypos = irisCenterY - irisRadius + y1;

				if (xpos >= 0 && ypos >= 0 && xpos + irisDiameter < DEMO_EYE_DISPLAY_WIDTH
						&& ypos + irisDiameter < DEMO_EYE_DISPLAY_HEIGHT) {
					m_rightEllipseItem->setRect(0.0, 0.0, irisDiameter, irisDiameter);
					m_rightEllipseItem->setPos(xpos, ypos);
					if (distance > m_maxOperatingDistance) {
						m_rightEllipseItem->setPen(QPen(QColor(QString("#0000ff")), 3));
					}
					else if (distance <= m_minOperatingDistance) {
						m_rightEllipseItem->setPen(QPen(QColor(QString("#ff0000")), 3));
					}
					else {
						m_rightEllipseItem->setPen(QPen(QColor(QString("#00ff00")), 3));
					}
					m_rightEllipseItem->show();
				}
			}
			else {
				m_rightEllipseItem->hide();
			}
		}

		cmi_releasePreviewBuffer(m_cmiHandle, image);
	}
}

void MainWindow::displaySelectedImages(CMI_IMAGE_INFO *imageInfo, unsigned char *lfinalImage,
                                       unsigned char *rfinalImage, bool showBoundary) {

	int width, height;
	unsigned char *image;
	float distance;
	int i, j, k;
	int irisCenterX, irisCenterY, irisRadius, irisDiameter;
	float xyMovementMM, zMovementMM;
	int displayWidth, displayHeight;
	QPixmap pixmap;

	// Left Iris Image
	width = imageInfo->leftWidth;
	height = imageInfo->leftHeight;
	image = lfinalImage;
	distance = imageInfo->leftDistance/1000.;
	irisCenterX = imageInfo->leftIrisCenterX;
	irisCenterY = imageInfo->leftIrisCenterY;
	irisRadius = imageInfo->leftIrisRadius;
	xyMovementMM = imageInfo->leftXYMovement/1000.;
	if (xyMovementMM < 0.01) xyMovementMM = 0;
	zMovementMM = imageInfo->leftZMovement/1000.;
	if (zMovementMM < 0.01) zMovementMM = 0;
	irisDiameter = irisRadius*2;

	if (image) {
		m_leftQimage->fill(BACKGROUND_COLOR);

		displayWidth = DEMO_EYE_DISPLAY_WIDTH >= width ? width : DEMO_EYE_DISPLAY_WIDTH;
		displayHeight = DEMO_EYE_DISPLAY_HEIGHT >= height ? height : DEMO_EYE_DISPLAY_HEIGHT;

		// Image Display
		for (j = 0; j < displayHeight; j++) {
			k = j*width;
			for (i = 0; i < displayWidth; i++) {
				m_leftQimage->setPixel(i, j, TRIPLE(image[k+i]));
			}
		}
		// Center crosshair
        if (showBoundary) {
            for (i = irisCenterX - 5; i < irisCenterX + 6; i++) {
                m_leftQimage->setPixel(i, irisCenterY, qRgb(0, 255, 0));
            }
            for (j = irisCenterY - 5; j < irisCenterY + 6; j++) {
                m_leftQimage->setPixel(irisCenterX, j, qRgb(0, 255, 0));
            }
        }
		m_leftPixmapItem->setPixmap(pixmap.fromImage(*m_leftQimage));
		ui->graphicsView_LeftEye->show();

		// Distance display
		if (distance > 0) {
            setDistanceRect(LEFT_EYE, distance);
            ui->label_LeftEyeDistance->setText(QString("%1mm").arg((int)(distance + 0.5)));
        }

		// Iris Diameter Display
		double xpos = irisCenterX - irisRadius;
		double ypos = irisCenterY - irisRadius;

        if (!showBoundary) {
            m_leftEllipseItem->hide();
		}
		else {
			m_leftEllipseItem->setPen(QPen(QColor(QString("#00ff00")), 2));
			m_leftEllipseItem->setRect(0.0, 0.0, irisDiameter, irisDiameter);
			m_leftEllipseItem->setPos(xpos, ypos);
			m_leftEllipseItem->show();
		}


        ui->label_LeftIrisSize->setText(QString("%1").arg(irisDiameter));
	}

	// Right Iris Image
	width = imageInfo->rightWidth;
	height = imageInfo->rightHeight;
	image = rfinalImage;
	distance = imageInfo->rightDistance/1000.;
	irisCenterX = imageInfo->rightIrisCenterX;
	irisCenterY = imageInfo->rightIrisCenterY;
	irisRadius = imageInfo->rightIrisRadius;
	xyMovementMM = imageInfo->rightXYMovement/1000.;
	if (xyMovementMM < 0.01) xyMovementMM = 0;
	zMovementMM = imageInfo->rightZMovement/1000.;
	if (zMovementMM < 0.01) zMovementMM = 0;
	irisDiameter = irisRadius*2;

	if (image) {
		m_rightQimage->fill(BACKGROUND_COLOR);

		displayWidth = DEMO_EYE_DISPLAY_WIDTH >= width ? width : DEMO_EYE_DISPLAY_WIDTH;
		displayHeight = DEMO_EYE_DISPLAY_HEIGHT >= height ? height : DEMO_EYE_DISPLAY_HEIGHT;

		// Image Display
		for (j = 0; j < displayHeight; j++) {
			k = j*width;
			for (i = 0; i < displayWidth; i++) {
				m_rightQimage->setPixel(i, j, TRIPLE(image[k+i]));
			}
		}
		// Center crosshair
        if (showBoundary) {
            for (i = irisCenterX - 5; i < irisCenterX + 6; i++) {
                m_rightQimage->setPixel(i, irisCenterY, qRgb(0, 255, 0));
            }
            for (j = irisCenterY - 5; j < irisCenterY + 6; j++) {
                m_rightQimage->setPixel(irisCenterX, j, qRgb(0, 255, 0));
            }
        }
		m_rightPixmapItem->setPixmap(pixmap.fromImage(*m_rightQimage));
		ui->graphicsView_RightEye->show();

		// Distance display
		if (distance > 0) {
            setDistanceRect(RIGHT_EYE, distance);
            ui->label_RightEyeDistance->setText(QString("%1mm").arg((int)(distance + 0.5)));
        }

		// Iris Diameter Display
		double xpos = irisCenterX - irisRadius;
		double ypos = irisCenterY - irisRadius;


        if (!showBoundary) {
			m_rightEllipseItem->hide();
			//m_rightEllipseItem->setPen(QPen(QColor(QString("#006fff")), 2));
		}
		else {
			m_rightEllipseItem->setPen(QPen(QColor(QString("#00ff00")), 2));
			m_rightEllipseItem->setRect(0.0, 0.0, irisDiameter, irisDiameter);
			m_rightEllipseItem->setPos(xpos, ypos);
			m_rightEllipseItem->show();
		}

        ui->label_RightIrisSize->setText(QString("%1").arg(irisDiameter));
	}

    // Face Image
    width = imageInfo->faceWidth;
    height = imageInfo->faceHeight;
    image = imageInfo->faceImage;

    int width2 = width/2;
    int height2 = height/2;

    if (image) {
        m_faceQimage->fill(BACKGROUND_COLOR);

        if (width == CMI_FACE_FULL_WIDTH && height == CMI_FACE_FULL_HEIGHT) {
            qDebug() << "=faceResolution" << imageInfo->faceResolution;
            if (imageInfo->faceResolution == CMI_FACE_BAYER_FULL_RESOLUTION) { // color full resolution image
                uchar *colorImage = new uchar [3*width2*height2];

                CMIC_HANDLE cmicHandle;

                cmic_createCMIColor(&cmicHandle);

                // colorImage size is the half of the original Bayer image size
                cmic_colorConversion(cmicHandle, image, width, height, colorImage);
                cmic_destroyCMIColor(cmicHandle);

                int ind;
                for (j = 0; j < height2; j++) {
                    k = 3*j*width2;
                    for (i = 0; i < width2; i++) {
                        ind = k + 3*i;
                        m_faceQimage->setPixel(i, j,
                                  qRgb(colorImage[ind], colorImage[ind+1], colorImage[ind+2]));
                        if (i%2 == 0 && j%2 == 0) {
                            m_leftQimage->setPixel(i/2, j/2+240,
                                  qRgb(colorImage[ind], colorImage[ind+1], colorImage[ind+2]));
                        }
                    }
                }
                delete [] colorImage;
            }
            else { // Mono
                for (j = 0; j < height; j+=2) {
                    k = j*width;
                    for (i = 0; i < width; i+=2) {
                        m_faceQimage->setPixel(i/2, j/2, TRIPLE(image[k+i]));
                        if (i%4 == 0 && j%4 == 0) {
                            m_leftQimage->setPixel(i/4, j/4+240, TRIPLE(image[k+i]));
                        }
                    }
                }
            }
        }
        else if (width == 360 && height == 480) {
            // Image Display
            for (j = 0; j < height; j++) {
                k = j*width;
                for (i = 0; i < width; i++) {
                    m_faceQimage->setPixel(i, j, TRIPLE(image[k+i]));
                    if (i%2 == 0 && j%2 == 0) {
                        m_leftQimage->setPixel(i/2, j/2+240, TRIPLE(image[k+i]));
                    }
                }
            }
        }

        m_leftPixmapItem->setPixmap(pixmap.fromImage(*m_leftQimage));
        ui->graphicsView_LeftEye->show();

        m_facePixmapItem->setPixmap(pixmap.fromImage(*m_faceQimage));
        ui->graphicsView_Face->show();
    }

	// IPD Display
	if (imageInfo->ipd > 0) {
		if (!ui->checkBox_Inch->isChecked())
			ui->label_IPD->setText(QString("%1mm").arg((double)imageInfo->ipd/1000., 0, 'g', 3));
		else
			ui->label_IPD->setText(QString("%1\"").arg((double)imageInfo->ipd/25400., 0, 'g', 3));
	}
	else {
		ui->label_IPD->setText("");
	}

	// Capture Done.
	if (ui->checkBox_AutoSave->isChecked()) {
		if (!autoSave(imageInfo)) {
            ui->statusBar->showMessage("Error! Fail to save the selected images.");
		}
	}
	else {
		ui->statusBar->showMessage(QString("Capture stopped"));
	}

}

