#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected :
    void closeEvent(QCloseEvent *event) override;

public slots:
    void on_submit_button_upload_clicked();
    void on_submit_button_noise_clicked();
    void on_submit_button_denoise_clicked();
    //void on_download_comp_button_clicked();
    void on_submit_button_download_clicked();
    //void on_compress_button_clicked();
    /*void on_select_pal_clicked();
    void on_select_index_clicked();
    void on_select_indexbis_clicked();
    void on_select_txt_clicked();*/
    //void on_decomp_button_clicked();
    void on_noise_algo_selected(int index);
    void on_denoise_algo_selected(int index);

private:
    Ui::MainWindow *ui;
    QString init_filename;
    QString seg_filename;
    QString comp_filename;
    QString pal_filepath;
    QString index_filepath;
    QString indexbis_filepath;
    QString txt_filepath;
};

#endif // MAINWINDOW_H
