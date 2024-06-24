#include "pch.h"
#include "Block.h"
//----------------------------------------------------------------------------
Adesk::UInt32 Block::kCurrentVersionNumber = 1 ;

//----------------------------------------------------------------------------
//---- runtime definition
ACRX_DXF_DEFINE_MEMBERS (
	Block, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, BLOCK, BLOCKAPP
)

//----------------------------------------------------------------------------
//---- construct & destruct

Block::Block(){}

Block::Block( AcGePoint3d stPoint, int realNum , double size) {
	m_realNum = realNum;
	m_size = size;
	updatePoints(stPoint);

	//m_ts = TextStyle(m_points[0] ,_T("1"), size);
	//下面是设置文本的格式
	/*AcGePoint3d pos(0.0, 0.0, 0.0);
	AcGeVector3d norm(0.0, 0.0, 1.0);
	AcGeVector3d dir(1.0, 0.2, 0.0);
	TCHAR *pStr = _T("Test string");
	int len = _tcslen(pStr);*/
	initText();
	
}

Block::~Block(){}

void Block::updatePoints(AcGePoint3d stPoint) //根据起点更新点阵坐标
{
	int dir[][2] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
	m_points[0] = stPoint;
	for (int i = 0; i < m_ptNum; i++) {
		m_points[i].x = m_points[0].x +  m_size * dir[i][0];
		m_points[i].y = m_points[0].y + m_size * dir[i][1];
		//acutPrintf(_T("x坐标为：%d"), m_points[i].x);
		//acutPrintf(_T("y坐标为：%d"), m_points[i].y);
	}

}

void Block::initText() { 
	
	std::string str = std::to_string(m_realNum); // 将int转换为字符串
	// 使用strcpy()进行复制（如果是Unicode字符集可使用wcscpy）
	pMsg = new TCHAR[str.length() + 1];
#ifdef _UNICODE
	std::wstring wstr(str.begin(), str.end()); // 将std::string转换为std::wstring
	wcsncpy(pMsg, wstr.c_str(), str.length());      // 使用wcsncpy将std::wstring复制到TCHAR*
	pMsg[wstr.length()] = L'\0'; // 添加字符串结束符
#else	
	strncpy(pMsg, str.c_str(), msgSize);       // 使用strncpy将std::string复制到TCHAR*
	pMsg[str.length()] = '\0'; // 添加字符串结束符
#endif

	double width = (m_size / 4) * 0.6638 * str.length();//

	position = AcGePoint3d(m_points[0].x + (m_size /2 - width / 2) , m_points[0].y +( m_size * 3 / 8), 0.0);
	normal = AcGeVector3d(0.0, 0.0, 1.0);
	direction = AcGeVector3d(1.0, 0.0, 0.0);
	
	

	length = _tcslen(pMsg);
	if (pMsg == nullptr) {
		throw std::invalid_argument("msg cannot be null");
	}
	raw = Adesk::kFalse;
	textStyle.setTextSize(m_size / 4);
}

void Block::moveBlock(AcGePoint3d stPoint, int size) //移动（棋盘发生大小的改变），调用的函数
{
	
	m_size = size;  //更新大小
	updatePoints(stPoint);  //更新点的坐标
	initText();  //更新文本的位置

}

void Block::updateBlock(int realnum) //玩家使用WSAD时调用的函数
{
	m_realNum = realnum;  //更新真实值
	initText();  //更新文本的位置
	if (m_realNum == 0) {
		this->setVisibility(AcDb::kInvisible);
	}
	else {
		this->setVisibility(AcDb::kVisible);
	}

}




//----------------------------------------------------------------------------
//----- AcDbObject protocols
//---- Dwg Filing protocol
Acad::ErrorStatus Block::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled ();
	Acad::ErrorStatus es = AcDbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	if ((es = pFiler->writeUInt32(Block::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	return (pFiler->filerStatus());
}

Acad::ErrorStatus Block::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > Block::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//if ( version < Block::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	return (pFiler->filerStatus());
}

//----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean Block::subWorldDraw(AcGiWorldDraw * mode) {
	
	assertReadEnabled();
	mode->geometry().polygon(4, m_points); //画一个正方形多边形
	
	mode->geometry().text(position, normal, direction, pMsg,
		length, Adesk::kFalse, textStyle); //画一个text
	//mode->geometry().text(position, normal, direction, 40, -1, 0, _T("2048"));
	acutPrintf(_T("\nBlock Draw"));
	return (AcDbEntity::subWorldDraw(mode));
}

Adesk::UInt32 Block::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return (AcDbEntity::subSetAttributes(traits));
}

