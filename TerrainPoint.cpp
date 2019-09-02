#include "pch.h"
#include "TerrainPoint.h"

TerrainPoint::TerrainPoint(void)
{
}

TerrainPoint::TerrainPoint(string N, double WspX, double WspY, double WspZ) : Name(N), X(WspX), Y(WspY), Z(WspZ)
{
	Mx = 0.10;
	My = 0.10;
	Mz = 0.10;
	Type = 3;
	TypParametru = "MES";
	Use = true;
}

TerrainPoint::TerrainPoint(string N, double WspX, double WspY, double WspZ, double ErrX, double ErrY, double ErrZ) : Name(N), X(WspX), Y(WspY), Z(WspZ), Mx(ErrX), My(ErrY), Mz(ErrZ)
{
	Type = 3;
	TypParametru = "MES";
	Use = true;
}

TerrainPoint::TerrainPoint(string N, double WspX, double WspY, double WspZ, short T) : Name(N), X(WspX), Y(WspY), Z(WspZ), Type(T)
{
	Mx = 0.10;
	My = 0.10;
	Mz = 0.10;
	TypParametru = "MES";
	Use = true;
}


TerrainPoint::TerrainPoint(string N, double WspX, double WspY, double WspZ, double ErrX, double ErrY, double ErrZ, short T) : Name(N), X(WspX), Y(WspY), Z(WspZ), Mx(ErrX), My(ErrY), Mz(ErrZ), Type(T)
{
	TypParametru = "MES";
	Use = true;
}

TerrainPoint::~TerrainPoint(void)
{
}

void TerrainPoint::Wypisz(void)
{
	cout<<"\nId:\t\t"<<Name<<"\tType: "<<Type<<endl;
	cout<<"Coordinates:\t"<<X<<"\t"<<Y<<"\t"<<Z<<endl;
	cout<<"Accuracy:\t"<<Mx<<"\t"<<My<<"\t"<<Mz<<endl;
}
