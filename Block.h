#ifndef BLOCK_H
#define BLOCK_H
#pragma once

class Block : public AcDbEntity
{
public:
	int m_ptNum = 4; //顶点数，用于绘制正方形
	AcGePoint3d* m_points = new AcGePoint3d[m_ptNum]; //顶点矩阵，用于绘制正方形
	//  3————2
	//	|		 |
	//  0————1
	int m_realNum;  //真实的数字
	double m_size;     //边长的大小
	int m_color;  //颜色
	//TextStyle m_ts;
	AcGePoint3d position;  // 设置文本位置
	AcGeVector3d normal;   // 设置文本法线方向
	AcGeVector3d direction; // 设置文本方向向量
	TCHAR  *pMsg; // 要显示的文本消息
	int length; // 获取消息长度
	ZSoft::Boolean raw = ZSoft::kFalse;  // 非原始文本
	AcGiTextStyle textStyle;  // 创建文本样式对象，根据需要设置样式属性


	 // 复制赋值运算符
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

	void updateBlock(int realnum); //玩家使用WSAD时调用的函数

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
