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
    QPushButton *load_button;
    QPushButton *download_button;
    QComboBox *select_algo;
    QDoubleSpinBox *select_2_bruit;
    QFrame *start_frame;
    QLabel *start_label;
    QWidget *fond_image_1;
    QPushButton *submit_button;
    QFrame *seg_frame;
    QLabel *seg_label;
    QWidget *fond_image_2;
    QComboBox *select_comp;
    QPushButton *compress_button;
    QFrame *decomp_frame;
    QLabel *decomp_label;
    QWidget *fond_image_3;
    QComboBox *select_comp_2;
    QLabel *pal_path;
    QLabel *index_path;
    QLabel *text_path;
    QLabel *indexbis_path;
    QPushButton *select_pal;
    QPushButton *select_index;
    QPushButton *select_indexbis;
    QPushButton *select_txt;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QPushButton *decomp_button;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_1_bruit;
    QLabel *label_2_bruit;
    QLabel *logoLabel;
    QLabel *PSNR_label;
    QLabel *taux_label;
    QComboBox *select_format;
    QPushButton *download_comp_button;
    QDoubleSpinBox *select_1_bruit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1472, 790);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        load_button = new QPushButton(centralwidget);
        load_button->setObjectName(QString::fromUtf8("load_button"));
        load_button->setGeometry(QRect(100, 450, 141, 24));
        download_button = new QPushButton(centralwidget);
        download_button->setObjectName(QString::fromUtf8("download_button"));
        download_button->setGeometry(QRect(1280, 670, 80, 24));
        select_algo = new QComboBox(centralwidget);
        select_algo->addItem(QString());
        select_algo->addItem(QString());
        select_algo->addItem(QString());
        select_algo->addItem(QString());
        select_algo->addItem(QString());
        select_algo->addItem(QString());
        select_algo->setObjectName(QString::fromUtf8("select_algo"));
        select_algo->setGeometry(QRect(510, 180, 111, 24));
        select_2_bruit = new QDoubleSpinBox(centralwidget);
        select_2_bruit->setObjectName(QString::fromUtf8("select_2_bruit"));
        select_2_bruit->setEnabled(true);
        select_2_bruit->setGeometry(QRect(510, 280, 111, 25));
        select_2_bruit->setSingleStep(0.500000000000000);
        start_frame = new QFrame(centralwidget);
        start_frame->setObjectName(QString::fromUtf8("start_frame"));
        start_frame->setGeometry(QRect(30, 140, 300, 300));
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
        submit_button = new QPushButton(centralwidget);
        submit_button->setObjectName(QString::fromUtf8("submit_button"));
        submit_button->setGeometry(QRect(440, 390, 80, 24));
        seg_frame = new QFrame(centralwidget);
        seg_frame->setObjectName(QString::fromUtf8("seg_frame"));
        seg_frame->setGeometry(QRect(640, 140, 300, 300));
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
        select_comp = new QComboBox(centralwidget);
        select_comp->addItem(QString());
        select_comp->addItem(QString());
        select_comp->addItem(QString());
        select_comp->addItem(QString());
        select_comp->setObjectName(QString::fromUtf8("select_comp"));
        select_comp->setGeometry(QRect(790, 450, 191, 24));
        compress_button = new QPushButton(centralwidget);
        compress_button->setObjectName(QString::fromUtf8("compress_button"));
        compress_button->setGeometry(QRect(750, 500, 80, 24));
        decomp_frame = new QFrame(centralwidget);
        decomp_frame->setObjectName(QString::fromUtf8("decomp_frame"));
        decomp_frame->setGeometry(QRect(1100, 290, 300, 300));
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
        pal_path = new QLabel(centralwidget);
        pal_path->setObjectName(QString::fromUtf8("pal_path"));
        pal_path->setGeometry(QRect(1180, 130, 91, 20));
        index_path = new QLabel(centralwidget);
        index_path->setObjectName(QString::fromUtf8("index_path"));
        index_path->setGeometry(QRect(1180, 160, 91, 20));
        text_path = new QLabel(centralwidget);
        text_path->setObjectName(QString::fromUtf8("text_path"));
        text_path->setGeometry(QRect(1180, 220, 91, 20));
        indexbis_path = new QLabel(centralwidget);
        indexbis_path->setObjectName(QString::fromUtf8("indexbis_path"));
        indexbis_path->setGeometry(QRect(1180, 190, 91, 20));
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
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(630, 450, 161, 16));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(340, 160, 161, 61));
        label_1_bruit = new QLabel(centralwidget);
        label_1_bruit->setObjectName(QString::fromUtf8("label_1_bruit"));
        label_1_bruit->setEnabled(true);
        label_1_bruit->setGeometry(QRect(420, 250, 71, 20));
        label_2_bruit = new QLabel(centralwidget);
        label_2_bruit->setObjectName(QString::fromUtf8("label_2_bruit"));
        label_2_bruit->setEnabled(true);
        label_2_bruit->setGeometry(QRect(420, 280, 71, 20));
        logoLabel = new QLabel(centralwidget);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        logoLabel->setGeometry(QRect(350, 0, 831, 81));
        PSNR_label = new QLabel(centralwidget);
        PSNR_label->setObjectName(QString::fromUtf8("PSNR_label"));
        PSNR_label->setGeometry(QRect(1100, 600, 301, 21));
        PSNR_label->setAlignment(Qt::AlignCenter);
        taux_label = new QLabel(centralwidget);
        taux_label->setObjectName(QString::fromUtf8("taux_label"));
        taux_label->setGeometry(QRect(638, 530, 301, 20));
        select_format = new QComboBox(centralwidget);
        select_format->addItem(QString());
        select_format->addItem(QString());
        select_format->addItem(QString());
        select_format->setObjectName(QString::fromUtf8("select_format"));
        select_format->setGeometry(QRect(1130, 670, 91, 24));
        download_comp_button = new QPushButton(centralwidget);
        download_comp_button->setObjectName(QString::fromUtf8("download_comp_button"));
        download_comp_button->setGeometry(QRect(750, 560, 80, 24));
        select_1_bruit = new QDoubleSpinBox(centralwidget);
        select_1_bruit->setObjectName(QString::fromUtf8("select_1_bruit"));
        select_1_bruit->setEnabled(true);
        select_1_bruit->setGeometry(QRect(510, 250, 111, 25));
        select_1_bruit->setSingleStep(0.500000000000000);
        MainWindow->setCentralWidget(centralwidget);
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
        load_button->setText(QCoreApplication::translate("MainWindow", "Charger une image", nullptr));
        download_button->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251charger", nullptr));
        select_algo->setItemText(0, QString());
        select_algo->setItemText(1, QCoreApplication::translate("MainWindow", "GAUSSIEN", nullptr));
        select_algo->setItemText(2, QCoreApplication::translate("MainWindow", "IMPULSIF", nullptr));
        select_algo->setItemText(3, QCoreApplication::translate("MainWindow", "POISSON", nullptr));
        select_algo->setItemText(4, QCoreApplication::translate("MainWindow", "POIVRE SEL", nullptr));
        select_algo->setItemText(5, QCoreApplication::translate("MainWindow", "SPECKLE", nullptr));

        start_label->setText(QString());
        submit_button->setText(QCoreApplication::translate("MainWindow", "Bruiter", nullptr));
        seg_label->setText(QString());
        select_comp->setItemText(0, QString());
        select_comp->setItemText(1, QCoreApplication::translate("MainWindow", "Palette couleur (256)", nullptr));
        select_comp->setItemText(2, QCoreApplication::translate("MainWindow", "Palette couleur (65536)", nullptr));
        select_comp->setItemText(3, QCoreApplication::translate("MainWindow", "Fichier texte", nullptr));

        compress_button->setText(QCoreApplication::translate("MainWindow", "Compresser", nullptr));
        decomp_label->setText(QString());
        select_comp_2->setItemText(0, QString());
        select_comp_2->setItemText(1, QCoreApplication::translate("MainWindow", "Palette couleur (256)", nullptr));
        select_comp_2->setItemText(2, QCoreApplication::translate("MainWindow", "Palette couleur (65536)", nullptr));
        select_comp_2->setItemText(3, QCoreApplication::translate("MainWindow", "Fichier texte", nullptr));

        pal_path->setText(QString());
        index_path->setText(QString());
        text_path->setText(QString());
        indexbis_path->setText(QString());
        select_pal->setText(QCoreApplication::translate("MainWindow", "S\303\251lectionner", nullptr));
        select_index->setText(QCoreApplication::translate("MainWindow", "S\303\251lectionner", nullptr));
        select_indexbis->setText(QCoreApplication::translate("MainWindow", "S\303\251lectionner", nullptr));
        select_txt->setText(QCoreApplication::translate("MainWindow", "S\303\251lectionner", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Palette", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Index", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Index (bis)", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Fichier txt", nullptr));
        decomp_button->setText(QCoreApplication::translate("MainWindow", "D\303\251compresser", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Algorithme de compression :", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Algorithme de bruitage :", nullptr));
        label_1_bruit->setText(QString());
        label_2_bruit->setText(QString());
        logoLabel->setText(QString());
        PSNR_label->setText(QString());
        taux_label->setText(QString());
        select_format->setItemText(0, QCoreApplication::translate("MainWindow", "PPM", nullptr));
        select_format->setItemText(1, QCoreApplication::translate("MainWindow", "PNG", nullptr));
        select_format->setItemText(2, QCoreApplication::translate("MainWindow", "JPG", nullptr));

        download_comp_button->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251charger", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
