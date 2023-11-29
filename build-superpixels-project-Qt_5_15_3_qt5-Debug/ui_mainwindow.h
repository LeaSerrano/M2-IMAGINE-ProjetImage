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
    QComboBox *select_algo;
    QDoubleSpinBox *select_2_bruit;
    QFrame *decomp_frame;
    QLabel *decomp_label;
    QWidget *fond_image_3;
    QComboBox *select_comp_2;
    QPushButton *select_pal;
    QPushButton *select_index;
    QPushButton *select_indexbis;
    QPushButton *select_txt;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QPushButton *decomp_button;
    QLabel *label_2;
    QLabel *label_1_bruit;
    QLabel *label_2_bruit;
    QLabel *logoLabel;
    QLabel *PSNR_label;
    QComboBox *select_format2;
    QDoubleSpinBox *select_1_bruit;
    QFrame *frame_1;
    QFrame *start_frame;
    QLabel *start_label;
    QWidget *fond_image_1;
    QPushButton *upload_button;
    QFrame *frame_2;
    QPushButton *submit_button;
    QFrame *seg_frame;
    QLabel *seg_label;
    QWidget *fond_image_2;
    QPushButton *download_button_1;
    QComboBox *select_format1;
    QFrame *frame_3;
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
        select_algo = new QComboBox(centralwidget);
        select_algo->addItem(QString());
        select_algo->addItem(QString());
        select_algo->addItem(QString());
        select_algo->addItem(QString());
        select_algo->addItem(QString());
        select_algo->addItem(QString());
        select_algo->setObjectName(QString::fromUtf8("select_algo"));
        select_algo->setGeometry(QRect(750, 190, 111, 24));
        select_2_bruit = new QDoubleSpinBox(centralwidget);
        select_2_bruit->setObjectName(QString::fromUtf8("select_2_bruit"));
        select_2_bruit->setEnabled(true);
        select_2_bruit->setGeometry(QRect(750, 270, 111, 25));
        select_2_bruit->setSingleStep(0.500000000000000);
        decomp_frame = new QFrame(centralwidget);
        decomp_frame->setObjectName(QString::fromUtf8("decomp_frame"));
        decomp_frame->setGeometry(QRect(1100, 370, 300, 300));
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
        select_comp_2 = new QComboBox(centralwidget);
        select_comp_2->addItem(QString());
        select_comp_2->addItem(QString());
        select_comp_2->addItem(QString());
        select_comp_2->addItem(QString());
        select_comp_2->setObjectName(QString::fromUtf8("select_comp_2"));
        select_comp_2->setGeometry(QRect(1100, 90, 111, 24));
        select_pal = new QPushButton(centralwidget);
        select_pal->setObjectName(QString::fromUtf8("select_pal"));
        select_pal->setGeometry(QRect(1320, 120, 80, 24));
        select_index = new QPushButton(centralwidget);
        select_index->setObjectName(QString::fromUtf8("select_index"));
        select_index->setGeometry(QRect(1320, 150, 80, 24));
        select_indexbis = new QPushButton(centralwidget);
        select_indexbis->setObjectName(QString::fromUtf8("select_indexbis"));
        select_indexbis->setGeometry(QRect(1320, 180, 80, 24));
        select_txt = new QPushButton(centralwidget);
        select_txt->setObjectName(QString::fromUtf8("select_txt"));
        select_txt->setGeometry(QRect(1320, 210, 80, 24));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(1100, 130, 61, 20));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(1100, 160, 61, 20));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(1100, 190, 61, 20));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(1100, 220, 61, 20));
        decomp_button = new QPushButton(centralwidget);
        decomp_button->setObjectName(QString::fromUtf8("decomp_button"));
        decomp_button->setGeometry(QRect(1200, 250, 91, 24));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(580, 170, 161, 61));
        label_1_bruit = new QLabel(centralwidget);
        label_1_bruit->setObjectName(QString::fromUtf8("label_1_bruit"));
        label_1_bruit->setEnabled(true);
        label_1_bruit->setGeometry(QRect(650, 240, 81, 25));
        label_2_bruit = new QLabel(centralwidget);
        label_2_bruit->setObjectName(QString::fromUtf8("label_2_bruit"));
        label_2_bruit->setEnabled(true);
        label_2_bruit->setGeometry(QRect(650, 270, 81, 25));
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
        select_1_bruit = new QDoubleSpinBox(centralwidget);
        select_1_bruit->setObjectName(QString::fromUtf8("select_1_bruit"));
        select_1_bruit->setEnabled(true);
        select_1_bruit->setGeometry(QRect(750, 240, 111, 25));
        select_1_bruit->setSingleStep(0.500000000000000);
        frame_1 = new QFrame(centralwidget);
        frame_1->setObjectName(QString::fromUtf8("frame_1"));
        frame_1->setGeometry(QRect(20, 130, 461, 621));
        frame_1->setFrameShape(QFrame::StyledPanel);
        frame_1->setFrameShadow(QFrame::Raised);
        start_frame = new QFrame(frame_1);
        start_frame->setObjectName(QString::fromUtf8("start_frame"));
        start_frame->setGeometry(QRect(60, 100, 300, 300));
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
        upload_button->setGeometry(QRect(60, 70, 30, 30));
        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(505, 130, 461, 621));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        submit_button = new QPushButton(frame_2);
        submit_button->setObjectName(QString::fromUtf8("submit_button"));
        submit_button->setGeometry(QRect(150, 190, 131, 41));
        seg_frame = new QFrame(frame_2);
        seg_frame->setObjectName(QString::fromUtf8("seg_frame"));
        seg_frame->setGeometry(QRect(70, 250, 300, 300));
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
        download_button_1->setGeometry(QRect(340, 550, 30, 30));
        select_format1 = new QComboBox(frame_2);
        select_format1->addItem(QString());
        select_format1->addItem(QString());
        select_format1->addItem(QString());
        select_format1->setObjectName(QString::fromUtf8("select_format1"));
        select_format1->setEnabled(true);
        select_format1->setGeometry(QRect(90, 560, 91, 24));
        frame_3 = new QFrame(centralwidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(990, 130, 461, 621));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        MainWindow->setCentralWidget(centralwidget);
        logoLabel->raise();
        PSNR_label->raise();
        frame_1->raise();
        frame_2->raise();
        frame_3->raise();
        label_1_bruit->raise();
        select_algo->raise();
        label_2_bruit->raise();
        select_1_bruit->raise();
        select_2_bruit->raise();
        label_2->raise();
        label_5->raise();
        select_index->raise();
        label_8->raise();
        decomp_frame->raise();
        select_txt->raise();
        select_pal->raise();
        select_comp_2->raise();
        label_7->raise();
        download_button_2->raise();
        select_format2->raise();
        decomp_button->raise();
        select_indexbis->raise();
        label_6->raise();
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
        select_algo->setItemText(0, QString());
        select_algo->setItemText(1, QCoreApplication::translate("MainWindow", "GAUSSIEN", nullptr));
        select_algo->setItemText(2, QCoreApplication::translate("MainWindow", "IMPULSIF", nullptr));
        select_algo->setItemText(3, QCoreApplication::translate("MainWindow", "POISSON", nullptr));
        select_algo->setItemText(4, QCoreApplication::translate("MainWindow", "POIVRE SEL", nullptr));
        select_algo->setItemText(5, QCoreApplication::translate("MainWindow", "SPECKLE", nullptr));

        decomp_label->setText(QString());
        select_comp_2->setItemText(0, QString());
        select_comp_2->setItemText(1, QCoreApplication::translate("MainWindow", "Palette couleur (256)", nullptr));
        select_comp_2->setItemText(2, QCoreApplication::translate("MainWindow", "Palette couleur (65536)", nullptr));
        select_comp_2->setItemText(3, QCoreApplication::translate("MainWindow", "Fichier texte", nullptr));

        select_pal->setText(QCoreApplication::translate("MainWindow", "S\303\251lectionner", nullptr));
        select_index->setText(QCoreApplication::translate("MainWindow", "S\303\251lectionner", nullptr));
        select_indexbis->setText(QCoreApplication::translate("MainWindow", "S\303\251lectionner", nullptr));
        select_txt->setText(QCoreApplication::translate("MainWindow", "S\303\251lectionner", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Palette", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Index", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Index (bis)", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Fichier txt", nullptr));
        decomp_button->setText(QCoreApplication::translate("MainWindow", "D\303\251compresser", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Algorithme de bruitage :", nullptr));
        label_1_bruit->setText(QString());
        label_2_bruit->setText(QString());
        logoLabel->setText(QCoreApplication::translate("MainWindow", "DENOISEPRO", nullptr));
        PSNR_label->setText(QString());
        select_format2->setItemText(0, QCoreApplication::translate("MainWindow", "PPM", nullptr));
        select_format2->setItemText(1, QCoreApplication::translate("MainWindow", "PNG", nullptr));
        select_format2->setItemText(2, QCoreApplication::translate("MainWindow", "JPG", nullptr));

        start_label->setText(QString());
        upload_button->setText(QString());
        submit_button->setText(QCoreApplication::translate("MainWindow", "Bruiter", nullptr));
        seg_label->setText(QString());
        download_button_1->setText(QString());
        select_format1->setItemText(0, QCoreApplication::translate("MainWindow", "PPM", nullptr));
        select_format1->setItemText(1, QCoreApplication::translate("MainWindow", "PNG", nullptr));
        select_format1->setItemText(2, QCoreApplication::translate("MainWindow", "JPG", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
