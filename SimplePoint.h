#pragma once

template  <class typ_oznaczenia> // Oznaczenia moga byc np typu string lub int
class SimplePoint
{
public:
	double x;
	double y;
	typ_oznaczenia Nr;
	SimplePoint();
	SimplePoint(double _x, double _y, typ_oznaczenia _Nr);
	~SimplePoint();
};

template  <class typ_oznaczenia>
SimplePoint<typ_oznaczenia>::SimplePoint(double _x, double _y, typ_oznaczenia _Nr) : x(_x), y(_y), Nr(_Nr)
{
}

template  <class typ_oznaczenia>
SimplePoint<typ_oznaczenia>::SimplePoint()
{
}

template  <class typ_oznaczenia>
SimplePoint<typ_oznaczenia>::~SimplePoint()
{
}
