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
    QPushButton *download_button_2;
    QFrame *decomp_frame;
    QLabel *decomp_label;
    QWidget *fond_image_3;
    QLabel *logoLabel;
    QLabel *PSNR_label;
    QComboBox *select_format2;
    QFrame *frame_1;
    QFrame *start_frame;
    QLabel *start_label;
    QWidget *fond_image_1;
    QPushButton *upload_button;
    QFrame *frame_2;
    QPushButton *submit_button_noise;
    QFrame *seg_frame;
    QLabel *seg_label;
    QWidget *fond_image_2;
    QPushButton *download_button_1;
    QComboBox *select_format1;
    QLabel *label_2;
    QDoubleSpinBox *select_2_bruit;
    QDoubleSpinBox *select_1_bruit;
    QComboBox *select_algo_bruit;
    QLabel *label_2_bruit;
    QLabel *label_1_bruit;
    QFrame *frame_3;
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
    QComboBox *select_algo_debruit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1472, 790);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        download_button_2 = new QPushButton(centralwidget);
        download_button_2->setObjectName(QString::fromUtf8("download_button_2"));
        download_button_2->setGeometry(QRect(1260, 690, 111, 24));
        decomp_frame = new QFrame(centralwidget);
        decomp_frame->setObjectName(QString::fromUtf8("decomp_frame"));
        decomp_frame->setGeometry(QRect(1100, 381, 300, 300));
        decomp_frame->setFrameShape(QFrame::StyledPanel);
        decomp_frame->setFrameShadow(QFrame::Raised);
        decomp_label = new QLabel(decomp_frame);
        decomp_label->setObjectName(QString::fromUtf8("decomp_label"));
        decomp_label->setGeometry(QRect(0, 0, 300, 300));
        fond_image_3 = new QWidget(decomp_frame);
        fond_image_3->setObjectName(QString::fromUtf8("fond_image_3"));
        fond_image_3->setGeometry(QRect(-10, -10, 321, 321));
        fond_image_3->raise();
        decomp_label->raise();
        logoLabel = new QLabel(centralwidget);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        logoLabel->setGeometry(QRect(350, 10, 831, 81));
        PSNR_label = new QLabel(centralwidget);
        PSNR_label->setObjectName(QString::fromUtf8("PSNR_label"));
        PSNR_label->setGeometry(QRect(1100, 600, 301, 21));
        PSNR_label->setAlignment(Qt::AlignCenter);
        select_format2 = new QComboBox(centralwidget);
        select_format2->addItem(QString());
        select_format2->addItem(QString());
        select_format2->addItem(QString());
        select_format2->setObjectName(QString::fromUtf8("select_format2"));
        select_format2->setGeometry(QRect(1140, 690, 91, 24));
        frame_1 = new QFrame(centralwidget);
        frame_1->setObjectName(QString::fromUtf8("frame_1"));
        frame_1->setGeometry(QRect(20, 10, 461, 741));
        frame_1->setFrameShape(QFrame::StyledPanel);
        frame_1->setFrameShadow(QFrame::Raised);
        start_frame = new QFrame(frame_1);
        start_frame->setObjectName(QString::fromUtf8("start_frame"));
        start_frame->setGeometry(QRect(60, 60, 300, 300));
        start_frame->setFrameShape(QFrame::StyledPanel);
        start_frame->setFrameShadow(QFrame::Raised);
        start_label = new QLabel(start_frame);
        start_label->setObjectName(QString::fromUtf8("start_label"));
        start_label->setGeometry(QRect(0, 0, 300, 300));
        fond_image_1 = new QWidget(start_frame);
        fond_image_1->setObjectName(QString::fromUtf8("fond_image_1"));
        fond_image_1->setGeometry(QRect(-10, -10, 321, 321));
        fond_image_1->raise();
        start_label->raise();
        upload_button = new QPushButton(frame_1);
        upload_button->setObjectName(QString::fromUtf8("upload_button"));
        upload_button->setGeometry(QRect(60, 30, 30, 30));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(505, 10, 461, 741));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        submit_button_noise = new QPushButton(frame_2);
        submit_button_noise->setObjectName(QString::fromUtf8("submit_button_noise"));
        submit_button_noise->setGeometry(QRect(150, 190, 131, 41));
        seg_frame = new QFrame(frame_2);
        seg_frame->setObjectName(QString::fromUtf8("seg_frame"));
        seg_frame->setGeometry(QRect(80, 350, 300, 300));
        seg_frame->setFrameShape(QFrame::StyledPanel);
        seg_frame->setFrameShadow(QFrame::Raised);
        seg_label = new QLabel(seg_frame);
        seg_label->setObjectName(QString::fromUtf8("seg_label"));
        seg_label->setGeometry(QRect(0, 0, 300, 300));
        fond_image_2 = new QWidget(seg_frame);
        fond_image_2->setObjectName(QString::fromUtf8("fond_image_2"));
        fond_image_2->setGeometry(QRect(-10, -10, 321, 321));
        fond_image_2->raise();
        seg_label->raise();
        download_button_1 = new QPushButton(frame_2);
        download_button_1->setObjectName(QString::fromUtf8("download_button_1"));
        download_button_1->setGeometry(QRect(350, 690, 30, 30));
        select_format1 = new QComboBox(frame_2);
        select_format1->addItem(QString());
        select_format1->addItem(QString());
        select_format1->addItem(QString());
        select_format1->setObjectName(QString::fromUtf8("select_format1"));
        select_format1->setEnabled(true);
        select_format1->setGeometry(QRect(90, 690, 91, 24));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 30, 171, 61));
        select_2_bruit = new QDoubleSpinBox(frame_2);
        select_2_bruit->setObjectName(QString::fromUtf8("select_2_bruit"));
        select_2_bruit->setEnabled(true);
        select_2_bruit->setGeometry(QRect(250, 130, 111, 25));
        select_2_bruit->setSingleStep(0.500000000000000);
        select_1_bruit = new QDoubleSpinBox(frame_2);
        select_1_bruit->setObjectName(QString::fromUtf8("select_1_bruit"));
        select_1_bruit->setEnabled(true);
        select_1_bruit->setGeometry(QRect(250, 100, 111, 25));
        select_1_bruit->setMaximum(300.000000000000000);
        select_1_bruit->setSingleStep(0.500000000000000);
        select_algo_bruit = new QComboBox(frame_2);
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->setObjectName(QString::fromUtf8("select_algo_bruit"));
        select_algo_bruit->setGeometry(QRect(250, 50, 111, 24));
        label_2_bruit = new QLabel(frame_2);
        label_2_bruit->setObjectName(QString::fromUtf8("label_2_bruit"));
        label_2_bruit->setEnabled(true);
        label_2_bruit->setGeometry(QRect(140, 130, 81, 25));
        label_1_bruit = new QLabel(frame_2);
        label_1_bruit->setObjectName(QString::fromUtf8("label_1_bruit"));
        label_1_bruit->setEnabled(true);
        label_1_bruit->setGeometry(QRect(140, 100, 81, 25));
        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(990, 10, 461, 741));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        label_1_debruit = new QLabel(frame_3);
        label_1_debruit->setObjectName(QString::fromUtf8("label_1_debruit"));
        label_1_debruit->setGeometry(QRect(160, 90, 111, 25));
        label_4_debruit = new QLabel(frame_3);
        label_4_debruit->setObjectName(QString::fromUtf8("label_4_debruit"));
        label_4_debruit->setGeometry(QRect(160, 180, 111, 25));
        submit_button_denoise = new QPushButton(frame_3);
        submit_button_denoise->setObjectName(QString::fromUtf8("submit_button_denoise"));
        submit_button_denoise->setGeometry(QRect(180, 260, 131, 41));
        label_2_debruit = new QLabel(frame_3);
        label_2_debruit->setObjectName(QString::fromUtf8("label_2_debruit"));
        label_2_debruit->setGeometry(QRect(160, 120, 111, 25));
        label_3_debruit = new QLabel(frame_3);
        label_3_debruit->setObjectName(QString::fromUtf8("label_3_debruit"));
        label_3_debruit->setGeometry(QRect(160, 150, 111, 25));
        label_3 = new QLabel(frame_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 10, 201, 61));
        select_1_debruit = new QDoubleSpinBox(frame_3);
        select_1_debruit->setObjectName(QString::fromUtf8("select_1_debruit"));
        select_1_debruit->setEnabled(true);
        select_1_debruit->setGeometry(QRect(290, 90, 111, 25));
        select_1_debruit->setMaximum(300.000000000000000);
        select_1_debruit->setSingleStep(0.500000000000000);
        select_2_debruit = new QDoubleSpinBox(frame_3);
        select_2_debruit->setObjectName(QString::fromUtf8("select_2_debruit"));
        select_2_debruit->setEnabled(true);
        select_2_debruit->setGeometry(QRect(290, 120, 111, 25));
        select_2_debruit->setSingleStep(0.500000000000000);
        select_4_debruit = new QDoubleSpinBox(frame_3);
        select_4_debruit->setObjectName(QString::fromUtf8("select_4_debruit"));
        select_4_debruit->setEnabled(true);
        select_4_debruit->setGeometry(QRect(290, 180, 111, 25));
        select_4_debruit->setSingleStep(0.500000000000000);
        select_3_debruit = new QDoubleSpinBox(frame_3);
        select_3_debruit->setObjectName(QString::fromUtf8("select_3_debruit"));
        select_3_debruit->setEnabled(true);
        select_3_debruit->setGeometry(QRect(290, 150, 111, 25));
        select_3_debruit->setMaximum(300.000000000000000);
        select_3_debruit->setSingleStep(0.500000000000000);
        select_algo_debruit = new QComboBox(frame_3);
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
        select_algo_debruit->setGeometry(QRect(260, 30, 141, 24));
        MainWindow->setCentralWidget(centralwidget);
        logoLabel->raise();
        PSNR_label->raise();
        frame_1->raise();
        frame_2->raise();
        frame_3->raise();
        decomp_frame->raise();
        download_button_2->raise();
        select_format2->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1472, 22));
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
        download_button_2->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251charger", nullptr));
        decomp_label->setText(QString());
        logoLabel->setText(QString());
        PSNR_label->setText(QString());
        select_format2->setItemText(0, QCoreApplication::translate("MainWindow", "PPM", nullptr));
        select_format2->setItemText(1, QCoreApplication::translate("MainWindow", "PNG", nullptr));
        select_format2->setItemText(2, QCoreApplication::translate("MainWindow", "JPG", nullptr));

        start_label->setText(QString());
        upload_button->setText(QString());
        submit_button_noise->setText(QCoreApplication::translate("MainWindow", "Bruiter", nullptr));
        seg_label->setText(QString());
        download_button_1->setText(QString());
        select_format1->setItemText(0, QCoreApplication::translate("MainWindow", "PPM", nullptr));
        select_format1->setItemText(1, QCoreApplication::translate("MainWindow", "PNG", nullptr));
        select_format1->setItemText(2, QCoreApplication::translate("MainWindow", "JPG", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "Algorithme de bruitage :", nullptr));
        select_algo_bruit->setItemText(0, QString());
        select_algo_bruit->setItemText(1, QCoreApplication::translate("MainWindow", "GAUSSIEN", nullptr));
        select_algo_bruit->setItemText(2, QCoreApplication::translate("MainWindow", "IMPULSIF", nullptr));
        select_algo_bruit->setItemText(3, QCoreApplication::translate("MainWindow", "POISSON", nullptr));
        select_algo_bruit->setItemText(4, QCoreApplication::translate("MainWindow", "POIVRE SEL", nullptr));
        select_algo_bruit->setItemText(5, QCoreApplication::translate("MainWindow", "SPECKLE", nullptr));

        label_2_bruit->setText(QString());
        label_1_bruit->setText(QString());
        label_1_debruit->setText(QString());
        label_4_debruit->setText(QString());
        submit_button_denoise->setText(QCoreApplication::translate("MainWindow", "D\303\251bruiter", nullptr));
        label_2_debruit->setText(QString());
        label_3_debruit->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "Algorithme de d\303\251bruitage :", nullptr));
        select_algo_debruit->setItemText(0, QString());
        select_algo_debruit->setItemText(1, QCoreApplication::translate("MainWindow", "MOYENNEUR", nullptr));
        select_algo_debruit->setItemText(2, QCoreApplication::translate("MainWindow", "MEDIAN", nullptr));
        select_algo_debruit->setItemText(3, QCoreApplication::translate("MainWindow", "WIENER", nullptr));
        select_algo_debruit->setItemText(4, QCoreApplication::translate("MainWindow", "GAUSSIEN", nullptr));
        select_algo_debruit->setItemText(5, QCoreApplication::translate("MainWindow", "GRADIENT SEUILLEE", nullptr));
        select_algo_debruit->setItemText(6, QCoreApplication::translate("MainWindow", "MOYENNEUR PONDERE", nullptr));
        select_algo_debruit->setItemText(7, QCoreApplication::translate("MainWindow", "NON LOCAL MEANS", nullptr));
        select_algo_debruit->setItemText(8, QCoreApplication::translate("MainWindow", "RESTORMER", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
