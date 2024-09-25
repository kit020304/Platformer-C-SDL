#ifndef KURSACH2_TILELAYER_H
#define KURSACH2_TILELAYER_H
#include "Layer.h"
#include <string>
#include <vector>

struct Tileset{
    int FirstID, LastID;
    int RowCount, ColCount;
    int TileCount, TilSize;
    std::string Name, Source;
};

using TilesetList = std::vector<Tileset>;
using TileMap = std::vector<std::vector<int>>;

class TileLayer : public Layer{
public:
    TileLayer(int tilesize, int rowcount, int colcount, TileMap tilemap, TilesetList tilesets);
    virtual void Render();
    virtual void Update();
    inline TileMap GetTilemap(){return m_TileMap;}
    inline int GetTileSize(){return m_TileSize;}
    inline int GetWidth(){return m_ColCount;}
    inline int GetHeight(){return m_RowCount;}
private:
    int m_TileSize;
    int m_RowCount, m_ColCount;
    TileMap m_TileMap;
    TilesetList m_Tilesets;
};


#endif //KURSACH2_TILELAYER_H
