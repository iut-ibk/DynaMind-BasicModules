#include <QtCore>
#include <DMcomponent.h>
#include <DMattribute.h>

#include <assert.h>

using namespace DM;

Component::Component(std::string name, std::string id)
{
    this->name=name;
    this->id=id;
}

Component::Component(const Component& c)
{
    this->name=c.name;
    this->id=c.id;
    attributesview=c.attributesview;

    std::map<std::string,Component*>::iterator it;

    for ( it=ownedchilds.begin() ; it != ownedchilds.end(); it++ )
    {
        ownedchilds[(*it).first]=(*it).second->clone();
        childsview[(*it).first]=ownedchilds[(*it).first];
    }
}

Component::~Component()
{
    while(ownedchilds.size())
    {
        delete (*ownedchilds.begin()).second;
        ownedchilds.erase(0);
    }

    while(ownedattributes.size())
    {
        delete (*ownedattributes.begin()).second;
        ownedattributes.erase(0);
    }
}

void Component::setName(std::string name)
{
    this->name=name;
}

std::string Component::getName()
{
    return name;
}

void Component::setID(std::string id)
{
    this->id=id;
}

std::string Component::getID()
{
    return id;
}

bool Component::addAttribute(Attribute *newattribute)
{
    if(!newattribute)
        return false;

    if(attributesview.find(newattribute->getName())!=attributesview.end())
        return false;

    attributesview[newattribute->getName()] = newattribute;
    ownedattributes[newattribute->getName()] = newattribute;
    return true;
}

bool Component::changeAttribute(Attribute *newattribute)
{
    if(!newattribute)
        return false;

    if(ownedattributes.find(newattribute->getName())!=ownedattributes.end())
        delete ownedattributes[newattribute->getName()];

    ownedattributes[newattribute->getName()] = newattribute;
    attributesview[newattribute->getName()] = newattribute;

    return true;
}

bool Component::removeAttribute(std::string name)
{
    if(attributesview.find(name)!=attributesview.end())
    {
        if(ownedattributes.find(name)!=ownedattributes.end())
        {
            delete ownedattributes[name];
            ownedattributes.erase(name);
        }

        attributesview.erase(name);
        return true;
    }

    return false;
}

Attribute* Component::getAttribute(std::string name)
{
    return attributesview[name];
}

std::map<std::string, Attribute*> Component::getAllAttributes()
{
    return attributesview;
}

bool Component::addChild(Component *newcomponent)
{
    if(!newcomponent)
        return false;

    if(childsview.find(newcomponent->getName())!=childsview.end())
        return false;

    childsview[newcomponent->getName()] = newcomponent;
    ownedchilds[newcomponent->getName()] = newcomponent;
    return true;
}

bool Component::changeChild(Component *newcomponent)
{
    if(!newcomponent)
        return false;

    if(ownedchilds.find(newcomponent->getName())!=ownedchilds.end())
        delete ownedchilds[newcomponent->getName()];

    ownedchilds[newcomponent->getName()] = newcomponent;
    childsview[newcomponent->getName()] = newcomponent;

    return true;
}

bool Component::removeChild(std::string name)
{
    if(childsview.find(name)!=childsview.end())
    {
        if(ownedchilds.find(name)!=ownedchilds.end())
        {
            delete ownedchilds[name];
            ownedchilds.erase(name);
        }

        childsview.erase(name);
        return true;
    }

    return false;
}

Component* Component::getChild(std::string name)
{
    return childsview[name];
}

std::map<std::string, Component*> Component::getAllChilds()
{
    return childsview;
}