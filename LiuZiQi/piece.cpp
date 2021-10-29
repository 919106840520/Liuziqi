#include "piece.h"

Piece::Piece()
{

}
Piece::Piece(int x = 0, int y = 0, bool isblack = true)
{
    this->point = {x,y};
    this->isblack = isblack;
}
Piece& Piece::operator=(Piece const &P)
{
    this->point = P.point;
    this->isblack = P.isblack;
    return *this;
}
Piece::Piece(Point point,bool isblack)
{
    this->point = point;
    this->isblack = isblack;
}
Point Piece::getPoint()
{
    return this->point;
}
bool Piece::getColor()
{
    return isblack;
}
bool Piece::operator ==(Piece const &P)
{
    return this->isblack ^ P.isblack;
}
