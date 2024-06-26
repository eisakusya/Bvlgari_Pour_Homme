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
	int m_ptNum = 4; //顶点数，用于绘制正方形
	AcGePoint3d* m_points = new AcGePoint3d[4]; //顶点矩阵，用于绘制正方形
	int16_t m_realNum;  //真实的数字
	int16_t m_color;  //颜色
	//TextStyle m_ts;
	AcGePoint3d position;  // 设置文本位置
	AcGeVector3d normal;   // 设置文本法线方向
	AcGeVector3d direction; // 设置文本方向向量
	TCHAR  *pMsg; // 要显示的文本消息
	int m_length; // 获取消息长度
	ZSoft::Boolean raw = ZSoft::kFalse;  // 非原始文本
	AcGiTextStyle textStyle;  // 创建文本样式对象，根据需要设置样式属性

	
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

	TCHAR  *pCurScore; // 当前的分数
	TCHAR  *pHisScore; // 历史分数
	AcGiTextStyle scoreStyle;  // 创建文本样式对象，根据需要设置样式属性
	
public:
	void updataArray(std::array<std::array<int, ROW>, COLUMN> dataArray);
	void updataScore(double curScore, double hisScore);
	void initText();
	/*void moveBlock(AcGePoint3d stPoint, int size);
	void updateBlock();*/
	
};

#endif
