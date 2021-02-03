#ifndef H_DRAW
#define H_DRAW

#include <vector>

class DrawContext{

    public:

};

class DrawController{

    int _width;
    int _height;

    std::vector<DrawContext> layers;

    public:

    DrawController();

    bool AddDrawingLayer();

};

#endif
