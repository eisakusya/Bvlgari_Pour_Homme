#ifndef GAMEDRAWER_H
#define GAMEDRAWER_H
#pragma once
#define COLUMN 4
#define ROW 4

enum PartialUndoCode {
	kCenter = 1,
	kLength = 2,
};

class gamedrawer : public AcDbEntity
{
public:
	ACRX_DECLARE_MEMBERS(gamedrawer);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	gamedrawer();
	gamedrawer(const std::array<std::array<int, COLUMN>, ROW>& data,const AcGePoint3d& center = { 500,500,0 }, const double len = 1000, const int grid = 4);
	virtual ~gamedrawer();

	//----- AcDbObject protocols
	//---- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler *pFiler) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler *pFiler);
	virtual Acad::ErrorStatus applyPartialUndo(AcDbDwgFiler* undoFiler, AcRxClass* classObj);

	//----- AcDbEntity protocols
	virtual Acad::ErrorStatus setStart(AcGePoint3d& start);
	virtual AcGePoint3d getStartpoint() const;
	virtual Acad::ErrorStatus setCenter(AcGePoint3d& center);
	virtual AcGePoint3d getCenter() const;
	virtual Acad::ErrorStatus setLength(double len);
	virtual double getLength() const;
	virtual double getGridsize() const;
	virtual Acad::ErrorStatus setdataArray(std::array<std::array<int, COLUMN>, ROW>& data);
	
	//----- Graphics protocol
	virtual Acad::ErrorStatus subGetGripPoints(
		AcGePoint3dArray &gripPoints,
		AcDbIntArray &osnapModes,
		AcDbIntArray &geomIds) const;
	virtual Acad::ErrorStatus subMoveGripPointsAt(
		const AcDbIntArray &indices,
		const AcGeVector3d &offset);
	

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits);

private:
	std::array<std::array<int, COLUMN>, ROW> dataArray;
	AcGePoint3d startPoint;
	AcGePoint3d m_center;
	double length;
	int gridSize;
};

#endif
