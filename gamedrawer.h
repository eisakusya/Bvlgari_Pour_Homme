#ifndef GAMEDRAWER_H
#define GAMEDRAWER_H
#pragma once
#define COLUMN 4
#define ROW 4
#include "pch.h"
enum PartialUndoCode {
	kCenter = 1,
	kLength = 2,
};

enum ColorIndex {
	color0 = 251,  //251
	color2 = 20,
	color4 = 30,
	color8 = 40,
	color16 = 151,
	color32 = 66,
	color64 = 72,
	color128 = 102,
	color256 = 85,
	color512 = 90,
	color1024 = 1,
	color2048 = 133,
};


struct Rectangle
{
	AcGePoint3d sPoint;
	int m_ptNum = 4; //�����������ڻ���������
	AcGePoint3d* m_points = new AcGePoint3d[4]; //����������ڻ���������
	int16_t m_realNum;  //��ʵ������
	int16_t m_color;  //��ɫ
	//TextStyle m_ts;
	AcGePoint3d position;  // �����ı�λ��
	AcGeVector3d normal;   // �����ı����߷���
	AcGeVector3d direction; // �����ı���������
	TCHAR  *pMsg; // Ҫ��ʾ���ı���Ϣ
	int m_length; // ��ȡ��Ϣ����
	ZSoft::Boolean raw = ZSoft::kFalse;  // ��ԭʼ�ı�
	AcGiTextStyle textStyle;  // �����ı���ʽ���󣬸�����Ҫ������ʽ����

	
};

class GameDrawer : public AcDbEntity
{
public:
	ACRX_DECLARE_MEMBERS(GameDrawer);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	GameDrawer();
	GameDrawer(int num, AcGePoint3d center = { 500,500,0 }, const double len = 1000, const int grid = 4);
	virtual ~GameDrawer();

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
	virtual Acad::ErrorStatus updataLocation();
	
	
	//----- Graphics protocol
	virtual Acad::ErrorStatus subGetGripPoints(
		AcGePoint3dArray &gripPoints,
		AcDbIntArray &osnapModes,
		AcDbIntArray &geomIds) const;
	virtual Acad::ErrorStatus subMoveGripPointsAt(
		const AcDbIntArray &indices,
		const AcGeVector3d &offset);
	Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d& pickPoint,
		const AcGePoint3d& lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray& snapPoints,
		AcDbIntArray & geomIds
	) const;

protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits);

private:
	AcGePoint3d startPoint;
	AcGePoint3d m_center;
	double length;
	int gridSize;

	double m_len;     
	struct Rectangle rectangles[(COLUMN*ROW)];
	AcGePoint3d sPArray[(COLUMN*ROW)];

	TCHAR  *pCurScore; // ��ǰ�ķ���
	TCHAR  *pHisScore; // ��ʷ����
	AcGiTextStyle scoreStyle;  // �����ı���ʽ���󣬸�����Ҫ������ʽ����
	
public:
	void updataArray(std::array<std::array<int, ROW>, COLUMN> dataArray);
	void updataScore(double curScore, double hisScore);
	void initText();
	/*void moveBlock(AcGePoint3d stPoint, int size);
	void updateBlock();*/
	
};

#endif
