#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include "Functions_Noise_Denoise.cpp"
#include <QProcess>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    setFixedSize(1106, 719);

    ui->fond_image_1->setStyleSheet("background-color: #3d3e42;");

    ui->frame_1->setObjectName("styledFrame");
    ui->frame_2->setObjectName("styledFrame");
    ui->frame_2->raise();
    ui->frame_3->setObjectName("styledFrame");
    ui->frame_3->lower();

    ui->frame_21_1->setStyleSheet("color : #3d3e42;");
    ui->frame_23_1->setStyleSheet("color : #3d3e42;");
    ui->frame_22_1->setStyleSheet("color : #202124;");
    ui->frame_24_1->setStyleSheet("color : #202124;");
    ui->frame_21_2->setStyleSheet("color : #3d3e42;");
    ui->frame_23_2->setStyleSheet("color : #3d3e42;");
    ui->frame_22_2->setStyleSheet("color : #202124;");
    ui->frame_24_2->setStyleSheet("color : #202124;");

    ui->download_button->setObjectName("downloadButton");
    ui->upload_button->setObjectName("uploadButton");
    ui->algo_button_f2->setObjectName("algoButton");
    ui->algo_button_f3->setObjectName("algoButton");
    ui->metric_button_f2->setObjectName("metricButton");
    ui->metric_button_f3->setObjectName("metricButton");

    ui->select_1_bruit->hide();
    ui->select_2_bruit->hide();

    ui->select_1_debruit->hide();
    ui->select_2_debruit->hide();
    ui->select_3_debruit->hide();
    ui->select_4_debruit->hide();

    ui->start_frame->setStyleSheet("border : transparent;");

    connect(ui->select_algo_bruit, SIGNAL(currentIndexChanged(int)), this, SLOT(on_noise_algo_selected(int)));
    connect(ui->select_algo_debruit, SIGNAL(currentIndexChanged(int)), this, SLOT(on_denoise_algo_selected(int)));
    connect(ui->upload_button, SIGNAL(clicked()), this, SLOT(on_submit_button_upload_clicked()));
    connect(ui->download_button, SIGNAL(clicked()), this, SLOT(on_submit_button_download_clicked()));
    connect(ui->submit_button_noise, SIGNAL(clicked()), this, SLOT(on_submit_button_noise_clicked()));
    connect(ui->submit_button_denoise, SIGNAL(clicked()), this, SLOT(on_submit_button_denoise_clicked()));
    connect(ui->download_button, SIGNAL(clicked()), this, SLOT(on_submit_button_download_clicked()));
    connect(ui->algo_button_f2, SIGNAL(clicked()), this, SLOT(on_submit_button_algo_clicked()));
    connect(ui->algo_button_f3, SIGNAL(clicked()), this, SLOT(on_submit_button_algo_clicked()));
    connect(ui->metric_button_f2, SIGNAL(clicked()), this, SLOT(on_submit_button_metric_clicked()));
    connect(ui->metric_button_f3, SIGNAL(clicked()), this, SLOT(on_submit_button_metric_clicked()));


    QString longText = "Meilleur filtre pour retirer les bruits : <br> - Bruit poivre et sel -&gt; filtre médian <br> - Bruit poisson -&gt; filtre moyenneur pondéré <br> - Bruit Speckle -&gt; Non Local Means si l’intensité &lt; 10, filtre pondéré si l’intensité &lt; 20, filtre gaussien sinon <br> - Bruit gaussien -&gt; Non Local Means si l’écart-type &lt; 10, le filtre médian sinon <br> - Bruit impulsif -&gt; filtre pondéré si l’intensité &lt;= à 2.5, filtre médian sinon";

    ui->label_help->setText(longText);
    ui->label_help->setWordWrap(true);
    ui->label_help->setTextFormat(Qt::RichText);
    ui->label_help->setStyleSheet("white-space: pre;");

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString extensionImageIn;

/*--------------------- ALGORITHMES POUR QT ---------------------*/

void MainWindow::on_submit_button_download_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Enregistrer l'image"), "", tr("Images (*.png *.jpg *.jpeg *.ppm *.pgm)"));

    if (!filePath.isEmpty()) {
        saveImage(filePath);
        QMessageBox::information(this, tr("Téléchargement terminé"), tr("L'image a été téléchargée avec succès."));
    } else {
        // L'utilisateur a annulé la sélection du fichier
        QMessageBox::information(this, tr("Annulé"), tr("Le téléchargement a été annulé."));
    }
}

