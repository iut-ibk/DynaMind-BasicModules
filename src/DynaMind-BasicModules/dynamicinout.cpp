/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2011  Christian Urich
 
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

#include "dynamicinout.h"
#include <dm.h>
#include <guidynamicinout.h>
DM_DECLARE_NODE_NAME (DynamicInOut, )

DynamicInOut::DynamicInOut()
{
    PrevSize = 0;
    this->addParameter("NewAttributes", DM::STRING_LIST, &NewAttributes);
    DM::View inlets = DM::View("Inlets", DM::NODE, DM::READ);
    inlets.getAttribute("A");
    inlets.getAttribute("B");
    inlets.getAttribute("C");
    std::vector<DM::View> views;    
    views.push_back(inlets);
    this->addData("Inport", views);


}

void DynamicInOut::run() {
    Logger(Debug) << "DynInOut";

}

void DynamicInOut::init() {
    Logger(Debug) << "Init Called";

    if (PrevSize == NewAttributes.size())
        return;
    PrevSize = NewAttributes.size();
    DM::View inlets = DM::View("Inlets", DM::NODE, DM::READ);
    inlets.getAttribute("A");
    inlets.getAttribute("B");
    inlets.getAttribute("C");
    foreach(std::string s, NewAttributes)
        inlets.addAttribute(s);
    std::vector<DM::View> views;
    views.push_back(inlets);
    this->addData("Inport", views);

}

void DynamicInOut::getAttribute(std::string n) {
    //Get View
    std::vector<View> views;
    DM::View view;
    foreach (DM::View v,this->views) {
        if (v.getName().compare("Inlets")==0) {
            view = v;
        }
    }

    view.getAttribute(n);

    views.push_back(view);
    this->views = views;

    this->addData("Inport", views);
}

void DynamicInOut::addAttribute(std::string n) {
    //Get View

    this->NewAttributes.push_back(n);

    //Call the init function after changing adding new attributes
    this->init();
}

bool DynamicInOut::createInputDialog() {
    QWidget * w = new GUIDynamicInOut(this);
    w->show();
    return true;
}
