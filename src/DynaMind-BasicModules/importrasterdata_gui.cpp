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
    QSettings settings;
    this->ird->workingDir = settings.value("workPath").toString().toStdString();
}

ImportRasterData_Gui::~ImportRasterData_Gui()
{
    delete ui;
}

void ImportRasterData_Gui::on_pb_load_clicked()
{
    QSettings settings;
    QString workdir = QString(settings.value("workPath").toString());
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
    settings.setValue("dataPath",finfo.absolutePath());
}

void ImportRasterData_Gui::on_bBox_accepted()
{
    QSettings settings;
    QString workdir = QString(settings.value("workPath").toString());
    this->ird->workingDir = settings.value("workPath").toString().toStdString();

    QFile tmpfile(workdir+"/impfile.txt");
    if(tmpfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream outstream (&tmpfile);
        outstream << ui->le_Filename->text() << endl;
    }
    tmpfile.close();
}

