/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2012  Christian Urich

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "importrasterdata.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <dm.h>
#include <dmview.h>
#include <dmcomponent.h>
#include <QFileDialog>
#include "importrasterdata_gui.h"
#include <QFileDialog>


//DM_DECLARE_NODE_NAME(ImportRasterData, Modules)
DM_DECLARE_CUSTOM_NODE_NAME(ImportRasterData,"Import Land Cover Map", "Scenario Definition")

ImportRasterData::ImportRasterData()
{
    multiplier = 1;
    flip = true;
    FileName = "";
    dataname = "Imp";
    dataname_old = "";
    useMCD = "false";
    MCDFilename = "";
    rows = "";
    cols = "";
    cellsize= "";
    xoffset= "0";
    yoffset= "0";

    this->addParameter("Filename", DM::FILENAME, &FileName);
    this->addParameter("DataName", DM::STRING, &dataname);
    this->addParameter("Multiplier", DM::DOUBLE, &multiplier);
    this->addParameter("Flip",DM::BOOL, &flip);
    this->addParameter("useMCD",DM::STRING,&useMCD);
    this->addParameter("rows", DM::STRING,&rows);
    this->addParameter("cols", DM::STRING,&cols);
    this->addParameter("cellsize", DM::STRING,&cellsize);
    this->addParameter("xoffset", DM::STRING,&xoffset);
    this->addParameter("yoffset", DM::STRING,&yoffset);
    this->addParameter("MCDFilename",DM::STRING,&MCDFilename);

}

void ImportRasterData::init()
{
    if (dataname.empty())
        return;
    if (dataname.compare(dataname_old) == 0)
        return;
    DM::View data(dataname, DM::RASTERDATA, DM::WRITE);

    std::vector<DM::View> vdata;
    vdata.push_back(data);
    dataname_old = dataname;

    Coords = DM::View("CoordOffset",DM::COMPONENT, DM::WRITE);
    Coords.addAttribute("Xoffset");
    Coords.addAttribute("Yoffset");
    MCD = DM::View("MCDs",DM::COMPONENT,DM::WRITE);
    MCD.addAttribute("useMCD");
    MCD.addAttribute("MCDFilename");
    vdata.push_back(Coords);
    vdata.push_back(MCD);

    this->addData("Data", vdata);
}

bool ImportRasterData::createInputDialog()
{
    QWidget * w = new ImportRasterData_Gui(this);
    w->show();
    return true;
}

string ImportRasterData::getFilename()
{
    return this->FileName;
}

string ImportRasterData::getMCDFilename()
{
    return this->MCDFilename;
}
std::string ImportRasterData::getrows()
{
    return this->rows;
}

std::string ImportRasterData::getcols()
{
    return this->cols;
}

std::string ImportRasterData::getcellsize()
{
    return this->cellsize;
}

std::string ImportRasterData::getxoffset()
{
    return this->xoffset;
}

std::string ImportRasterData::getyoffset()
{
    return this->yoffset;
}

ImportRasterData::~ImportRasterData()
{
//    if(QFile::exists(QString(QDir::currentPath() + QString("/impfile.txt"))))
//        QFile::remove(QString(QDir::currentPath() + QString("/impfile.txt")));
}


void ImportRasterData::run()
{
    workingDir = this->getWorkPath().toStdString();
    DM::View data(dataname, DM::RASTERDATA, DM::WRITE);
    DM::RasterData * r = this->getRasterData("Data", data);
    QString p=QString(workingDir.c_str())+"/"+QString::fromStdString(FileName);
    QFile file(p);

    DM::System * sys = this->getData("Data");

    DM::Component * cmp = new DM::Component();
    sys->addComponent(cmp,Coords);
    DM::Component * cmp2 = new DM::Component();
    cmp2->addAttribute("useMCD",this->useMCD);
    cmp2->addAttribute("MCDFilename",this->MCDFilename);
    sys->addComponent(cmp2,MCD);
    std::cout << this->useMCD << std::endl;
    if(this->useMCD!="true")
    {
        QTextStream stream(&file);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
             DM::Logger(DM::Error) << "warning, read input file ";
             return;
        }

        QString line("NULL");

        int LineCounter  = 0;
        int rowCounter = 0;
        int ncols = 0;
        int nrows = 0;
        double xoffset = 0;
        double yoffset = 0;
        double cellsize = 0;
        double NoDataValue = -9999; //default

        //Read Header
        while (!line.isNull() && LineCounter < 6 ) {
            LineCounter++;
            line =stream.readLine();
            if (LineCounter == 1) {
                QStringList list = line.split(QRegExp("\\s+"));
                QString s = QString(list[1]);
                s.replace(",", ".");
                ncols = s.toInt();
            }
            if (LineCounter == 2) {
                QStringList list = line.split(QRegExp("\\s+"));
                QString s = QString(list[1]);
                s.replace(",", ".");
                nrows = s.toInt();
            }
            if (LineCounter == 3) {
                QStringList list = line.split(QRegExp("\\s+"));
                QString s = QString(list[1]);
                s.replace(",", ".");
                xoffset = s.toDouble();
                cmp->addAttribute("Xoffset",xoffset);
            }
            if (LineCounter == 4) {
                QStringList list = line.split(QRegExp("\\s+"));
                QString s = QString(list[1]);
                s.replace(",", ".");
                yoffset = s.toDouble();
                cmp->addAttribute("Yoffset",yoffset);
            }
            if (LineCounter == 5) {
                QStringList list = line.split(QRegExp("\\s+"));
                QString s = QString(list[1]);
                s.replace(",", ".");
                cellsize = s.toDouble() * multiplier;
            }
            if (LineCounter == 6) {
                QStringList list = line.split(QRegExp("\\s+"));
                QString s = QString(list[1]);
                s.replace(",", ".");
                NoDataValue = s.toDouble();
            }
        }
        std::cout <<" Cols " << ncols << std::endl;
        std::cout <<" Rows " << nrows << std::endl;
        std::cout <<" Cellsize " << cellsize << std::endl;
        r->setNoValue(NoDataValue);
        r->setSize(ncols, nrows, cellsize,cellsize,xoffset,yoffset);
        while (!line.isNull()) {
            LineCounter++;
            line =stream.readLine();
            if (LineCounter >= 6 && rowCounter < nrows) {
                QStringList list = line.split(QRegExp("\\s+"));
                for ( int i = 0; i < list.size(); i++ ) {
                    QString s = QString(list[i]);
                    s.replace(",", ".");
                    if (flip)
                        r->setCell(i, nrows-rowCounter-1, s.toDouble());
                    else
                        r->setCell(i, rowCounter, s.toDouble());
                }
                rowCounter++;

            }
        }
        file.close();
    }
    else{
        r->setNoValue(-9999);
        r->setSize(atof(cols.c_str()), atof(rows.c_str()), atof(cellsize.c_str()),atof(cellsize.c_str()),atof(xoffset.c_str()),atof(yoffset.c_str()));
        r->setCell(0,0,0);
    }
}
