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
    QFrame *frame_21_1;
    QFrame *frame_22_1;
    QFrame *frame_23_1;
    QPushButton *submit_button_noise;
    QComboBox *select_algo_bruit;
    QLabel *label_2;
    QComboBox *select_algo_debruit;
    QFrame *frame_24_1;
    QPushButton *metric_button_f2;
    QPushButton *algo_button_f2;
    QFrame *frame_3;
    QLabel *label_help;
    QLabel *label_4;
    QLabel *label_psnr;
    QLabel *label_snr;
    QFrame *frame_21_2;
    QFrame *frame_22_2;
    QFrame *frame_23_2;
    QLabel *label_5;
    QFrame *frame_24_2;
    QPushButton *metric_button_f3;
    QPushButton *algo_button_f3;
    QLabel *label_ssim;
    QLabel *label_rmse;
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
        upload_button->setGeometry(QRect(20, 20, 35, 35));
        download_button = new QPushButton(frame_1);
        download_button->setObjectName(QString::fromUtf8("download_button"));
        download_button->setGeometry(QRect(70, 20, 35, 35));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setEnabled(true);
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
        label_1_bruit->setEnabled(false);
        label_1_bruit->setGeometry(QRect(20, 180, 131, 25));
        label_2_bruit = new QLabel(frame_2);
        label_2_bruit->setObjectName(QString::fromUtf8("label_2_bruit"));
        label_2_bruit->setEnabled(false);
        label_2_bruit->setGeometry(QRect(20, 210, 131, 25));
        select_1_bruit = new QDoubleSpinBox(frame_2);
        select_1_bruit->setObjectName(QString::fromUtf8("select_1_bruit"));
        select_1_bruit->setEnabled(true);
        select_1_bruit->setGeometry(QRect(180, 180, 111, 25));
        select_1_bruit->setMaximum(300.000000000000000);
        select_1_bruit->setSingleStep(0.500000000000000);
        select_2_bruit = new QDoubleSpinBox(frame_2);
        select_2_bruit->setObjectName(QString::fromUtf8("select_2_bruit"));
        select_2_bruit->setEnabled(true);
        select_2_bruit->setGeometry(QRect(180, 210, 111, 25));
        select_2_bruit->setSingleStep(0.500000000000000);
        frame_21_1 = new QFrame(frame_2);
        frame_21_1->setObjectName(QString::fromUtf8("frame_21_1"));
        frame_21_1->setGeometry(QRect(0, 70, 311, 91));
        frame_21_1->setFrameShape(QFrame::HLine);
        frame_21_1->setFrameShadow(QFrame::Plain);
        frame_21_1->setLineWidth(2);
        frame_22_1 = new QFrame(frame_2);
        frame_22_1->setObjectName(QString::fromUtf8("frame_22_1"));
        frame_22_1->setGeometry(QRect(0, 300, 311, 61));
        frame_22_1->setFrameShape(QFrame::HLine);
        frame_22_1->setFrameShadow(QFrame::Plain);
        frame_22_1->setLineWidth(2);
        frame_23_1 = new QFrame(frame_2);
        frame_23_1->setObjectName(QString::fromUtf8("frame_23_1"));
        frame_23_1->setGeometry(QRect(0, 350, 311, 91));
        frame_23_1->setFrameShape(QFrame::HLine);
        frame_23_1->setFrameShadow(QFrame::Plain);
        frame_23_1->setLineWidth(2);
        submit_button_noise = new QPushButton(frame_2);
        submit_button_noise->setObjectName(QString::fromUtf8("submit_button_noise"));
        submit_button_noise->setGeometry(QRect(80, 260, 131, 41));
        select_algo_bruit = new QComboBox(frame_2);
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->addItem(QString());
        select_algo_bruit->setObjectName(QString::fromUtf8("select_algo_bruit"));
        select_algo_bruit->setGeometry(QRect(60, 130, 187, 24));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 54, 311, 61));
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
        frame_24_1 = new QFrame(frame_2);
        frame_24_1->setObjectName(QString::fromUtf8("frame_24_1"));
        frame_24_1->setGeometry(QRect(0, 24, 311, 61));
        frame_24_1->setFrameShape(QFrame::HLine);
        frame_24_1->setFrameShadow(QFrame::Plain);
        frame_24_1->setLineWidth(2);
        metric_button_f2 = new QPushButton(frame_2);
        metric_button_f2->setObjectName(QString::fromUtf8("metric_button_f2"));
        metric_button_f2->setGeometry(QRect(225, 10, 35, 35));
        algo_button_f2 = new QPushButton(frame_2);
        algo_button_f2->setObjectName(QString::fromUtf8("algo_button_f2"));
        algo_button_f2->setGeometry(QRect(50, 10, 35, 35));
        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(780, 10, 311, 676));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        label_help = new QLabel(frame_3);
        label_help->setObjectName(QString::fromUtf8("label_help"));
        label_help->setGeometry(QRect(20, 410, 271, 261));
        label_4 = new QLabel(frame_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(0, 333, 311, 61));
        label_4->setAlignment(Qt::AlignCenter);
        label_psnr = new QLabel(frame_3);
        label_psnr->setObjectName(QString::fromUtf8("label_psnr"));
        label_psnr->setEnabled(true);
        label_psnr->setGeometry(QRect(20, 170, 131, 25));
        label_snr = new QLabel(frame_3);
        label_snr->setObjectName(QString::fromUtf8("label_snr"));
        label_snr->setEnabled(true);
        label_snr->setGeometry(QRect(20, 220, 131, 25));
        frame_21_2 = new QFrame(frame_3);
        frame_21_2->setObjectName(QString::fromUtf8("frame_21_2"));
        frame_21_2->setGeometry(QRect(0, 70, 311, 91));
        frame_21_2->setFrameShape(QFrame::HLine);
        frame_21_2->setFrameShadow(QFrame::Plain);
        frame_21_2->setLineWidth(2);
        frame_22_2 = new QFrame(frame_3);
        frame_22_2->setObjectName(QString::fromUtf8("frame_22_2"));
        frame_22_2->setGeometry(QRect(0, 300, 311, 61));
        frame_22_2->setFrameShape(QFrame::HLine);
        frame_22_2->setFrameShadow(QFrame::Plain);
        frame_22_2->setLineWidth(2);
        frame_23_2 = new QFrame(frame_3);
        frame_23_2->setObjectName(QString::fromUtf8("frame_23_2"));
        frame_23_2->setGeometry(QRect(0, 350, 311, 91));
        frame_23_2->setFrameShape(QFrame::HLine);
        frame_23_2->setFrameShadow(QFrame::Plain);
        frame_23_2->setLineWidth(2);
        label_5 = new QLabel(frame_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(0, 54, 311, 61));
        label_5->setAlignment(Qt::AlignCenter);
        frame_24_2 = new QFrame(frame_3);
        frame_24_2->setObjectName(QString::fromUtf8("frame_24_2"));
        frame_24_2->setGeometry(QRect(0, 24, 311, 61));
        frame_24_2->setFrameShape(QFrame::HLine);
        frame_24_2->setFrameShadow(QFrame::Plain);
        frame_24_2->setLineWidth(2);
        metric_button_f3 = new QPushButton(frame_3);
        metric_button_f3->setObjectName(QString::fromUtf8("metric_button_f3"));
        metric_button_f3->setGeometry(QRect(225, 10, 35, 35));
        algo_button_f3 = new QPushButton(frame_3);
        algo_button_f3->setObjectName(QString::fromUtf8("algo_button_f3"));
        algo_button_f3->setGeometry(QRect(50, 10, 35, 35));
        label_ssim = new QLabel(frame_3);
        label_ssim->setObjectName(QString::fromUtf8("label_ssim"));
        label_ssim->setEnabled(true);
        label_ssim->setGeometry(QRect(160, 170, 131, 25));
        label_rmse = new QLabel(frame_3);
        label_rmse->setObjectName(QString::fromUtf8("label_rmse"));
        label_rmse->setEnabled(true);
        label_rmse->setGeometry(QRect(160, 220, 131, 25));
        MainWindow->setCentralWidget(centralwidget);
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
        start_label->setText(QString());
        upload_button->setText(QString());
        download_button->setText(QString());
        label_1_debruit->setText(QString());
        label_4_debruit->setText(QString());
        submit_button_denoise->setText(QCoreApplication::translate("MainWindow", "D\303\251bruiter", nullptr));
        label_2_debruit->setText(QString());
        label_3_debruit->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "D\303\251bruitage", nullptr));
        label_1_bruit->setText(QString());
        label_2_bruit->setText(QString());
        submit_button_noise->setText(QCoreApplication::translate("MainWindow", "Bruiter", nullptr));
        select_algo_bruit->setItemText(0, QCoreApplication::translate("MainWindow", "S\303\251lection bruit", nullptr));
        select_algo_bruit->setItemText(1, QCoreApplication::translate("MainWindow", "GAUSSIEN", nullptr));
        select_algo_bruit->setItemText(2, QCoreApplication::translate("MainWindow", "IMPULSIF", nullptr));
        select_algo_bruit->setItemText(3, QCoreApplication::translate("MainWindow", "POISSON", nullptr));
        select_algo_bruit->setItemText(4, QCoreApplication::translate("MainWindow", "POIVRE SEL", nullptr));
        select_algo_bruit->setItemText(5, QCoreApplication::translate("MainWindow", "SPECKLE", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "Bruitage", nullptr));
        select_algo_debruit->setItemText(0, QCoreApplication::translate("MainWindow", "S\303\251lection d\303\251bruitage", nullptr));
        select_algo_debruit->setItemText(1, QCoreApplication::translate("MainWindow", "MOYENNEUR", nullptr));
        select_algo_debruit->setItemText(2, QCoreApplication::translate("MainWindow", "MEDIAN", nullptr));
        select_algo_debruit->setItemText(3, QCoreApplication::translate("MainWindow", "WIENER", nullptr));
        select_algo_debruit->setItemText(4, QCoreApplication::translate("MainWindow", "GAUSSIEN", nullptr));
        select_algo_debruit->setItemText(5, QCoreApplication::translate("MainWindow", "GRADIENT SEUILLEE", nullptr));
        select_algo_debruit->setItemText(6, QCoreApplication::translate("MainWindow", "MOYENNEUR PONDERE", nullptr));
        select_algo_debruit->setItemText(7, QCoreApplication::translate("MainWindow", "NON LOCAL MEANS", nullptr));
        select_algo_debruit->setItemText(8, QCoreApplication::translate("MainWindow", "RESTORMER", nullptr));

        metric_button_f2->setText(QString());
        algo_button_f2->setText(QString());
        label_help->setText(QString());
        label_4->setText(QCoreApplication::translate("MainWindow", "Choisir son d\303\251bruitage", nullptr));
        label_psnr->setText(QString());
        label_snr->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "M\303\251triques", nullptr));
        metric_button_f3->setText(QString());
        algo_button_f3->setText(QString());
        label_ssim->setText(QString());
        label_rmse->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
