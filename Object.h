#ifndef KURSACH2_OBJECT_H
#define KURSACH2_OBJECT_H


class Object {
public:
    virtual void Draw() = 0;
    virtual int Update(float dt) = 0;
    virtual void Clean() = 0;
};


#endif //KURSACH2_OBJECT_H
