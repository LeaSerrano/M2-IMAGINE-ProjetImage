/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *logoLabel;
    QLabel *PSNR_label;
    QFrame *frame_1;
    QFrame *start_frame;
    QWidget *fond_image_1;
    QLabel *start_label;
    QPushButton *upload_button;
    QPushButton *download_button;
    QFrame *frame_2;
    QLabel *label_1_debruit;
    QLabel *label_4_debruit;
    QPushButton *submit_button_denoise;
    QLabel *label_2_debruit;
    QLabel *label_3_debruit;
    QLabel *label_3;
    QDoubleSpinBox *select_1_debruit;
    QDoubleSpinBox *select_2_debruit;
    QDoubleSpinBox *select_4_debruit;
    QDoubleSpinBox *select_3_debruit;
    QLabel *label_1_bruit;
    QLabel *label_2_bruit;
    QDoubleSpinBox *select_1_bruit;
    QDoubleSpinBox *select_2_bruit;
    QFrame *frame_21;
    QFrame *frame_22;
    QFrame *frame_23;
    QPushButton *submit_button_noise;
    QComboBox *select_algo_bruit;
    QLabel *label_2;
    QComboBox *select_algo_debruit;
    QPushButton *algo_button;
    QFrame *frame_27;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1106, 719);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        logoLabel = new QLabel(centralwidget);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        logoLabel->setGeometry(QRect(350, 10, 831, 81));
        PSNR_label = new QLabel(centralwidget);
        PSNR_label->setObjectName(QString::fromUtf8("PSNR_label"));
        PSNR_label->setGeometry(QRect(1100, 600, 301, 21));
        PSNR_label->setAlignment(Qt::AlignCenter);
        frame_1 = new QFrame(centralwidget);
        frame_1->setObjectName(QString::fromUtf8("frame_1"));
        frame_1->setGeometry(QRect(20, 10, 741, 676));
        frame_1->setFrameShape(QFrame::StyledPanel);
        frame_1->setFrameShadow(QFrame::Raised);
        start_frame = new QFrame(frame_1);
        start_frame->setObjectName(QString::fromUtf8("start_frame"));
        start_frame->setGeometry(QRect(110, 80, 512, 512));
        start_frame->setFrameShape(QFrame::StyledPanel);
        start_frame->setFrameShadow(QFrame::Raised);
        fond_image_1 = new QWidget(start_frame);
        fond_image_1->setObjectName(QString::fromUtf8("fond_image_1"));
        fond_image_1->setGeometry(QRect(0, 0, 512, 512));
        start_label = new QLabel(fond_image_1);
        start_label->setObjectName(QString::fromUtf8("start_label"));
        start_label->setGeometry(QRect(0, 0, 512, 512));
        upload_button = new QPushButton(frame_1);
        upload_button->setObjectName(QString::fromUtf8("upload_button"));
        upload_button->setGeometry(QRect(20, 20, 30, 30));
        download_button = new QPushButton(frame_1);
        download_button->setObjectName(QString::fromUtf8("download_button"));
        download_button->setGeometry(QRect(60, 20, 30, 30));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(780, 10, 311, 676));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_1_debruit = new QLabel(frame_2);
        label_1_debruit->setObjectName(QString::fromUtf8("label_1_debruit"));
        label_1_debruit->setGeometry(QRect(20, 470, 131, 25));
        label_4_debruit = new QLabel(frame_2);
        label_4_debruit->setObjectName(QString::fromUtf8("label_4_debruit"));
        label_4_debruit->setGeometry(QRect(20, 560, 131, 25));
        submit_button_denoise = new QPushButton(frame_2);
        submit_button_denoise->setObjectName(QString::fromUtf8("submit_button_denoise"));
        submit_button_denoise->setGeometry(QRect(90, 610, 131, 41));
        label_2_debruit = new QLabel(frame_2);
        label_2_debruit->setObjectName(QString::fromUtf8("label_2_debruit"));
        label_2_debruit->setGeometry(QRect(20, 500, 131, 25));
        label_3_debruit = new QLabel(frame_2);
        label_3_debruit->setObjectName(QString::fromUtf8("label_3_debruit"));
        label_3_debruit->setGeometry(QRect(20, 530, 131, 25));
        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 333, 311, 61));
        label_3->setAlignment(Qt::AlignCenter);
        select_1_debruit = new QDoubleSpinBox(frame_2);
        select_1_debruit->setObjectName(QString::fromUtf8("select_1_debruit"));
        select_1_debruit->setEnabled(true);
        select_1_debruit->setGeometry(QRect(180, 470, 111, 25));
        select_1_debruit->setMaximum(300.000000000000000);
        select_1_debruit->setSingleStep(0.500000000000000);
        select_2_debruit = new QDoubleSpinBox(frame_2);
        select_2_debruit->setObjectName(QString::fromUtf8("select_2_debruit"));
        select_2_debruit->setEnabled(true);
        select_2_debruit->setGeometry(QRect(180, 500, 111, 25));
        select_2_debruit->setSingleStep(0.500000000000000);
        select_4_debruit = new QDoubleSpinBox(frame_2);
        select_4_debruit->setObjectName(QString::fromUtf8("select_4_debruit"));
        select_4_debruit->setEnabled(true);
        select_4_debruit->setGeometry(QRect(180, 560, 111, 25));
        select_4_debruit->setSingleStep(0.500000000000000);
        select_3_debruit = new QDoubleSpinBox(frame_2);
        select_3_debruit->setObjectName(QString::fromUtf8("select_3_debruit"));
        select_3_debruit->setEnabled(true);
        select_3_debruit->setGeometry(QRect(180, 530, 111, 25));
        select_3_debruit->setMaximum(300.000000000000000);
        select_3_debruit->setSingleStep(0.500000000000000);
        label_1_bruit = new QLabel(frame_2);
        label_1_bruit->setObjectName(QString::fromUtf8("label_1_bruit"));
        label_1_bruit->setEnabled(true);
        label_1_bruit->setGeometry(QRect(20, 170, 131, 25));
        label_2_bruit = new QLabel(frame_2);
        label_2_bruit->setObjectName(QString::fromUtf8("label_2_bruit"));
        label_2_bruit->setEnabled(true);
        label_2_bruit->setGeometry(QRect(20, 200, 131, 25));
        select_1_bruit = new QDoubleSpinBox(frame_2);
        select_1_bruit->setObjectName(QString::fromUtf8("select_1_bruit"));
        select_1_bruit->setEnabled(true);
        select_1_bruit->setGeometry(QRect(180, 170, 111, 25));
        select_1_bruit->setMaximum(300.000000000000000);
        select_1_bruit->setSingleStep(0.500000000000000);
        select_2_bruit = new QDoubleSpinBox(frame_2);
        select_2_bruit->setObjectName(QString::fromUtf8("select_2_bruit"));
        select_2_bruit->setEnabled(true);
        select_2_bruit->setGeometry(QRect(180, 200, 111, 25));
        select_2_bruit->setSingleStep(0.500000000000000);
        frame_21 = new QFrame(frame_2);
        frame_21->setObjectName(QString::fromUtf8("frame_21"));
        frame_21->setGeometry(QRect(0, 60, 311, 91));
        frame_21->setFrameShape(QFrame::HLine);
        frame_21->setFrameShadow(QFrame::Plain);
        frame_21->setLineWidth(2);
        frame_22 = new QFrame(frame_2);
        frame_22->setObjectName(QString::fromUtf8("frame_22"));
        frame_22->setGeometry(QRect(0, 300, 311, 61));
        frame_22->setFrameShape(QFrame::HLine);
        frame_22->setFrameShadow(QFrame::Plain);
        frame_22->setLineWidth(2);
        frame_23 = new QFrame(frame_2);
        frame_23->setObjectName(QString::fromUtf8("frame_23"));
        frame_23->setGeometry(QRect(0, 350, 311, 91));
        frame_23->setFrameShape(QFrame::HLine);
        frame_23->setFrameShadow(QFrame::Plain);
        frame_23->setLineWidth(2);
        submit_button_noise = new QPushButton(frame_2);
        submit_button_noise->setObjectName(QString::fromUtf8("submit_button_noise"));
        submit_button_noise->setGeometry(QRect(80, 250, 131, 41));
        select_algo_bruit = new QComboBox(frame_2);
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->setObjectName(QString::fromUtf8("select_algo_bruit"));
        select_algo_bruit->setGeometry(QRect(60, 120, 187, 24));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 44, 311, 61));
        label_2->setAlignment(Qt::AlignCenter);
        select_algo_debruit = new QComboBox(frame_2);
        select_algo_debruit->addItem(QString());
        select_algo_debruit->addItem(QString());
        select_algo_debruit->addItem(QString());
        select_algo_debruit->addItem(QString());
        select_algo_debruit->addItem(QString());
        select_algo_debruit->addItem(QString());
        select_algo_debruit->addItem(QString());
        select_algo_debruit->addItem(QString());
        select_algo_debruit->addItem(QString());
        select_algo_debruit->setObjectName(QString::fromUtf8("select_algo_debruit"));
        select_algo_debruit->setGeometry(QRect(60, 409, 187, 24));
        algo_button = new QPushButton(frame_2);
        algo_button->setObjectName(QString::fromUtf8("algo_button"));
        algo_button->setGeometry(QRect(60, 10, 35, 35));
        frame_27 = new QFrame(frame_2);
        frame_27->setObjectName(QString::fromUtf8("frame_27"));
        frame_27->setGeometry(QRect(0, 20, 311, 61));
        frame_27->setFrameShape(QFrame::HLine);
        frame_27->setFrameShadow(QFrame::Plain);
        frame_27->setLineWidth(2);
        label_1_debruit->raise();
        label_4_debruit->raise();
        submit_button_denoise->raise();
        label_2_debruit->raise();
        label_3_debruit->raise();
        label_3->raise();
        select_1_debruit->raise();
        select_2_debruit->raise();
        select_4_debruit->raise();
        select_3_debruit->raise();
        label_1_bruit->raise();
        label_2_bruit->raise();
        select_1_bruit->raise();
        select_2_bruit->raise();
        frame_21->raise();
        frame_22->raise();
        frame_23->raise();
        submit_button_noise->raise();
        select_algo_bruit->raise();
        label_2->raise();
        select_algo_debruit->raise();
        frame_27->raise();
        algo_button->raise();
        MainWindow->setCentralWidget(centralwidget);
        logoLabel->raise();
        PSNR_label->raise();
        frame_2->raise();
        frame_1->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1106, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        logoLabel->setText(QString());
        PSNR_label->setText(QString());
        start_label->setText(QString());
        upload_button->setText(QString());
        download_button->setText(QString());
        label_1_debruit->setText(QString());
        label_4_debruit->setText(QString());
        submit_button_denoise->setText(QCoreApplication::translate("MainWindow", "D\303\251bruiter", nullptr));
        label_2_debruit->setText(QString());
        label_3_debruit->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "D\303\251bruitage :", nullptr));
        label_1_bruit->setText(QString());
        label_2_bruit->setText(QString());
        submit_button_noise->setText(QCoreApplication::translate("MainWindow", "Bruiter", nullptr));
        select_algo_bruit->setItemText(0, QString());
        select_algo_bruit->setItemText(1, QCoreApplication::translate("MainWindow", "GAUSSIEN", nullptr));
        select_algo_bruit->setItemText(2, QCoreApplication::translate("MainWindow", "IMPULSIF", nullptr));
        select_algo_bruit->setItemText(3, QCoreApplication::translate("MainWindow", "POISSON", nullptr));
        select_algo_bruit->setItemText(4, QCoreApplication::translate("MainWindow", "POIVRE SEL", nullptr));
        select_algo_bruit->setItemText(5, QCoreApplication::translate("MainWindow", "SPECKLE", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "Bruitage", nullptr));
        select_algo_debruit->setItemText(0, QString());
        select_algo_debruit->setItemText(1, QCoreApplication::translate("MainWindow", "MOYENNEUR", nullptr));
        select_algo_debruit->setItemText(2, QCoreApplication::translate("MainWindow", "MEDIAN", nullptr));
        select_algo_debruit->setItemText(3, QCoreApplication::translate("MainWindow", "WIENER", nullptr));
        select_algo_debruit->setItemText(4, QCoreApplication::translate("MainWindow", "GAUSSIEN", nullptr));
        select_algo_debruit->setItemText(5, QCoreApplication::translate("MainWindow", "GRADIENT SEUILLEE", nullptr));
        select_algo_debruit->setItemText(6, QCoreApplication::translate("MainWindow", "MOYENNEUR PONDERE", nullptr));
        select_algo_debruit->setItemText(7, QCoreApplication::translate("MainWindow", "NON LOCAL MEANS", nullptr));
        select_algo_debruit->setItemText(8, QCoreApplication::translate("MainWindow", "RESTORMER", nullptr));

        algo_button->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
