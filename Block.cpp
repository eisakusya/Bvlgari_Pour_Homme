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
	//�����������ı��ĸ�ʽ
	/*AcGePoint3d pos(0.0, 0.0, 0.0);
	AcGeVector3d norm(0.0, 0.0, 1.0);
	AcGeVector3d dir(1.0, 0.2, 0.0);
	TCHAR *pStr = _T("Test string");
	int len = _tcslen(pStr);*/
	initText();
	
}

Block::~Block(){}

void Block::updatePoints(AcGePoint3d stPoint) //���������µ�������
{
	int dir[][2] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
	m_points[0] = stPoint;
	for (int i = 0; i < m_ptNum; i++) {
		m_points[i].x = m_points[0].x +  m_size * dir[i][0];
		m_points[i].y = m_points[0].y + m_size * dir[i][1];
		//acutPrintf(_T("x����Ϊ��%d"), m_points[i].x);
		//acutPrintf(_T("y����Ϊ��%d"), m_points[i].y);
	}

}

void Block::initText() { 
	
	std::string str = std::to_string(m_realNum); // ��intת��Ϊ�ַ���
	// ʹ��strcpy()���и��ƣ������Unicode�ַ�����ʹ��wcscpy��
	pMsg = new TCHAR[str.length() + 1];
#ifdef _UNICODE
	std::wstring wstr(str.begin(), str.end()); // ��std::stringת��Ϊstd::wstring
	wcsncpy(pMsg, wstr.c_str(), str.length());      // ʹ��wcsncpy��std::wstring���Ƶ�TCHAR*
	pMsg[wstr.length()] = L'\0'; // ����ַ���������
#else	
	strncpy(pMsg, str.c_str(), msgSize);       // ʹ��strncpy��std::string���Ƶ�TCHAR*
	pMsg[str.length()] = '\0'; // ����ַ���������
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

void Block::moveBlock(AcGePoint3d stPoint, int size) //�ƶ������̷�����С�ĸı䣩�����õĺ���
{
	
	m_size = size;  //���´�С
	updatePoints(stPoint);  //���µ������
	initText();  //�����ı���λ��

}

void Block::updateBlock(int realnum) //���ʹ��WSADʱ���õĺ���
{
	m_realNum = realnum;  //������ʵֵ
	initText();  //�����ı���λ��
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
	mode->geometry().polygon(4, m_points); //��һ�������ζ����
	
	mode->geometry().text(position, normal, direction, pMsg,
		length, Adesk::kFalse, textStyle); //��һ��text
	//mode->geometry().text(position, normal, direction, 40, -1, 0, _T("2048"));
	acutPrintf(_T("\nBlock Draw"));
	return (AcDbEntity::subWorldDraw(mode));
}

Adesk::UInt32 Block::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return (AcDbEntity::subSetAttributes(traits));
}

