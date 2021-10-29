#ifndef PIECE_H
#define PIECE_H
#include "point.h"

class Piece
{
private:
    Point point;
    bool isblack;
public:
    Piece();
    Piece(Point point,bool isblack);
    Piece(int x,int y,bool isblack);
    Piece& operator=(Piece const &P);
    Point getPoint();
    bool getColor();
    bool operator==(Piece const &P);
};

#endif // PIECE_H
