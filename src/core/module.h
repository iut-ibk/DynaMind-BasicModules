/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of VIBe2
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

/**
  * @addtogroup DynaMind-Core
  */
#ifndef MODULE_H
#define MODULE_H

#include "compilersettings.h"
#include <boost/shared_ptr.hpp>
#include <list>
#include <map>
#include <boost/unordered_map.hpp>
#include <QString>
#include <boost/python.hpp>
#include <iostream>
#include <resultobserver.h>
#include <vibe_log.h>
#include <vibe_logger.h>
#include <vibe_logsink.h>


class QThreadPool;
class PortObserver;
namespace DM {
class System;
class View;
}

namespace DM {



enum  DATATYPES {
    INT,
    LONG,
    DOUBLE,
    STRING,
    FILENAME,
    STRING_LIST,
    STRING_MAP,
    BOOL,
    LASTPRIMITIVETYPE,

    USER_DEFINED_INPUT,

    SYSTEM_OUT,
    SYSTEM_IN,
    SYSTEM
};

enum PORTTYPES {
    OUTSYSTEM,
    OUTTUPLESYSTEM,
    OUTPORTS,

    INSYSTEM,
    INTUPLESYSTEM,
    INPORTS
};

enum CORINE {
    ContUrbanFabric = 2,
    DisContUrbanFabric = 3,
    RoadRailNetwork = 4,
    AgriculturalAreas = 5,
    ForestsSemiNatural = 6,
    WaterBodies = 7
};


struct ModulePrivate;

typedef boost::unordered_map<std::string, std::string> parameter_type;

class Port;
class Group;
class Simulation;
class RasterData;


/**
* @class DM::Module
*
*
* @ingroup DynaMind-Core
*
* @brief Basic module class
* @section Implementation
* @todo write stuff here
* @section Development
* To create a new DynaMind module the new module has to be derived from the module class
*
* @subsection C++
*
*
* Example Code
*
* mymodule.h
* @code
*
* #include "module.h"
*
* using namespace vibens;
* class DM_HELPER_DLL_EXPORT MyModule : public Module
* {
*     DM_DECLARE_NODE(MyModule)
*     public:
*         MyModule();
*         void run();
* };
* @endcode
*
* mymodule.cpp
* @code
*
* #include "mymodule.h"
*
* DM_DECLARE_NODE_NAME(MyModule, MyModuleGroup)
* MyModule::MyModule()
* {
* }
* void MyModule::run();
* {
* }
* @endcode
*
* To add the Module to DynaMind
* mymodules.cpp
* @code
* #include "nodefactory.h"
* #include "moduleregistry.h"
* #include "mymodule.h"
* #include "mymodule2.h"
* extern "C" void DM_HELPER_DLL_EXPORT  registerModules(ModuleRegistry *registry) {
*      registry->addNodeFactory(new NodeFactory<MyModule>());
*      registry->addNodeFactory(new NodeFactory<MyModule2>());
* }
* @endcode
* @author Christian Urich
*
*/
class DM_HELPER_DLL_EXPORT  Module {


public:
    Module();
    virtual ~Module();

    virtual void Destructor();
    virtual void init();
    virtual void run() = 0;

    /** @brief Returns the current system state
      *
      * If the succeeding module changes the data a new state of the system has to be created! */
    virtual  DM::System* getSystemState(const std::string &name);


    /** @brief Returns a pointer to the system that is linked to the inport of the module.
      * If no system can be found the method returns 0
      *
      * I ports have to be linked to a standard link and in additionally to a back link. If the internal counter of a the module is 0 (the module is)
      * called the first time. The method tries to get the data from the standard link. If the counter > 0 the back link is used.
      */
    virtual  DM::System* getSystemData(const std::string &name) ;

    /** @brief Creates a new system and adds the corresponding views */
    virtual  DM::System* getSystem_Write(string name, std::vector<View> view);

    virtual double getDoubleData(const std::string &name);
    virtual void setDoubleData(const std::string &name, const double r);


    void setInitCalled(){this->init_called=true;}
    virtual int getID() const;
    void setID(const int id);

    /** @brief adds a Parameter to the module.
      *
      * Parameters can be:
      * - DM::DOUBLE
      * - DM::INT
      * - DM::BOOL
      * - DM::STRING
      * - DM::FILENAME
      * - DM::LONG
      * - DM::STRING_LIST<
      * - DM::STRING_MAP
      */
    void addParameter(std::string name, int type, void * ref, std::string description = "");


    /** @brief Used to define the data that are used in the module.
      *
      * The data are defined as a vetor of views. For every view in the vector a port according to the AccessType is created.
      * - AccessType Read:   Inport
      * - AccessType Write:  Outport
      * - AccessType Modify: In and Outport
      *
      * The name of the port is defined by the name of the View. If a port already exists no new port is added.
      */

    void addData(std::string name, std::vector<DM::View> view);
    /** @brief Returns a pointer to the system where the view is stored
      *
      * The pointer to the system is updated by the updateParameter method. The updateParameter
      * method is always called before the run method.
      * Follwing system states are returnet. T is the system state before at the outport of the
      * module that provides the data
      * - AccessType Read:   T
      * - AccessType Write:  T+1
      * - AccessType Modify: T+1
      */

    DM::System * getData(std::string dataname);

