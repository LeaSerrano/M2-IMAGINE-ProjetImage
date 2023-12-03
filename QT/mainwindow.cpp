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



    //QPixmap logoPixmap(":/stylesheet/logo.png");
    //logoPixmap = logoPixmap.scaled(logoPixmap.width() * 2, logoPixmap.height() * 2);
    //ui->logoLabel->setPixmap(logoPixmap);
    //ui->logoLabel->setAlignment(Qt::AlignCenter);

    //ui->logoLabel->setText("DENOISEPRO");
    //ui->logoLabel->setStyleSheet("color: #704257; font-size: 20px;");
    //ui->logoLabel->setAlignment(Qt::AlignCenter);

    ui->fond_image_1->setStyleSheet("background-color: #3d3e42;");

    ui->frame_1->setObjectName("styledFrame");
    ui->frame_2->setObjectName("styledFrame");

    ui->frame_21->setStyleSheet("color : #3d3e42;");
    ui->frame_23->setStyleSheet("color : #3d3e42;");
    ui->frame_22->setStyleSheet("color : #202124;");

    ui->download_button->setObjectName("downloadButton");
    ui->upload_button->setObjectName("uploadButton");
    ui->algo_button->setObjectName("algoButton");

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

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString extensionImageIn;

/*--------------------- ALGORITHMES POUR QT ---------------------*/

/*
void copieFichier(const QString filepath, const QString newName)
{
    QFile sourceFile(filepath);
    QDir appFolder = QCoreApplication::applicationDirPath();
    sourceFile.copy(appFolder.filePath("../"+newName));
}

void downloadFile(const QString inputFilePath, const QString outputFilePath)
{
    //QFile sourceFile(inputFilePath);
    //sourceFile.copy(outputFilePath);
    QFile::copy(inputFilePath, outputFilePath);
}

void getData(QString to_data, char* char_tab)
{
    QByteArray tempData = "Hello World";
    QByteArray ba = to_data.toUtf8();
    char_tab = ba.data();
}

void convertToPPM(const QString& inputFilePath, const QString& outputFilePath)
{
    QImage image;
    image.load(inputFilePath);
    image.save(outputFilePath, "PPM");
    qDebug() << outputFilePath;
}

void downloadAs(const QString& inputFilePath, const QString& outputFilePath, const QString& format)
{
    QImage image;
    image.load(inputFilePath);

    if (format == "PPM")
        image.save(outputFilePath, "PPM");
    if (format == "PNG")
        image.save(outputFilePath, "PNG");
    if (format == "JPG")
        image.save(outputFilePath, "JPG");
}*/

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

/*void MainWindow::on_download_comp_button_clicked()
{
    /*QString selected_algo = ui->select_comp->currentText();
    QString selectedDir = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir app_folder = QCoreApplication::applicationDirPath();
    if (selected_algo == "Palette couleur (256)")
    {
        downloadFile(app_folder.filePath("../index.pgm"), selectedDir+"/index_1.pgm");
        downloadFile(app_folder.filePath("../palette.ppm"), selectedDir+"/ma_palette.ppm");
    }
    if (selected_algo == "Palette couleur (65536)")
    {
        downloadFile(app_folder.filePath("../index.pgm"), selectedDir+"/index_1.pgm");
        downloadFile(app_folder.filePath("../indexbis.pgm"), selectedDir+"/index_2.pgm");
        downloadFile(app_folder.filePath("../palette.ppm"), selectedDir+"/ma_palette.ppm");
    }
    if (selected_algo == "Fichier texte")
    {
        downloadFile(app_folder.filePath("../comp_res.txt"), selectedDir+"/txt_compresse.txt");
    }
}*/

/*void MainWindow::on_submit_button_download_clicked()
{
    //QString selected_format = ui->select_format2->currentText();

    QDir app_folder = QCoreApplication::applicationDirPath();
    QString res_path = app_folder.filePath("../resultat.ppm");
    QString fileName;

    /*if (selected_format == "PPM") {
        fileName = QFileDialog::getSaveFileName(this, tr("Sauvegarder l'image"),
                                                QDir::homePath(),
                                                tr("Images (*.ppm)"));
    }
    if (selected_format == "PNG") {
        fileName = QFileDialog::getSaveFileName(this, tr("Sauvegarder l'image"),
                                                QDir::homePath(),
                                                tr("Images (*.png)"));
    }
    if (selected_format == "JPG") {
        fileName = QFileDialog::getSaveFileName(this, tr("Sauvegarder l'image"),
                                                QDir::homePath(),
                                                tr("Images (*.jpg)"));
    }*/

    /*fileName = QFileDialog::getSaveFileName(this, tr("Sauvegarder l'image"),
                                            QDir::homePath(),
                                            tr("Images (*.jpg, png, ppm, pgm)"));

    QString format = fileName.right(3);

    downloadAs(res_path, fileName, format);
}*/

