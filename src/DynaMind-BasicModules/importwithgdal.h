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

#ifndef IMPORTWITHGDAL_H
#define IMPORTWITHGDAL_H
#include <dmmodule.h>
#include <dm.h>
#include "ogrsf_frmts.h"
#include "ogr_geometry.h"
#include "gdal_priv.h"
#include <QHash>
#include <QString>


using namespace DM;

enum DRIVERTYPE {
	ShapeFile,
	WFS,
	PostGIS
};


class DM_HELPER_DLL_EXPORT ImportwithGDAL : public Module
{
	DM_DECLARE_NODE(ImportwithGDAL)

	private:
		bool fileok;

public:
	std::string FileName;
	std::string ViewName;

	std::string WFSDataName;
	std::string WFSServer;
	std::string WFSUsername;
	std::string WFSPassword;

	std::string PostGISServer;
	std::string PGDatabase;
	std::string PGTable;
	std::string PGUsername;
	std::string PGPassword;
	std::string attribute_filter;
	std::string view_filter;

	bool append;
	int epsgcode;
	int epsgcode_import;
	bool linkWithExistingView;
	bool flip_wfs;

	double offsetX;
	double offsetY;
private:
	bool isvectordata;
	bool transformok;
	double tol;

	std::string FileName_old;
	std::string ViewName_old;
	std::string WFSDataName_old;
	std::string WFSServer_old;
	std::string WFSUsername_old;
	std::string WFSPassword_old;

	bool append_old;

	DM::View view;
	double devider;
	std::map<std::string, std::string> attributesToImport;
	bool ImportAll;
	OGRCoordinateTransformation *poCT;

	OGRCoordinateTransformation *poCT_reverse;

	QHash<QString, std::vector<DM::Node* > > nodeList;
	DM::Node * addNode(DM::System * sys, double x, double y, double z, string uuid = "");
	void appendAttributes(DM::Component * cmp, OGRFeatureDefn *poFDefn, OGRFeature *poFeature);

	std::vector<Node*> ExtractNodes(System* sys, OGRLineString *ls);
	std::vector<Node*> ExtractNodesFromFace(System* sys, OGRLinearRing *ls);

	DM::Component * loadNode(DM::System * sys,  OGRFeature *poFeature);
	DM::Component * loadEdge(DM::System * sys,  OGRFeature *poFeature);
	DM::Component * loadFace(DM::System * sys,  OGRFeature *poFeature);
	void initPointList(DM::System * sys);
	QString createHash(double x, double y);
	void vectorDataInit(OGRLayer       *poLayer);
	void rasterDataInit(GDALDataset  *poDataset);
	bool importVectorData();
	bool importRasterData();
	bool transform(double *x, double *y, bool reverse=false);
	void reset();
	bool moduleParametersChanged();
	std::string server_full_name;

	OGRLayer* LoadWFSLayer(OGRDataSource *poDS);

	int driverType;
public:
	void run();
	void init();
	virtual bool  createInputDialog();
	string getHelpUrl();
	ImportwithGDAL();
	~ImportwithGDAL();
	void test_writing();

	OGRLayer * initPostGISServer(OGRDataSource *poDS, std::string DBName, std::string host, std::string table, std::string user = "", std::string password = "", std::string filter = "", OGRGeometry * spatial_filter = NULL);
	OGRGeometry * convertFaceToOGRGeometry(DM::Face * f);

	std::string getPostGISServer() const;
	void setPostGISServer(const std::string &value);
	std::string getDatabase() const;
	void setDatabase(const std::string &value);
	int getDriverType() const;
	void setDriverType(int value);
};

#endif // IMPORTWITHGDAL_H
