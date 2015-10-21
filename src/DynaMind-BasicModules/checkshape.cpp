#include "checkshape.h"
#include <dmcomponent.h>
#include <dmsystem.h>
#include <dmattribute.h>
#include <dmnode.h>


DM_DECLARE_NODE_NAME( CheckShape, )
CheckShape::CheckShape() {
    std::vector<DM::View> views;
    shape = DM::View("Shape", DM::SUBSYSTEM, DM::READ);
    shape.getAttribute("Type");
    views.push_back(shape);

    this->addData("Shapefile", views);
}



void CheckShape::run()
{
    //TODO Aactivate
    sys_in = this->getData("Shapefile");
    std::map<std::string, DM::Component*> comp = sys_in->getAllComponentsInView(shape);

    Logger(Standard) << "Found Components: " << comp.size();
    Logger(Standard) << "Found Subsystems: " << sys_in->getAllSubSystems().size();
    Logger(Standard) << "Found Views: " << sys_in->getViews().size();

    if(sys_in->getViews().size())
        Logger(Standard) << "Name of 1st view: " << sys_in->getNamesOfViews()[0];

    Logger(Standard) << "Check well done :-)";
}

CheckShape::~CheckShape() {
}
