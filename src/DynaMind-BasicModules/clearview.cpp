#include "clearview.h"
#include <guiclearview.h>

DM_DECLARE_NODE_NAME(ClearView, )
ClearView::ClearView()
{
    sys_in = 0;
    this->NameOfExistingView = "";
    this->NameOfExistingView_old = "";
    std::vector<DM::View> data;
    data.push_back(  DM::View ("dummy", DM::SUBSYSTEM, DM::MODIFY) );
    this->addParameter("NameOfExistingView", DM::STRING, &this->NameOfExistingView);
    this->addData("Data", data);
}

void ClearView::run() {
    DM::System * data = this->getData("Data");
    DM::View v = DM::View (NameOfExistingView, sys_in->getViewDefinition(NameOfExistingView)->getType(), DM::MODIFY);
    DM::ComponentMap cmp = data->getAllComponentsInView(v);
    for (DM::ComponentMap::const_iterator it = cmp.begin(); it != cmp.end(); ++it) {
        data->removeComponentFromView(it->second, v);
    }
    int i = 0;


}

void ClearView::init() {
    sys_in = this->getData("Data");
    if (sys_in == 0)
        return;
    if (NameOfExistingView.empty())
        return;
    if (NameOfExistingView.compare(NameOfExistingView_old) == 0)
        return;

    std::vector<DM::View> data;
    data.push_back(  DM::View (NameOfExistingView, sys_in->getViewDefinition(NameOfExistingView)->getType(), DM::MODIFY) );
    this->addData("Data", data);
    NameOfExistingView_old = NameOfExistingView;

}

bool ClearView::createInputDialog() {
    QWidget * w = new GUIClearView(this);
    w->show();
    return true;
}

DM::System * ClearView::getSystemIn() {
    return this->sys_in;
}
