#include "exportrasterdata.h"
#include "guiexportrasterdata.h"
#include <sstream>
#include <fstream>

DM_DECLARE_NODE_NAME(ExportRasterData, Modules)

ExportRasterData::ExportRasterData()
{
	sys_in = 0;
	this->NameOfExistingView = "";

	data.push_back(  DM::View ("dummy", DM::SUBSYSTEM, DM::READ) );
	this->FileName = "";
	this->flip_h = false;
	this->add_run_number = true;
	this->addParameter("FileName", DM::FILENAME, &this->FileName);
	this->addParameter("flip_h", DM::BOOL, &this->flip_h);
	this->addParameter("add_run_number", DM::BOOL, &this->add_run_number);
	this->addParameter("NameOfExistingView", DM::STRING, &this->NameOfExistingView);
	this->addData("Data", data);
	counter = 0;
}

void ExportRasterData::run () {
	counter++;
	DM::View(this->NameOfExistingView, DM::RASTERDATA, DM::READ);
	DM::RasterData * rData = this->getRasterData("Data",DM::View(this->NameOfExistingView, DM::RASTERDATA, DM::READ));

	if (!rData)
	{
		DM::Logger(DM::Error) << "Error, no rasterdata found";
		return;
	}

	QString extension=".txt";
	std::stringstream s;
	s << "_"<<counter;
	QString fullFileName;
	if (add_run_number)
		 fullFileName =   QString::fromStdString(FileName)+  QString::fromStdString(s.str()) +extension;
	else {
		fullFileName =   QString::fromStdString(FileName) + extension;
	}
	std::fstream txtout;

	txtout.open(fullFileName.toLatin1(),std::ios::out);

	//header for ARCGIS import

	txtout<<"ncols "<< rData->getWidth() <<"\n";
	txtout<<"nrows "<< rData->getHeight() <<"\n";
	txtout<<"xllcorner " << rData->getXOffset() <<"\n";
	txtout<<"yllcorner " << rData->getYOffset() <<"\n";
	txtout<<"cellsize "<<rData->getCellSizeX()<<"\n";
	txtout<<"nodata_value "<< rData->getNoValue()<<"\n";


	if (flip_h) {
		for (unsigned int j=0; j<rData->getHeight() ; j++)
		{
			for (unsigned int i=0; i<rData->getWidth(); i++)
			{
				txtout<<rData->getCell(i,j)<< " ";
			}
			txtout<<"\n";

		}
	}
	if (!flip_h) {
		for (int j=rData->getHeight() -1 ; j > -1 ; j--)
		{
			for (unsigned int i=0; i<rData->getWidth(); i++)
			{
				txtout<<rData->getCell(i,j)<< " ";
			}
			txtout<<"\n";

		}
	}
	txtout.close();
}

void ExportRasterData::init()
{
	//sys_in = this->getData("Data");
	//if (sys_in == 0)
	//    return;
}

string ExportRasterData::getHelpUrl()
{
	return "https://github.com/iut-ibk/DynaMind-BasicModules/blob/master/doc/ExportRasterData.md";
}

bool ExportRasterData::createInputDialog() {
	QWidget * w = new GUIExportRasterData(this);
	w->show();
	return true;
}

DM::System * ExportRasterData::getSystemIn() {
	return this->sys_in;
}

