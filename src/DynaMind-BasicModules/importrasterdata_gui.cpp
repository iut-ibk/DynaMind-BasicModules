#include "importrasterdata_gui.h"
#include "ui_importrasterdata_gui.h"
#include "importrasterdata.h"
#include <QFileDialog>
#include <QTextStream>



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
    QString fname = QFileDialog::getOpenFileName(this,"Map jpeg",QDir::currentPath(),"*.txt");
    if (fname == "")
        return;
    ui->le_Filename->setText(fname);
    this->ird->setParameterValue("Filename",fname.toStdString());
}

void ImportRasterData_Gui::on_bBox_accepted()
{
        QFile tmpfile(QDir::currentPath()+"/impfile.txt");
         if(tmpfile.open(QIODevice::WriteOnly | QIODevice::Text))
         {
             QTextStream outstream (&tmpfile);
             outstream << ui->le_Filename->text() << endl;
         }
         tmpfile.close();
}

