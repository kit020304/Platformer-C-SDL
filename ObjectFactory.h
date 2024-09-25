#ifndef KURSACH2_OBJECTFACTORY_H
#define KURSACH2_OBJECTFACTORY_H
#include <map>
#include <string>
#include <functional>
#include "GameObject.h"

class ObjectFactory {
public:
    GameObject* CreateObject(std::string type, Proporties* props);
    void RegisterType(std::string className, std::function<GameObject*(Proporties* props)> type);
    static ObjectFactory* GetInstance(){return  s_Instance = (s_Instance != nullptr) ? s_Instance : new ObjectFactory();}
private:
    ObjectFactory(){}
    static ObjectFactory* s_Instance;
    std::map<std::string, std::function<GameObject*(Proporties* props)>> m_TypeRegistry;
};
template <class Type>
class Registrar{
public:
    Registrar(std::string className){
        ObjectFactory::GetInstance()->RegisterType(className, [](Proporties* props) ->GameObject* {return new Type(props);});
    }
};

#endif //KURSACH2_OBJECTFACTORY_H