void MainWindow::saveImage(const QString &filePath)
{
    QPixmap pixmap = ui->start_label->pixmap(Qt::ReturnByValue);

    // Vérifiez si le QPixmap est valide
    if (!pixmap.isNull()) {
        // Convertissez le QPixmap en QImage
        QImage image = pixmap.toImage();

        if (image.isNull()) {
            QMessageBox::warning(this, tr("Erreur"), tr("Impossible de charger l'image."));
            return;
        }

        QFileInfo fileInfo(filePath);
        QString extension = fileInfo.completeSuffix();

        if (extension.toLower() == "pgm") {
            if (!image.save(filePath, "PGM")) {
                QMessageBox::warning(this, tr("Erreur"), tr("Impossible d'enregistrer l'image."));
            }
        } else if (extension.toLower() == "ppm") {
            if (!image.save(filePath, "PPM")) {
                QMessageBox::warning(this, tr("Erreur"), tr("Impossible d'enregistrer l'image."));
            }
        } else if (extension.toLower() == "png") {
            if (!image.save(filePath, "PNG")) {
                QMessageBox::warning(this, tr("Erreur"), tr("Impossible d'enregistrer l'image."));
            }
        } else if (extension.toLower() == "jpg" || extension.toLower() == "jpeg") {
            if (!image.save(filePath, "JPEG")) {
                QMessageBox::warning(this, tr("Erreur"), tr("Impossible d'enregistrer l'image."));
            }
        } else {
            QMessageBox::warning(this, tr("Erreur"), tr("Format d'image non pris en charge."));
        }
    }
}