    /** @brief Returns all views used in the module */
    std::map<std::string, std::vector<DM::View> >  getViews();

    /** @brief Returns all inports */
    std::vector<Port*> getInPorts();

    /** @brief Returns all outports */
    std::vector<Port*> getOutPorts();



    std::vector<std::string> getParameterListAsVector()  {return this->parameterList;}
    boost::unordered_map<std::string, int> getParameterList()  {return this->parameter;}
    template<class T>
    T getParameter(std::string name)   {
        T  * val = (T  *)parameter_vals[name];
        return *val;
    }

    template<class T>
    void setParameterNative(std::string name, T val)  {
        T * ref =(T * ) parameter_vals[name];

        *ref = val;
    }


    virtual void submitRunnables(QThreadPool* pool) {}

    /** @brief Returns the parameter as string value
      *
      * As seperator for STRING_LIST *|* is used and for maps also *||*
      *
      * 1*|*2*|*3*|4*||*
      *
      * 5*|*6*|*7*|*8*||*
      *
    */
    virtual std::string getParameterAsString(std::string Name);

    void setSelf(boost::python::object self);
    void setPythonModule(bool b) {this->PythonModule = b;}
    bool isPythonModule(){return PythonModule;}

    /** @brief update parameter and data used in the module.
      *
      * Based on the data used in the module the pointer to the data are updated.
      * If the module reads data from, the method tries to get the system from linked module.
      * Therefore the getSystemData method is called. If the module changes the data a new state of the system
      * is created.
      *
      */
    virtual void updateParameter();

    virtual void setParameter();
    virtual void setParameterValue(std::string name, std::string value);

    virtual void addPort(std::string LinkedDataName, int PortType);
    virtual void removePort(std::string LinkedDataName, int PortType);
    void addPortObserver(PortObserver * portobserver);
    virtual Port * getInPort(std::string Name);
    virtual Port * getOutPort(std::string Name);
    virtual  const char * getClassName()  {return "OVERWRITE getClassName";}
    virtual  const char * getFileName()  {return "OVERWRITE getFileName";}
    virtual bool isGroup()const {return false;}
    void setGroup(Group * group);
    Group * getGroup();
    std::string getUuid() const {return this->uuid;}
    void setSimulation(Simulation * simulation);
    RasterData & createRasterData(std::string name);
    //VectorData & createVectorData(std::string name);
    void createDoubleData(std::string name);
    void setName(std::string name) {
        this->name = name;
    }

    std::string getName() const {return this->name;}
    void addUrlToHelpFile(std::string s) {this->urlToHelpFile = s;}
    std::string getUrlToHelpFile(){return this->urlToHelpFile;}
    void sendImageToResultViewer(std::string);
    void sendRasterDataToResultViewer(std::map<std::string , std::string > maps);
    //void sendVectorDataToResultViewer(std::vector<VectorData> maps);
    void sendDoubleValueToPlot(double, double);
    void addResultObserver(ResultObserver * ro) {this->resultobserver.push_back(ro);}
    void resetParameter();
    int getInternalCounter(){return this->internalCounter;}
    void copyParameterFromOtherModule(Module * m);
    void printParameterList();
    void addDescription(std::string s){this->description = s;}
    std::string generateHelp();
    std::vector<PortObserver *> getPortObserver(){return this->portobserver;}
    std::vector<ResultObserver * > getResultObserver(){return this->resultobserver;}
    Simulation * getSimulation(){return this->simulation;}

    DM::RasterData * getRasterData(std::string dataname, const DM::View & view);

    /** @brief Returns if the module comes with its own GUI.
      *
      * The default value is false. If you develop your own GUI for the module the GUI is
      * overwrite this method in the module implementation, call the GUI within the method
      * an return true.
      */
    virtual bool createInputDialog(){return false;}

private:
    boost::python::object self;
    bool PythonModule;
    long id;
    void convertValus( void *value, int Type, QString val);
    std::vector<Port*> InPorts;
    std::vector<Port*> OutPorts;

    std::string uuid;
    std::string name;
    std::string urlToHelpFile;
    std::string description;


    Simulation * simulation;



protected:
    std::map<std::string, int> int_params;
    boost::unordered_map<std::string, std::string> paramRaw;
    boost::unordered_map<std::string, int> parameter;
    boost::unordered_map<std::string, std::string> parameter_description;
    boost::unordered_map<std::string, DM::System *> data_vals;
    boost::unordered_map<std::string, void *> parameter_vals;
    std::map<std::string,std::vector<DM::View> > views;
    std::map<std::string, double> InputDoubleData;
    std::vector<std::string> parameterList;
    int internalCounter;
    bool init_called;
    Group * group;
    std::vector<PortObserver *> portobserver;
    std::vector<ResultObserver * > resultobserver;
};
}


#define DM_DECLARE_NODE(node)  \
    public: \
    static const char *classname; \
    static const char *filename; \
    virtual const char *getClassName() ; \
    virtual const char *getFileName() ; \
    private:

#define  DM_DECLARE_NODE_NAME(nodename, module) \
    const char *nodename::classname = #nodename; \
    const char *nodename::getClassName()  { return nodename::classname; } \
    const char *nodename::filename = #module; \
    const char *nodename::getFileName()  { return nodename::filename; }

#endif // MODULE_H
