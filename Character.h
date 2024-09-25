#ifndef KURSACH2_CHARACTER_H
#define KURSACH2_CHARACTER_H
#include "GameObject.h"
#include <string>
class Character: public GameObject {
public:
    Character(Proporties* props) : GameObject(props){}
    virtual void Draw() = 0;
    virtual void Clean() = 0;
    virtual int Update(float dt) = 0;
protected:
    std::string m_Name;
};


#endif //KURSACH2_CHARACTER_H