void MainWindow::on_submit_button_upload_clicked()
{
    init_filename = QFileDialog::getOpenFileName(this, tr("Sélectionner une image"), "", tr("Fichiers image (*.png *.jpg *.jpeg *.ppm *.pgm)"));
        if (!init_filename.isEmpty()) {
        QPixmap image(init_filename);
        if (!image.isNull()) {
            QString cheminInitPic = "../init_pic.";
            cheminInitPic += QFileInfo(init_filename).suffix();
            extensionImageIn = QFileInfo(init_filename).suffix();

            QFile::remove(cheminInitPic);

            if (QFile::copy(init_filename, cheminInitPic)) {
                qDebug() << "Copie de l'image réussie à : " << cheminInitPic;
                        QPixmap scaledImage = image.scaled(ui->start_label->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
                ui->start_label->setPixmap(scaledImage);
            } else {
                qDebug() << "Échec de la copie de l'image.";
            }
        } else {
            qDebug() << "Impossible de charger l'image";
        }
    } else {
        qDebug() << "Impossible de sélectionner le fichier";
    }
}

void MainWindow::on_noise_algo_selected(int index)
{
    QString selectedAlgo = ui->select_algo_bruit->itemText(index);

    if (selectedAlgo == "GAUSSIEN") {
        ui->label_1_bruit->setText("Moyenne");
        ui->label_2_bruit->show();
        ui->label_2_bruit->setText("Ecart-type");
        ui->select_1_bruit->show();
        ui->select_2_bruit->show();

    } else if (selectedAlgo == "IMPULSIF") {
        ui->label_1_bruit->setText("Facteur");
        ui->label_2_bruit->hide();
        ui->select_1_bruit->show();
        ui->select_2_bruit->hide();
    }
    else if (selectedAlgo == "POISSON") {
        ui->label_1_bruit->setText("Moyenne");
        ui->label_2_bruit->hide();
        ui->select_1_bruit->show();
        ui->select_2_bruit->hide();
    }
    else if (selectedAlgo == "POIVRE SEL") {
        ui->label_1_bruit->setText("Proportion");
        ui->label_2_bruit->hide();
        ui->select_1_bruit->show();
        ui->select_2_bruit->hide();
    }
    else if (selectedAlgo == "SPECKLE") {
        ui->label_1_bruit->setText("Intensité");
        ui->label_2_bruit->hide();
        ui->select_1_bruit->show();
        ui->select_2_bruit->hide();
    }
}


void MainWindow::on_submit_button_noise_clicked()
{
    if (init_filename.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une image de départ !");
        return;
    }

    QString selected_algo = ui->select_algo_bruit->currentText();
    if (selected_algo.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un algorithme de segmentation !");
        return;
    }

    QDir app_folder = QCoreApplication::applicationDirPath();
    QString init_path = app_folder.filePath("../init_pic." + extensionImageIn);
    QString noise_path = app_folder.filePath("../noise_pic." + extensionImageIn);

    if (selected_algo == "GAUSSIEN") {
        float moyenne = ui->select_1_bruit->value();
        float ecartType = ui->select_2_bruit->value();

        Gaussien_Noise((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData(), moyenne, ecartType);
    }
    else if (selected_algo == "IMPULSIF") {
        float facteur = ui->select_1_bruit->value();

        Impulsif((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData(), facteur);
    }
    else if (selected_algo == "POISSON") {
        float moyenne = ui->select_1_bruit->value();

        Poisson((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData(), moyenne);
    }
    else if (selected_algo == "POIVRE SEL") {
        float proportion = ui->select_1_bruit->value();

        PoivreEtSel((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData(), proportion);
    }
    else if (selected_algo == "SPECKLE") {
        float intensite = ui->select_1_bruit->value();

        Speckle((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData(), intensite);
    }

    QPixmap image(noise_path);
    if (!image.isNull()) {
        QPixmap scaledImage = image.scaled(ui->start_label->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
        ui->start_label->setPixmap(scaledImage);
    }
}

void MainWindow::on_denoise_algo_selected(int index)
{
    QString selectedAlgo = ui->select_algo_debruit->itemText(index);

    if (selectedAlgo == "MOYENNEUR" || selectedAlgo == "MEDIAN") {
        ui->label_1_debruit->setText("Nombre voisins");
        ui->label_2_debruit->setText("Intensité");

        ui->label_1_debruit->show();
        ui->label_2_debruit->show();
        ui->label_3_debruit->hide();
        ui->label_4_debruit->hide();

        ui->select_1_debruit->show();
        ui->select_2_debruit->show();
        ui->select_3_debruit->hide();
        ui->select_4_debruit->hide();

    }
    else if (selectedAlgo == "WIENER") {
        ui->label_1_debruit->setText("Nombre voisins");
        ui->label_2_debruit->setText("Variance");
        ui->label_3_debruit->setText("Intensité");

        ui->label_1_debruit->show();
        ui->label_2_debruit->show();
        ui->label_3_debruit->show();
        ui->label_4_debruit->hide();

        ui->select_1_debruit->show();
        ui->select_2_debruit->show();
        ui->select_3_debruit->show();
        ui->select_4_debruit->hide();
    }
    else if (selectedAlgo == "GAUSSIEN") {
        ui->label_1_debruit->setText("Nombre voisins");
        ui->label_2_debruit->setText("Moyenne");
        ui->label_3_debruit->setText("Variance");
        ui->label_4_debruit->setText("Intensité");

        ui->label_1_debruit->show();
        ui->label_2_debruit->show();
        ui->label_3_debruit->show();
        ui->label_4_debruit->show();

        ui->select_1_debruit->show();
        ui->select_2_debruit->show();
        ui->select_3_debruit->show();
        ui->select_4_debruit->show();
    }
    else if (selectedAlgo == "GRADIENT SEUILLEE") {
        ui->label_1_debruit->setText("Nombre voisins");
        ui->label_2_debruit->setText("Moyenne");
        ui->label_3_debruit->setText("Intensité");

        ui->label_1_debruit->show();
        ui->label_2_debruit->show();
        ui->label_3_debruit->show();
        ui->label_4_debruit->hide();

        ui->select_1_debruit->show();
        ui->select_2_debruit->show();
        ui->select_3_debruit->show();
        ui->select_4_debruit->hide();
    }
    else if (selectedAlgo == "MOYENNEUR PONDERE") {
        ui->label_1_debruit->setText("Nombre voisins");
        ui->label_2_debruit->setText("Puissance");
        ui->label_3_debruit->setText("Intensité");

        ui->label_1_debruit->show();
        ui->label_2_debruit->show();
        ui->label_3_debruit->show();
        ui->label_4_debruit->hide();

        ui->select_1_debruit->show();
        ui->select_2_debruit->show();
        ui->select_3_debruit->show();
        ui->select_4_debruit->hide();
    }
    else if(selectedAlgo == "NON LOCAL MEANS") {
        ui->label_1_debruit->setText("Pondération");
        ui->label_2_debruit->setText("Taille fenêtre recherche");
        ui->label_3_debruit->setText("Taille fenêtre");

        ui->label_1_debruit->show();
        ui->label_2_debruit->show();
        ui->label_3_debruit->show();
        ui->label_4_debruit->hide();

        ui->select_1_debruit->show();
        ui->select_2_debruit->show();
        ui->select_3_debruit->show();
        ui->select_4_debruit->hide();
    }
    else if(selectedAlgo == "RESTORMER") {
        ui->label_1_debruit->hide();
        ui->label_2_debruit->hide();
        ui->label_3_debruit->hide();
        ui->label_4_debruit->hide();

        ui->select_1_debruit->hide();
        ui->select_2_debruit->hide();
        ui->select_3_debruit->hide();
        ui->select_4_debruit->hide();
    }
}

void MainWindow::on_submit_button_denoise_clicked()
{
    if (init_filename.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une image de départ !");
        return;
    }

    QString selected_algo = ui->select_algo_debruit->currentText();
    if (selected_algo.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un algorithme de segmentation !");
        return;
    }

    QDir app_folder = QCoreApplication::applicationDirPath();
    QString noise_path = app_folder.filePath("../noise_pic." + extensionImageIn);
    QString denoise_path = app_folder.filePath("../denoise_pic." + extensionImageIn);

    if (selected_algo == "MOYENNEUR") {
        float nbVoisins = ui->select_1_debruit->value();
        float intensite = ui->select_2_debruit->value();

        Moyenneur((char*)noise_path.toUtf8().constData(), (char*)denoise_path.toUtf8().constData(), nbVoisins, intensite);
    }
    else if (selected_algo == "MEDIAN") {
        float nbVoisins = ui->select_1_debruit->value();
        float intensite = ui->select_2_debruit->value();

        Median((char*)noise_path.toUtf8().constData(), (char*)denoise_path.toUtf8().constData(), nbVoisins, intensite);
    }
    else if (selected_algo == "WIENER") {
        float nbVoisins = ui->select_1_debruit->value();
        float variance = ui->select_2_debruit->value();
        float intensite = ui->select_3_debruit->value();

        Wiener((char*)noise_path.toUtf8().constData(), (char*)denoise_path.toUtf8().constData(), nbVoisins, variance, intensite);
    }
    else if (selected_algo == "GAUSSIEN") {
        float nbVoisins = ui->select_1_debruit->value();
        float moyenne = ui->select_2_debruit->value();
        float variance = ui->select_3_debruit->value();
        float intensite = ui->select_4_debruit->value();

        Gaussien_Denoise((char*)noise_path.toUtf8().constData(), (char*)denoise_path.toUtf8().constData(), nbVoisins, moyenne, variance, intensite);
    }
    else if (selected_algo == "GRADIENT SEUILLEE") {
        float nbVoisins = ui->select_1_debruit->value();
        float moyenne = ui->select_2_debruit->value();
        float intensite = ui->select_3_debruit->value();

        Gradient((char*)noise_path.toUtf8().constData(), (char*)denoise_path.toUtf8().constData(), nbVoisins, moyenne, intensite);
    }
    else if (selected_algo == "MOYENNEUR PONDERE") {
        float nbVoisins = ui->select_1_debruit->value();
        float puissance = ui->select_2_debruit->value();
        float intensite = ui->select_3_debruit->value();

        Pondere((char*)noise_path.toUtf8().constData(), (char*)denoise_path.toUtf8().constData(), nbVoisins, puissance, intensite);
    }
    else if(selected_algo == "NON LOCAL MEANS") {
        float ponderation = ui->select_1_debruit->value();
        float tailleFenetreRecherche = ui->select_2_debruit->value();
        float tailleFenetre = ui->select_3_debruit->value();

        NonLocalMeans((char*)noise_path.toUtf8().constData(), (char*)denoise_path.toUtf8().constData(), ponderation, tailleFenetreRecherche, tailleFenetre);
    }
    else if(selected_algo == "RESTORMER") {
        QDir app_folder = QCoreApplication::applicationDirPath();

        QString cheminScript = app_folder.filePath("../QT/preEntraine_test_Restormer.py");
        QString noise_path = app_folder.filePath("../noise_pic." + extensionImageIn);

        QImage input_image(noise_path);
        if (input_image.isNull())
        {
            qDebug() << "Erreur : Impossible de charger l'image d'entrée.";
                return;
        }

        input_image.save("../noise_pic.png", "PNG");

        QString denoise_input_path = app_folder.filePath("../denoise_pic.png");
        QString noise_input_path = app_folder.filePath("../noise_pic.png");

        QProcess *process = new QProcess(this);
        process->setProgram("/net/apps/anaconda3/bin/python3");
        process->setArguments(QStringList() << cheminScript << noise_input_path << denoise_input_path);

        process->start();
        process->waitForFinished(-1);

        delete process;

        QImage image(denoise_input_path);
        QString denoise_path;

        if (extensionImageIn == "ppm") {
            QImage ppmImage = image.convertToFormat(QImage::Format_RGB32);
            ppmImage.save("../denoise_pic.ppm");
            denoise_path = "../denoise_pic.ppm";
        }
        else {
            QImage pgmImage = image.convertToFormat(QImage::Format_Grayscale8);
            pgmImage.save("../denoise_pic.pgm");
            denoise_path = "../denoise_pic.pgm";
        }

        QString cheminNoisePic = "../noise_pic.png";
        QFile::remove(cheminNoisePic);

        QString cheminDenoisePic = "../denoise_pic.png";
        QFile::remove(cheminDenoisePic);
    }

    QPixmap image(denoise_path);
    if (!image.isNull()) {
        QPixmap scaledImage = image.scaled(ui->start_label->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
        ui->start_label->setPixmap(scaledImage);
    }
}

void MainWindow::on_submit_button_algo_clicked() {
    ui->frame_2->raise();
    ui->frame_3->lower();

    ui->algo_button_f2->setStyleSheet("border-image: url(:/stylesheet/calculate_icon_gray.png) 0 0 0 0 stretch stretch;");
}

void MainWindow::on_submit_button_metric_clicked() {
    ui->frame_2->lower();
    ui->frame_3->raise();

    ui->metric_button_f3->setStyleSheet("border-image: url(:/stylesheet/analytics_icon_gray.png) 0 0 0 0 stretch stretch;");

    QDir app_folder = QCoreApplication::applicationDirPath();
    QString init_path = app_folder.filePath("../init_pic." + extensionImageIn);
    QString noise_path = app_folder.filePath("../noise_pic." + extensionImageIn);
    QString denoise_path = app_folder.filePath("../denoise_pic." + extensionImageIn);

    QPixmap init_image(init_path);
    if (!init_image.isNull()) {
        QPixmap noise_pixmap(noise_path);
        QImage noise_image = noise_pixmap.toImage();

        QPixmap denoise_pixmap(denoise_path);
        QImage denoise_image = denoise_pixmap.toImage();

        const QPixmap* currentPixmap = ui->start_label->pixmap();
        QImage currentImage = currentPixmap->toImage();

        float PSNR, SNR, SSIM, RMSE;

        if (!noise_image.isNull()) {

            if (currentImage == noise_image) {
                if (extensionImageIn == "pgm") {
                    PSNR = calculPSNR_G((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData());
                    QString psnrText = "PSNR : " + QString::number(PSNR);
                    ui->label_psnr->setText(psnrText);

                    SNR = calculSNR_G((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData());
                    QString snrText = "SNR : " + QString::number(SNR);
                    ui->label_snr->setText(snrText);

                    SSIM = calculSSIM_G((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData());
                    QString ssimText = "SSIM : " + QString::number(SSIM);
                    ui->label_ssim->setText(ssimText);

                    RMSE = calculRMSE_G((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData());
                    QString rmseText = "RMSE : " + QString::number(RMSE);
                    ui->label_rmse->setText(rmseText);
                }
                else if (extensionImageIn == "ppm") {
                    PSNR = calculPSNR_RGB((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData());
                    QString psnrText = "PSNR : " + QString::number(PSNR);
                    ui->label_psnr->setText(psnrText);

                    SNR = calculSNR_RGB((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData());
                    QString snrText = "SNR : " + QString::number(SNR);
                    ui->label_snr->setText(snrText);

                    SSIM = calculSSIM_RGB((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData());
                    QString ssimText = "SSIM : " + QString::number(SSIM);
                    ui->label_ssim->setText(ssimText);

                    RMSE = calculRMSE_RGB((char*)init_path.toUtf8().constData(), (char*)noise_path.toUtf8().constData());
                    QString rmseText = "RMSE : " + QString::number(RMSE);
                    ui->label_rmse->setText(rmseText);
                }
            }
        }
        else if (!denoise_image.isNull()) {

        }

    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QString cheminInitPic = "../init_pic." + extensionImageIn;
    QFile::remove(cheminInitPic);

    QString cheminNoisePic = "../noise_pic." + extensionImageIn;
    QFile::remove(cheminNoisePic);

    QString cheminDenoisePic = "../denoise_pic." + extensionImageIn;
    QFile::remove(cheminDenoisePic);

    QMainWindow::closeEvent(event);
}
