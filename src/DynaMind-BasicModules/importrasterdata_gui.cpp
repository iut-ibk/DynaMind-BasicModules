#include "importrasterdata_gui.h"
#include "ui_importrasterdata_gui.h"
#include "importrasterdata.h"
#include <QFileDialog>
#include <QTextStream>
#include <QSettings>
#include <QMessageBox>



ImportRasterData_Gui::ImportRasterData_Gui(ImportRasterData * ird, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportRasterData_Gui)
{
    ui->setupUi(this);
    this->ird = ird;
    ui->le_Filename->setText(this->ird->getFilename().c_str());
    ui->le_Filename_2->setText(this->ird->getMCDFilename().c_str());
    ui->le_cellsize->setText(this->ird->getcellsize().c_str());
    ui->le_cols->setText(this->ird->getcols().c_str());
    ui->le_rows->setText(this->ird->getrows().c_str());
    this->ird->workingDir = this->ird->getWorkPath().toStdString();
}

ImportRasterData_Gui::~ImportRasterData_Gui()
{
    delete ui;
}

void ImportRasterData_Gui::on_pb_load_clicked()
{
    QSettings settings;
    QString workdir = this->ird->getWorkPath();
    QString datadir = QString(settings.value("dataPath").toString());
    QString fname = QFileDialog::getOpenFileName(this,"Map jpeg",datadir,"*.txt");
    if (fname == "")
        return;
    QFile file(fname);
    if (!file.exists())
    {
        QMessageBox::warning(NULL,"Error",QString("Could not open file %1").arg(fname));
        return;
    }
    QString bfname=QFileInfo(file).fileName();
    QString fi=workdir+"/"+bfname;
    QFileInfo workfinfo = QFileInfo(fi);
    QFileInfo finfo = QFileInfo(fi);
    if(finfo.absolutePath() != workfinfo.absolutePath())
    {
        if(QFile::exists(fi))
            QFile::remove(fi);

        if (!file.copy(fi))
        {
            QMessageBox::warning(NULL,"Error",QString("Could not copy file %1 to %2").arg(fname).arg(fi));
            return;
        }

    }
    QFile::copy(fname,fi);
    ui->le_Filename->setText(bfname);
    this->ird->setParameterValue("Filename",bfname.toStdString());
    this->ird->setParameterValue("useMCD","false");
    settings.setValue("dataPath",finfo.absolutePath());
    if(QFile::exists(this->ird->getWorkPath() + "/WSUDtech.mcd"));
        QFile::remove(this->ird->getWorkPath() +"/WSUDtech.mcd");
}

void ImportRasterData_Gui::on_pb_load_2_released()
{
    QSettings settings;
    QString workdir = this->ird->getWorkPath();
    QString datadir = QString(settings.value("dataPath").toString());
    QString fname = QFileDialog::getOpenFileName(this,"Map jpeg",datadir,"*.mcd");
    if (fname == "")
        return;
    QFile file(fname);
    if (!file.exists())
    {
        QMessageBox::warning(NULL,"Error",QString("Could not open file %1").arg(fname));
        return;
    }
    QString bfname=QFileInfo(file).fileName();
    QString fi=workdir+"/"+bfname;
    QFileInfo workfinfo = QFileInfo(fi);
    QFileInfo finfo = QFileInfo(fi);
    if(finfo.absolutePath() != workfinfo.absolutePath())
    {
        if(QFile::exists(fi))
            QFile::remove(fi);

        if (!file.copy(fi))
        {
            QMessageBox::warning(NULL,"Error",QString("Could not copy file %1 to %2").arg(fname).arg(fi));
            return;
        }

    }

    QFile::copy(fname,fi);
    ui->le_Filename_2->setText(bfname);
    this->ird->setParameterValue("MCDFilename",bfname.toStdString());
    this->ird->setParameterValue("useMCD","true");
    settings.setValue("dataPath",finfo.absolutePath());
    if(QFile::exists(this->ird->getWorkPath() + "/WSUDtech.mcd"));
        QFile::remove(this->ird->getWorkPath() +"/WSUDtech.mcd");
}

void ImportRasterData_Gui::on_bBox_accepted()
{
    QString workdir = this->ird->getWorkPath();
    this->ird->workingDir = workdir.toStdString();

    QFile tmpfile(workdir+"/impfile.txt");
    if(tmpfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream outstream (&tmpfile);
        if(ui->le_Filename->text() == "")
        {
            outstream << "dimensions.txt" << endl;
            writeDimensions();
        }
        else
        {
            outstream << ui->le_Filename->text() << endl;
        }
    }
    tmpfile.close();
    this->ird->setParameterValue("rows",this->ui->le_rows->text().toStdString());
    this->ird->setParameterValue("cols",this->ui->le_cols->text().toStdString());
    this->ird->setParameterValue("cellsize",this->ui->le_cellsize->text().toStdString());

}

void ImportRasterData_Gui::writeDimensions()
{
    QString workdir = this->ird->getWorkPath();
    QFile dimfile(workdir+"/dimensions.txt");
    if(dimfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream outstream (&dimfile);

            outstream << "ncols " << ui->le_cols->text() << endl;
            outstream << "nrows " << ui->le_rows->text() << endl;
            outstream << "xllcorner 0" << endl;
            outstream << "yllcorner 0" << endl;
            outstream << "cellsize " << ui->le_cellsize->text() << endl;
    }
    dimfile.close();
}



