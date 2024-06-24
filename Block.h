#ifndef BLOCK_H
#define BLOCK_H
#pragma once

class Block : public AcDbEntity
{
public:
	int m_ptNum = 4; //�����������ڻ���������
	AcGePoint3d* m_points = new AcGePoint3d[m_ptNum]; //����������ڻ���������
	//  3��������2
	//	|		 |
	//  0��������1
	int m_realNum;  //��ʵ������
	double m_size;     //�߳��Ĵ�С
	int m_color;  //��ɫ
	//TextStyle m_ts;
	AcGePoint3d position;  // �����ı�λ��
	AcGeVector3d normal;   // �����ı����߷���
	AcGeVector3d direction; // �����ı���������
	TCHAR  *pMsg; // Ҫ��ʾ���ı���Ϣ
	int length; // ��ȡ��Ϣ����
	ZSoft::Boolean raw = ZSoft::kFalse;  // ��ԭʼ�ı�
	AcGiTextStyle textStyle;  // �����ı���ʽ���󣬸�����Ҫ������ʽ����


	 // ���Ƹ�ֵ�����
	/*Block& operator=(const Block& other) {
		if (this != &other) {
			m_ptNum = other.m_ptNum;
			m_points = other.m_points;
			m_realNum = other.m_realNum;
			m_size = other.m_size;
			m_color = other.m_color;
			position = other.position;
			normal = other.normal;
			direction = other.direction;
			length = other.length;
			raw = other.raw;
			textStyle = other.textStyle;

			if (pMsg) {
				delete[] pMsg;
			}
			if (other.pMsg) {
				size_t msgLength = _tcslen(other.pMsg) + 1;
				pMsg = new TCHAR[msgLength];
				_tcscpy_s(pMsg, msgLength, other.pMsg);
			}
			else {
				pMsg = nullptr;
			}
		}
		return *this;
	}
*/
	ACRX_DECLARE_MEMBERS(Block);


protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	Block();
	Block(AcGePoint3d stPoint, int realNum, double size);
	virtual ~Block();

	void updatePoints(AcGePoint3d stPoint);
	void initText();
	void moveBlock(AcGePoint3d stPoint, int size);

	void updateBlock(int realnum); //���ʹ��WSADʱ���õĺ���

	//----- AcDbObject protocols
	//---- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler);

	//----- AcDbEntity protocols
	//----- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode);
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits);

};

#endif
