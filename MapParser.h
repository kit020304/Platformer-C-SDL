#ifndef KURSACH2_MAPPARSER_H
#define KURSACH2_MAPPARSER_H
#include <map>
#include <string>
#include "GameMap.h"
#include "TileLayer.h"
#include "tinyxml.h"

class MapParser {
public:
    bool Load(std::string id, std::string source);
    void Clean();

    inline GameMap* GetMap(std::string id){return m_MapDict[id];}
    inline static MapParser * GetInstance(){return s_Instance = (s_Instance != nullptr) ? s_Instance : new MapParser();}
private:
    bool Parse(std::string id, std::string source);
    Tileset ParseTileset(TiXmlElement* xmlTileset);
    TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, TilesetList tilesets, int tilesize, int rowcount, int colcount);
    int GetExitX(){return m_ExitX;}
    int GetExitY(){return m_ExitY;}
private:
    MapParser(){};
    int m_ExitX;
    int m_ExitY;
    static MapParser* s_Instance;
    std::map<std::string, GameMap*> m_MapDict;
};


#endif //KURSACH2_MAPPARSER_H
