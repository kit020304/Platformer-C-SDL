#ifndef KURSACH2_GAMEMAP_H
#define KURSACH2_GAMEMAP_H
#include "Layer.h"
#include <vector>


class GameMap {
public:
    void Render(){
        for(unsigned int i = 0; i < m_MapLayers.size(); i++)
            m_MapLayers[i]->Render();
    }
    void Update(){
        for(unsigned int i = 0; i < m_MapLayers.size(); i++)
            m_MapLayers[i]->Update();
    }
    int GetExitX(){return m_ExitX;}
    int GetExitY(){return m_ExitY;}

    void SetExitX(int X){m_ExitX = X;}
    void SetExitY(int Y){m_ExitY = Y;}
    std::vector<Layer*> GetMapLayers(){return m_MapLayers;}
private:
    friend class MapParser;
    int m_ExitX;
    int m_ExitY;
    std::vector<Layer*> m_MapLayers;
};


#endif //KURSACH2_GAMEMAP_H
