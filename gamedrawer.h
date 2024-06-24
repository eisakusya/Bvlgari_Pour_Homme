#ifndef GAMEDRAWER_H
#define GAMEDRAWER_H
#pragma once
#define COLUMN 4
#define ROW 4

enum PartialUndoCode {
	kCenter = 1,
	kLength = 2,
};

<<<<<<< Updated upstream
class gamedrawer : public AcDbEntity
{
public:
	ACRX_DECLARE_MEMBERS(gamedrawer);
=======
struct Rectangle
{
	AcGePoint3d sPoint;
	int m_ptNum = 4; //�����������ڻ���������
	AcGePoint3d* m_points = new AcGePoint3d[4]; //����������ڻ���������
	int m_realNum;  //��ʵ������
	int m_color;  //��ɫ
	//TextStyle m_ts;
	AcGePoint3d position;  // �����ı�λ��
	AcGeVector3d normal;   // �����ı����߷���
	AcGeVector3d direction; // �����ı���������
	TCHAR  *pMsg; // Ҫ��ʾ���ı���Ϣ
	int m_length; // ��ȡ��Ϣ����
	ZSoft::Boolean raw = ZSoft::kFalse;  // ��ԭʼ�ı�
	AcGiTextStyle textStyle;  // �����ı���ʽ���󣬸�����Ҫ������ʽ����
};

class Gamedrawer : public AcDbEntity
{
public:
	ACRX_DECLARE_MEMBERS(Gamedrawer);
>>>>>>> Stashed changes

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
<<<<<<< Updated upstream
	gamedrawer();
	gamedrawer(const std::array<std::array<int, COLUMN>, ROW>& data,const AcGePoint3d& center = { 500,500,0 }, const double len = 1000, const int grid = 4);
	virtual ~gamedrawer();
=======
	Gamedrawer();
	Gamedrawer(int num, AcGePoint3d center = { 500,500,0 }, const double len = 1000, const int grid = 4);
	virtual ~Gamedrawer();
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
	virtual Acad::ErrorStatus setdataArray(std::array<std::array<int, COLUMN>, ROW>& data);
=======
	virtual Acad::ErrorStatus updataLocation();
	
>>>>>>> Stashed changes
	
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
<<<<<<< Updated upstream
=======

	double m_len;     
	struct Rectangle rectangles[(COLUMN*ROW)];
	AcGePoint3d sPArray[(COLUMN*ROW)];
	
public:
	void updataArray(std::array<std::array<int, ROW>, COLUMN> dataArray);
	void initText();
	/*void moveBlock(AcGePoint3d stPoint, int size);
	void updateBlock();*/
>>>>>>> Stashed changes
};

#endif