/*void MainWindow::on_compress_button_clicked()
{
    QDir app_folder = QCoreApplication::applicationDirPath();
    if (init_filename.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une image de départ !");
        return;
    }

    QString selected_algo = ui->select_algo->currentText();
    if (selected_algo.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un algorithme de segmentation !");
        return;
    }

    /*int KX = ui->select_KX->value();
    int KY = ui->select_KY->value();
    int M = ui->select_M->value();
    int SEUIL = ui->select_SEUIL->value();
    if (KX == 0 || KY == 0 || M == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner des valeurs valides (supérieures à 0) !");
        return;
    }*/

    /*QFile img_base(app_folder.filePath("../init_pic.ppm"));
    float size_before = img_base.size();
    float size_after = 1;

    /*QString selected_compress = ui->select_comp->currentText();
    if (selected_compress == "Palette couleur (256)")
    {
        if ((KX * KY) > 256) {
            QMessageBox::warning(this, "Erreur", "Utilisation de cet algorithme de compression impossible avec les valeurs passées actuelles !");
            return;
        }

        if (selected_algo == "SLIC") {
            compute_SLIC((char*)"init_pic.ppm", (char*)"seg_pic.ppm", KX, KY, SEUIL, M, "256");
        }
        if (selected_algo == "SNIC") {
            compute_SNIC((char*)"init_pic.ppm", (char*)"seg_pic.ppm", KX, KY, M, "256");
        }QFile img_index1(app_folder.filePath("../index.pgm"));
        QFile img_palette(app_folder.filePath("../palette.ppm"));
        size_after = img_index1.size() + img_palette.size();
    }
    if (selected_compress == "Palette couleur (65536)")
    {
        /*if ((KX * KY) > 65536) {
            QMessageBox::warning(this, "Erreur", "Utilisation de cet algorithme de compression impossible avec les valeurs passées actuelles !");
            return;
        }

        if (selected_algo == "SLIC") {
            compute_SLIC((char*)"init_pic.ppm", (char*)"seg_pic.ppm", KX, KY, SEUIL, M, "65536");
        }
        if (selected_algo == "SNIC") {
            compute_SNIC((char*)"init_pic.ppm", (char*)"seg_pic.ppm", KX, KY, M, "65536");
        }

        QFile img_index1(app_folder.filePath("../index.pgm"));
        QFile img_index2(app_folder.filePath("../indexbis.pgm"));
        QFile img_palette(app_folder.filePath("../palette.ppm"));
        size_after = img_index1.size() + img_index2.size() + img_palette.size();
    }
    if (selected_compress == "Fichier texte")
    {
        /*if (selected_algo == "SLIC") {
            compute_SLIC((char*)"init_pic.ppm", (char*)"seg_pic.ppm", KX, KY, SEUIL, M, "txt");
        }
        if (selected_algo == "SNIC") {
            compute_SNIC((char*)"init_pic.ppm", (char*)"seg_pic.ppm", KX, KY, M, "txt");
        }

        QFile txt_file(app_folder.filePath("../comp_res.txt"));
        size_after = txt_file.size();
    }

    qDebug() << "Before : " << size_before;
    qDebug() << "After : " << size_after;
    float size_ratio = size_before / size_after;
    ui->taux_label->setText("Taux de compression : "+QString::number(size_ratio));
}*/

/*void MainWindow::on_select_pal_clicked()
{
    pal_filepath = QFileDialog::getOpenFileName(this, tr("Sélectionner la palette de couleurs"), "", tr("Fichiers image (*.ppm)"));
    QFileInfo fileInfo(pal_filepath);
    ui->pal_path->setText(fileInfo.fileName());
}

void MainWindow::on_select_index_clicked()
{
    index_filepath = QFileDialog::getOpenFileName(this, tr("Sélectionner l'image des index"), "", tr("Fichiers image (*.pgm)"));
    QFileInfo fileInfo(index_filepath);
    ui->index_path->setText(fileInfo.fileName());
}

void MainWindow::on_select_indexbis_clicked()
{
    indexbis_filepath = QFileDialog::getOpenFileName(this, tr("Sélectionner l'image des index (bis)"), "", tr("Fichiers image (*.pgm)"));
    QFileInfo fileInfo(indexbis_filepath);
    ui->indexbis_path->setText(fileInfo.fileName());
}

void MainWindow::on_select_txt_clicked()
{
    txt_filepath = QFileDialog::getOpenFileName(this, tr("Sélectionner le fichier txt compressé"), "", tr("Fichiers texte (*.txt)"));
    QFileInfo fileInfo(txt_filepath);
    ui->text_path->setText(fileInfo.fileName());
}*/

/*void MainWindow::on_decomp_button_clicked() {
    QDir app_folder = QCoreApplication::applicationDirPath();
    QString decomp_selected = ui->select_algo_debruit->currentText();
    if (decomp_selected == "Palette couleur (256)") {
        if (pal_filepath.isEmpty() || index_filepath.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez passez l'ensemble des fichiers nécessaires");
            return;
        }
        else {
            copieFichier(pal_filepath, "palette.ppm");
            copieFichier(index_filepath, "index.pgm");
            //decompress_256((char*) "index.pgm", (char*) "palette.ppm", (char*) "resultat.ppm");
        }
    }
    if (decomp_selected == "Palette couleur (65536)") {
        if (pal_filepath.isEmpty() || index_filepath.isEmpty() || indexbis_filepath.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez passez l'ensemble des fichiers nécessaires");
            return;
        }
        else {
            copieFichier(pal_filepath, "palette.ppm");
            copieFichier(index_filepath, "index1.pgm");
            copieFichier(indexbis_filepath, "index2.pgm");
            //decompress_65536((char*) "index1.pgm", (char*) "index2.pgm", (char*) "palette.ppm", (char*) "resultat.ppm");
        }
    }
    if (decomp_selected == "Fichier texte") {
        if (txt_filepath.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez passez l'ensemble des fichiers nécessaires");
            return;
        }
        else {
            copieFichier(txt_filepath, "text.txt");
            //decompress_txt((char*) "text.txt", (char*) "resultat.ppm");
        }
    }

    QString res_path = app_folder.filePath("../resultat.ppm");
    QPixmap image(res_path);
    if (!image.isNull()) {
        QPixmap scaledImage = image.scaled(ui->decomp_label->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
        ui->decomp_label->setPixmap(scaledImage);
    }
    //float PSNR = compute_PSNR((char*) "init_pic.ppm", (char*) "seg_pic.ppm");
    //ui->PSNR_label->setText("PSNR : "+QString::number(PSNR));

}*/

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
