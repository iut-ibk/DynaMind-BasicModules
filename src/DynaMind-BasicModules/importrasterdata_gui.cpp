#include "importrasterdata_gui.h"
#include "ui_importrasterdata_gui.h"
#include "importrasterdata.h"
#include <QFileDialog>
#include <QTextStream>
#include <QSettings>



ImportRasterData_Gui::ImportRasterData_Gui(ImportRasterData * ird, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportRasterData_Gui)
{
    ui->setupUi(this);
    this->ird = ird;
    ui->le_Filename->setText(this->ird->getFilename().c_str());
}

ImportRasterData_Gui::~ImportRasterData_Gui()
{
    delete ui;
}

void ImportRasterData_Gui::on_pb_load_clicked()
{
    QSettings settings;
    QString workdir = QString(settings.value("workPath").toString());
    QString fname = QFileDialog::getOpenFileName(this,"Map jpeg",workdir,"*.txt");
    if (fname == "")
        return;
    ui->le_Filename->setText(fname);
    this->ird->setParameterValue("Filename",fname.toStdString());
}

void ImportRasterData_Gui::on_bBox_accepted()
{
    QSettings settings;
    QString workdir = QString(settings.value("workPath").toString());
    QFile tmpfile(workdir+"/impfile.txt");
     if(tmpfile.open(QIODevice::WriteOnly | QIODevice::Text))
     {
         QTextStream outstream (&tmpfile);
         outstream << ui->le_Filename->text() << endl;
     }
     tmpfile.close();
}

