#include "exportnodestofile.h"
#include <fstream>
#include <QString>

DM_DECLARE_NODE_NAME(ExportNodesToFile, Modules)

ExportNodesToFile::ExportNodesToFile()
{

	this->FileName = "ress.dat";
	this->ViewName = "";

	this->addParameter("FileName", DM::STRING, &FileName);
	this->addParameter("View", DM::STRING, &ViewName);

	DM::View dummy("dummy", DM::SUBSYSTEM, DM::READ);
	std::vector<DM::View> data;
	data.push_back(dummy);

	this->addData("Data", data);
}

void ExportNodesToFile::run() {
	DM::View readview(this->ViewName, DM::NODE, DM::READ);

	DM::System * sys =this->getData("Data");

	QString fileName = QString::fromStdString(this->FileName);
	std::fstream out;
#if QT_VERSION >= 0x050000
	out.open(fileName.toLatin1(),fstream::out | fstream::app);
#else
	out.open(fileName.toAscii(),fstream::out | fstream::app);
#endif


	foreach (std::string s, sys->getUUIDsOfComponentsInView(readview)) {
		DM::Node * n = sys->getNode(s);
		out << n->getX() << "\t";
		out << n->getY() << "\t";
		out << n->getZ() << "\t";
		DM::AttributeMap am = n->getAllAttributes();
		for (DM::AttributeMap::const_iterator it = am.begin(); it != am.end(); ++it) {
			out << it->first << "\t";
			DM::Attribute * attr = it->second;
			if (attr->hasDouble())
				out << attr->getDouble() << "\t";
			if (attr->hasString())
				out << attr->getString() << "\t";
		}
		out <<  "\n";

	}
	out.close();

}

string ExportNodesToFile::getHelpUrl()
{
	return "https://github.com/iut-ibk/DynaMind-BasicModules/blob/master/doc/ExportNodesToFile.md";
}
