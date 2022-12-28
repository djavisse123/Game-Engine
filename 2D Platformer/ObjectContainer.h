/**
 * @file ObjectContainer.h
 * @author Daniel Avisse (djavisse)
 * This is the ObjectContainer header file. This holds
 * the list and singleton need for the ObjectContainer.
 */
#ifndef OBJECTCONTAINER_H
#define OBJECTCONTAINER_H

#include <cstdio>
#include <v8.h>
#include <map>

class ObjectContainer
{
private:
    static ObjectContainer * objectContainerInstance;
    ObjectContainer();
public:
    static ObjectContainer * getInstance();
    //Key = Context Name, Value = Object GUID
    std::map<std::string, std::string> listOfV8Objs;
    //std::vector<std::string> listOfV8Objs;
};

#endif