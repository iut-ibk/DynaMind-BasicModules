/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of DynaMind
 *
 * Copyright (C) 2013  Christian Urich

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

#ifndef OFFESTFACE_H
#define OFFESTFACE_H

#include <dmmodule.h>
#include <dm.h>

class DM_HELPER_DLL_EXPORT OffsetFace : public DM::Module
{
	DM_DECLARE_NODE(OffsetFace)
private:
	std::string nameInputView;
	std::string nameOutputView;

	double offest;

	DM::View inputView;
	DM::View outputView;

public:
	OffsetFace();

	void run();
	void init();

	std::vector<DM::Face *> createOffest(DM::System *sys, DM::Face * f, double offset);

	std::string getHelpUrl();

	std::string getNameInputView() const;
	void setNameInputView(const std::string &value);
	std::string getNameOutPutView() const;
	void setNameOutPutView(const std::string &value);
	double getOffest() const;
	void setOffest(double value);
};

#endif // OFFESTFACE_H
