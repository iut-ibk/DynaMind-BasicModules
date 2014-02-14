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

#ifndef IMPORTRASTERDATA_H
#define IMPORTRASTERDATA_H

#include <dmmodule.h>
#include <dm.h>

class DM_HELPER_DLL_EXPORT ImportRasterData : public DM::Module
{
    DM_DECLARE_NODE(ImportRasterData)
private:
    double multiplier;
    double flip;
    std::string FileName;
    std::string dataname;
    std::string dataname_old;
    DM::View Coords;

public:
    ImportRasterData();
    void run();
    void init();
    virtual bool createInputDialog();
    std::string getFilename();
    ~ImportRasterData();
    std::string workingDir;
};

#endif // IMPORTRASTERDATA_H
