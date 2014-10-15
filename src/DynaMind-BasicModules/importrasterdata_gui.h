#ifndef IMPORTRASTERDATA_GUI_H
#define IMPORTRASTERDATA_GUI_H

#include <QDialog>
#include "importrasterdata.h"

namespace Ui {
class ImportRasterData_Gui;
}

class ImportRasterData_Gui : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImportRasterData_Gui(ImportRasterData *ird,QWidget *parent = 0);
    ~ImportRasterData_Gui();
private slots:
    void on_pb_load_clicked();
    void on_bBox_accepted();
    void writeDimensions();
    void on_pb_load_2_released();

private:
    Ui::ImportRasterData_Gui *ui;
    ImportRasterData *ird;

};

#endif // IMPORTRASTERDATA_GUI_H
