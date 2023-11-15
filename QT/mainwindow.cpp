#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "our_functions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap logoPixmap(":/stylesheet/logo.png");
    logoPixmap = logoPixmap.scaled(logoPixmap.width() * 2, logoPixmap.height() * 2);
    ui->logoLabel->setPixmap(logoPixmap);
    ui->logoLabel->setAlignment(Qt::AlignCenter);

    ui->fond_image_1->setStyleSheet("background-color: #071247; border-radius: 30px;");
    ui->fond_image_2->setStyleSheet("background-color: #071247; border-radius: 30px;");
    ui->fond_image_3->setStyleSheet("background-color: #071247; border-radius: 30px;");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*--------------------- ALGORITHMES POUR QT ---------------------*/

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
}



void MainWindow::on_load_button_clicked()
{
    init_filename = QFileDialog::getOpenFileName(this, tr("Sélectionner une image"), "", tr("Fichiers image (*.png *.jpg *.jpeg *.ppm)"));
    if (!init_filename.isEmpty()) {
        QPixmap image(init_filename);
        if (!image.isNull()) {
            QPixmap scaledImage = image.scaled(ui->start_label->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
            ui->start_label->setPixmap(scaledImage);
        }
        else
            qDebug() << "Impossible de charger l'image";
    } else {
        qDebug() << "Impossible de sélectionner le fichier";
    }
}

void MainWindow::on_submit_button_clicked()
{
    if (init_filename.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une image de départ !");
        return;
    }

    QString selected_algo = ui->select_algo->currentText();
    if (selected_algo.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un algorithme de segmentation !");
        return;
    }

    int KX = ui->select_KX->value();
    int KY = ui->select_KY->value();
    int M = ui->select_M->value();
    int SEUIL = ui->select_SEUIL->value();
    if (KX == 0 || KY == 0 || M == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner des valeurs valides (supérieures à 0) !");
        return;
    }

    QDir app_folder = QCoreApplication::applicationDirPath();
    QString init_path = app_folder.filePath("../init_pic.ppm");
    QString seg_path = app_folder.filePath("../seg_pic.ppm");
    QString comp_path = app_folder.filePath("../comp_pic.ppm");

    convertToPPM(init_filename, init_path);

    if (selected_algo == "SLIC") {
        compute_SLIC((char*)"init_pic.ppm", (char*)"seg_pic.ppm", KX, KY, SEUIL, M, "");
    }
    if (selected_algo == "SNIC") {
        compute_SNIC((char*)"init_pic.ppm", (char*)"seg_pic.ppm", KX, KY, M, "");
    }

    QPixmap image(seg_path);
    if (!image.isNull()) {
        QPixmap scaledImage = image.scaled(ui->start_label->size(), Qt::KeepAspectRatio, Qt::FastTransformation);
        ui->seg_label->setPixmap(scaledImage);
    }

}

void MainWindow::on_download_comp_button_clicked()
{
    QString selected_algo = ui->select_comp->currentText();
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
}

void MainWindow::on_download_button_clicked()
{
    QString selected_format = ui->select_format->currentText();

    QDir app_folder = QCoreApplication::applicationDirPath();
    QString res_path = app_folder.filePath("../resultat.ppm");
    QString fileName;

    if (selected_format == "PPM") {
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
    }

    downloadAs(res_path, fileName, selected_format);
}

void MainWindow::on_compress_button_clicked()
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

    int KX = ui->select_KX->value();
    int KY = ui->select_KY->value();
    int M = ui->select_M->value();
    int SEUIL = ui->select_SEUIL->value();
    if (KX == 0 || KY == 0 || M == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner des valeurs valides (supérieures à 0) !");
        return;
    }

    QFile img_base(app_folder.filePath("../init_pic.ppm"));
    float size_before = img_base.size();
    float size_after = 1;

    QString selected_compress = ui->select_comp->currentText();
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
        }
        QFile img_index1(app_folder.filePath("../index.pgm"));
        QFile img_palette(app_folder.filePath("../palette.ppm"));
        size_after = img_index1.size() + img_palette.size();
    }
    if (selected_compress == "Palette couleur (65536)")
    {
        if ((KX * KY) > 65536) {
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
        if (selected_algo == "SLIC") {
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
}

void MainWindow::on_select_pal_clicked()
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
}

void MainWindow::on_decomp_button_clicked() {
    QDir app_folder = QCoreApplication::applicationDirPath();
    QString decomp_selected = ui->select_comp_2->currentText();
    if (decomp_selected == "Palette couleur (256)") {
        if (pal_filepath.isEmpty() || index_filepath.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez passez l'ensemble des fichiers nécessaires");
            return;
        }
        else {
            copieFichier(pal_filepath, "palette.ppm");
            copieFichier(index_filepath, "index.pgm");
            decompress_256((char*) "index.pgm", (char*) "palette.ppm", (char*) "resultat.ppm");
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
            decompress_65536((char*) "index1.pgm", (char*) "index2.pgm", (char*) "palette.ppm", (char*) "resultat.ppm");
        }
    }
    if (decomp_selected == "Fichier texte") {
        if (txt_filepath.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez passez l'ensemble des fichiers nécessaires");
            return;
        }
        else {
            copieFichier(txt_filepath, "text.txt");
            decompress_txt((char*) "text.txt", (char*) "resultat.ppm");
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

}
