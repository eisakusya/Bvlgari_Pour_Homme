#include "pch.h"
#include "gamedrawer.h"

//----------------------------------------------------------------------------
<<<<<<< Updated upstream
Adesk::UInt32 gamedrawer::kCurrentVersionNumber = 1 ;
=======
Adesk::UInt32 Gamedrawer::kCurrentVersionNumber = 1 ;
>>>>>>> Stashed changes

//----------------------------------------------------------------------------
//---- runtime definition
ACRX_DXF_DEFINE_MEMBERS (
<<<<<<< Updated upstream
	gamedrawer, AcDbEntity,
=======
	Gamedrawer, AcDbEntity,
>>>>>>> Stashed changes
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, GAMEDRAWER, GAMEDRAWERAPP
)

//----------------------------------------------------------------------------
//---- construct & destruct

<<<<<<< Updated upstream
gamedrawer::gamedrawer(){
	dataArray = { 0 };
	length = 1000;
	startPoint = { 0,0,0 };
	gridSize = 4;
	m_center.set(500,500,0);
}

gamedrawer::gamedrawer(const std::array<std::array<int, COLUMN>, ROW>& data,const AcGePoint3d& center, const double len, const int grid) {
	dataArray = data;
	length = len;
	m_center = center;
	gridSize = grid;
	startPoint.set(m_center.x - length / 2, m_center.y - length / 2, m_center.z);
}

gamedrawer::~gamedrawer(){}
=======
Gamedrawer::Gamedrawer() {
	setLength(1000.0);
	startPoint = { 0,0,0 };
	gridSize = 4;
	m_center.set(500, 500, 0);
	m_len = length / gridSize;
	updataLocation();
	//updataArray();
	initText();
}

Gamedrawer::Gamedrawer(int num, AcGePoint3d center, const double len, const int grid) {
	setLength(len);
	setCenter(center);
	gridSize = grid;
	startPoint.set(m_center.x - length / 2, m_center.y - length / 2, m_center.z);
	m_len = length / gridSize;
	updataLocation();
	for (int i = 0; i < 16; i++) {
		rectangles[i].m_realNum = i;
	}
	initText();
}

Gamedrawer::~Gamedrawer(){}
>>>>>>> Stashed changes

//----------------------------------------------------------------------------
//----- AcDbObject protocols
//---- Dwg Filing protocol
<<<<<<< Updated upstream
Acad::ErrorStatus gamedrawer::dwgOutFields (AcDbDwgFiler *pFiler) const {
=======
Acad::ErrorStatus Gamedrawer::dwgOutFields (AcDbDwgFiler *pFiler) const {
>>>>>>> Stashed changes
	assertReadEnabled ();
	Acad::ErrorStatus es = AcDbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
<<<<<<< Updated upstream
	if ((es = pFiler->writeUInt32(gamedrawer::kCurrentVersionNumber)) != Acad::eOk)
=======
	if ((es = pFiler->writeUInt32(Gamedrawer::kCurrentVersionNumber)) != Acad::eOk)
>>>>>>> Stashed changes
		return (es);
	//----- Output params
	//.....
	pFiler->writeDouble(length);
	//pFiler->writePoint3d(startPoint);
	pFiler->writePoint3d(m_center);

	return (pFiler->filerStatus());
}

<<<<<<< Updated upstream
Acad::ErrorStatus gamedrawer::dwgInFields(AcDbDwgFiler * pFiler) {
=======
Acad::ErrorStatus Gamedrawer::dwgInFields(AcDbDwgFiler * pFiler) {
>>>>>>> Stashed changes
	assertWriteEnabled();
	Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
<<<<<<< Updated upstream
	if (version > gamedrawer::kCurrentVersionNumber)
=======
	if (version > Gamedrawer::kCurrentVersionNumber)
>>>>>>> Stashed changes
		return (Acad::eMakeMeProxy);
	//if ( version < gamedrawer::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	//pFiler->readPoint3d(&startPoint);
	pFiler->readDouble(&length);
	pFiler->readPoint3d(&m_center);

	return (pFiler->filerStatus());
}

<<<<<<< Updated upstream
Acad::ErrorStatus gamedrawer::dxfOutFields(AcDbDxfFiler *pFiler) const {
=======
Acad::ErrorStatus Gamedrawer::dxfOutFields(AcDbDxfFiler *pFiler) const {
>>>>>>> Stashed changes
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dxfOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	es = pFiler->writeItem(AcDb::kDxfSubclass, _T("SampleCustEnt"));
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
<<<<<<< Updated upstream
	if ((es = pFiler->writeUInt32(AcDb::kDxfInt32, gamedrawer::kCurrentVersionNumber)) != Acad::eOk)
=======
	if ((es = pFiler->writeUInt32(AcDb::kDxfInt32, Gamedrawer::kCurrentVersionNumber)) != Acad::eOk)
>>>>>>> Stashed changes
		return (es);
	//----- Output params
	//.....

	return (pFiler->filerStatus());
}

<<<<<<< Updated upstream
Acad::ErrorStatus gamedrawer::dxfInFields(AcDbDxfFiler *pFiler) {
=======
Acad::ErrorStatus Gamedrawer::dxfInFields(AcDbDxfFiler *pFiler) {
>>>>>>> Stashed changes
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dxfInFields(pFiler);
	if (es != Acad::eOk || !pFiler->atSubclassData(_T("SampleCustEnt")))
		return (pFiler->filerStatus());
	//----- Object version number needs to be read first
	struct resbuf rb;
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt32) {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode, _T("\nError: expected group code %d (version #)"), AcDb::kDxfInt32);
		return (pFiler->filerStatus());
	}
	Adesk::UInt32 version = (Adesk::UInt32)rb.resval.rlong;
<<<<<<< Updated upstream
	if (version > gamedrawer::kCurrentVersionNumber)
=======
	if (version > Gamedrawer::kCurrentVersionNumber)
>>>>>>> Stashed changes
		return (Acad::eMakeMeProxy);

	//----- Read params in non order dependant manner
	AcGePoint3d center;
	while (es == Acad::eOk && (es = pFiler->readResBuf(&rb)) == Acad::eOk) {
		switch (rb.restype) {
			//----- Read params by looking at their DXF code (example below)
		case AcDb::kDxfXCoord:
			center = { rb.resval.rpoint[0], rb.resval.rpoint[1], rb.resval.rpoint[2] };
			setCenter(center);
			//m_center.set(rb.resval.rpoint[0], rb.resval.rpoint[1], rb.resval.rpoint[2]);
			break;
		case AcDb::kDxfReal:
			setLength(rb.resval.rreal);
			//m_radius = rb.resval.rreal;  
			break;
		default:
			//----- An unrecognized group. Push it back so that the subclass can read it again.
			pFiler->pushBackItem();
			es = Acad::eEndOfFile;
			break;
		}
	}

	if (es != Acad::eEndOfFile)
		return (Acad::eInvalidResBuf);

	return (pFiler->filerStatus());
}


<<<<<<< Updated upstream
Acad::ErrorStatus gamedrawer::applyPartialUndo(AcDbDwgFiler* undoFiler, AcRxClass* classObj)
{
	if (classObj != gamedrawer::desc())
=======
Acad::ErrorStatus Gamedrawer::applyPartialUndo(AcDbDwgFiler* undoFiler, AcRxClass* classObj)
{
	if (classObj != Gamedrawer::desc())
>>>>>>> Stashed changes
		return AcDbEntity::applyPartialUndo(undoFiler, classObj);
	Adesk::Int16 shortCode;
	undoFiler->readItem(&shortCode);
	PartialUndoCode code = (PartialUndoCode)shortCode;
	double len = 0;
	AcGePoint3d center;
	switch (code) {
	case kLength:
		undoFiler->readDouble(&len);
		setLength(len);
		break;
	case kCenter:
		undoFiler->readPoint3d(&center);
		setCenter(center);
		break;
	default:
		assert(Adesk::kFalse);
		break;
	}
	acutPrintf(_T("\nUndo successly"));
	return Acad::eOk;
}


//----------------------------------------------------------------------------
//----- AcDbEntity protocols

<<<<<<< Updated upstream
Acad::ErrorStatus gamedrawer::setStart(AcGePoint3d& start) {
	startPoint = start;
	return Acad::eOk;
}
AcGePoint3d gamedrawer::getStartpoint() const {
	return startPoint;
}

Acad::ErrorStatus gamedrawer::setCenter(AcGePoint3d& center) {
	assertWriteEnabled(false);
	AcDbDwgFiler *pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(gamedrawer::desc());
=======
Acad::ErrorStatus Gamedrawer::setStart(AcGePoint3d& start) {
	startPoint = start;
	return Acad::eOk;
}
AcGePoint3d Gamedrawer::getStartpoint() const {
	return startPoint;
}

Acad::ErrorStatus Gamedrawer::setCenter(AcGePoint3d& center) {
	assertWriteEnabled(false);
	AcDbDwgFiler *pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(Gamedrawer::desc());
>>>>>>> Stashed changes
		undoFiler()->writeItem((Adesk::Int16)kCenter);
		undoFiler()->writePoint3d(m_center);
	}
	m_center = center;
	return Acad::eOk;
}
<<<<<<< Updated upstream
AcGePoint3d gamedrawer::getCenter() const {
=======
AcGePoint3d Gamedrawer::getCenter() const {
>>>>>>> Stashed changes
	assertReadEnabled();
	return m_center;
}

<<<<<<< Updated upstream
Acad::ErrorStatus gamedrawer::setLength(double len) {
	assertWriteEnabled(false);
	AcDbDwgFiler *pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(gamedrawer::desc());
=======
Acad::ErrorStatus Gamedrawer::setLength(double len) {
	assertWriteEnabled(false);
	AcDbDwgFiler *pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(Gamedrawer::desc());
>>>>>>> Stashed changes
		undoFiler()->writeItem((Adesk::Int16)kLength);
		undoFiler()->writeDouble(length);
	}
	length = len;
	return Acad::eOk;
}
<<<<<<< Updated upstream
double gamedrawer::getLength() const {
=======
double Gamedrawer::getLength() const {
>>>>>>> Stashed changes
	assertReadEnabled();
	return length;
}

<<<<<<< Updated upstream
double gamedrawer::getGridsize() const {
	return gridSize;
}

Acad::ErrorStatus gamedrawer::setdataArray(std::array<std::array<int, COLUMN>, ROW>& data) {
	dataArray = data;
=======
double Gamedrawer::getGridsize() const {
	return gridSize;
}

Acad::ErrorStatus Gamedrawer::updataLocation()
{
	int num = 0;
	for (int i = 0; i < COLUMN; i++) {
		for (int j = 0; j < ROW; j++) {
			AcGePoint3d st;
			st.set(startPoint.x + j * m_len, startPoint.y + i * m_len, startPoint.z);
			rectangles[num].sPoint = st;
			num = num + 1;
		}
	}
	for (int i = 0; i < 16; i++) {
		int dir[][2] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };
		rectangles[i].m_points[0] = rectangles[i].sPoint;
		//acutPrintf(_T("\n rectangles[i].sPoint:%f,%f,%f"), rectangles[i].sPoint.x, rectangles[i].sPoint.y, rectangles[i].sPoint.z);
		AcGePoint3d startP = rectangles[i].m_points[0];
		for (int j = 0; j < rectangles[i].m_ptNum; j++) {
			rectangles[i].m_points[j].x = startP.x + m_len * dir[j][0];
			rectangles[i].m_points[j].y = startP.y + m_len * dir[j][1];
			rectangles[i].m_points[j].z = startP.z;
			//acutPrintf(_T("\nRecPoint:%f,%f,%f"), rectangles[i].m_points[j].x, rectangles[i].m_points[j].y, rectangles[i].m_points[j].z);
		}
	}
>>>>>>> Stashed changes
	return Acad::eOk;
}

//----------------------------------------------------------------------------
//----- Graphics protocols


<<<<<<< Updated upstream
Acad::ErrorStatus gamedrawer::subGetGripPoints(
=======
Acad::ErrorStatus Gamedrawer::subGetGripPoints(
>>>>>>> Stashed changes
	AcGePoint3dArray &gripPoints,
	AcDbIntArray &osnapModes,
	AcDbIntArray &geomIds) const
{
	assertReadEnabled();
	AcGePoint3d start;
	start.set(m_center.x - length / 2, m_center.y - length / 2, m_center.z);
	AcGePoint3d pZero, pOne, pTwo, pThree, center;
	//	3---2
	//	|	|
	//	0---1
	pZero = start;
	pOne.set(start.x + length, start.y, start.z);
	pTwo.set(start.x + length, start.y + length, start.z);
	pThree.set(start.x, start.y + length, start.z);
	center = m_center;
	gripPoints.append(pZero);
	gripPoints.append(pOne);
	gripPoints.append(pTwo);
	gripPoints.append(pThree);
	gripPoints.append(center);
	return Acad::eOk;
}

<<<<<<< Updated upstream
Acad::ErrorStatus gamedrawer::subMoveGripPointsAt(
=======
Acad::ErrorStatus Gamedrawer::subMoveGripPointsAt(
>>>>>>> Stashed changes
	const AcDbIntArray &indices,
	const AcGeVector3d &offset)
{
	assertWriteEnabled();
	AcGePoint3d start;
	start.set(m_center.x - length / 2, m_center.y - length / 2, m_center.z);
	setStart(start);
	AcGePoint3d movePoint, pZero, pOne, pTwo, pThree, center, temp;
	double temp1, temp2, temp3, temp4, len;
	switch (indices[0]) {
	case(0):
		pZero = startPoint;
		movePoint = pZero + offset;
		temp1 = movePoint.distanceTo(m_center);
		len = temp1 * std::sqrt(2.0);
		setLength(len);
		break;
	case(1):
		pOne.set(startPoint.x + length, startPoint.y, startPoint.z);
		movePoint = pOne + offset;
		temp1 = movePoint.distanceTo(m_center);
		len = temp1 * std::sqrt(2.0);
		setLength(len);
		break;
	case(2):
		pTwo.set(startPoint.x + length, startPoint.y + length, startPoint.z);
		movePoint = pTwo + offset;
		temp1 = movePoint.distanceTo(m_center);
		len = temp1 * std::sqrt(2.0);
		setLength(len);
		break;
	case(3):
		pThree.set(startPoint.x, startPoint.y + length, startPoint.z);
		movePoint = pThree + offset;
		temp1 = movePoint.distanceTo(m_center);
		len = temp1 * std::sqrt(2.0);
		setLength(len);
		break;
	case(4):
		temp = m_center + offset;
		setCenter(temp);
		break;
	}
	
	return Acad::eOk;
}

<<<<<<< Updated upstream



Adesk::Boolean gamedrawer::subWorldDraw(AcGiWorldDraw * mode) {
=======
Adesk::Boolean Gamedrawer::subWorldDraw(AcGiWorldDraw * mode) {
>>>>>>> Stashed changes
	assertReadEnabled();

	if (mode == nullptr) {
		return Adesk::kFalse;
	}
	
	mode->subEntityTraits().setColor(1);  // set color
	mode->subEntityTraits().setLineWeight(AcDb::kLnWtByLayer);  // set Line weight
	double samllLength = length / gridSize;
	// draw
	AcGePoint3d start;
	start.set(m_center.x - length / 2, m_center.y - length / 2, m_center.z);
	setStart(start);
<<<<<<< Updated upstream
=======
	updataLocation();
	initText();
>>>>>>> Stashed changes
	for (int i = 0; i <= gridSize; ++i) {
		// vertical line
		AcGePoint3d startP(startPoint.x + i * samllLength, startPoint.y, 0.0);
		AcGePoint3d endP(startPoint.x + i * samllLength, startPoint.y + length, 0.0);
		mode->geometry().polyline(2, new AcGePoint3d[2]{ startP ,endP });
		// horizon
		AcGePoint3d startP2(startPoint.x , startPoint.y + i * samllLength, 0.0);
		AcGePoint3d endP2(startPoint.x + length, startPoint.y + i * samllLength, 0.0);
		mode->geometry().polyline(2, new AcGePoint3d[2]{ startP2 ,endP2 });
	}

	return (AcDbEntity::subWorldDraw(mode));
}

<<<<<<< Updated upstream
Adesk::UInt32 gamedrawer::subSetAttributes(AcGiDrawableTraits * traits) {
=======
Adesk::UInt32 Gamedrawer::subSetAttributes(AcGiDrawableTraits * traits) {
>>>>>>> Stashed changes
	assertReadEnabled();
	return (AcDbEntity::subSetAttributes(traits));
}

<<<<<<< Updated upstream
=======
void Gamedrawer::updataArray(std::array<std::array<int, ROW>, COLUMN> dataArray) //���������µ�������
{
	for (int i = 0; i < 16; i++) {
		rectangles[i].m_realNum = dataArray[3 - i / gridSize][i % gridSize];
	}
	initText();
	recordGraphicsModified();
}

void Gamedrawer::initText() {
	for (int i = 0; i < 16; i++) {
		std::string str;
		if (rectangles[i].m_realNum == 0) {
			str = " ";
		}
		else {
			str = std::to_string(rectangles[i].m_realNum); // ��intת��Ϊ�ַ���
		}
		
	// ʹ��strcpy()���и��ƣ������Unicode�ַ�����ʹ��wcscpy��
		rectangles[i].pMsg = new TCHAR[str.length() + 1];
#ifdef _UNICODE
		std::wstring wstr(str.begin(), str.end()); // ��std::stringת��Ϊstd::wstring
		wcsncpy(rectangles[i].pMsg, wstr.c_str(), str.length());      // ʹ��wcsncpy��std::wstring���Ƶ�TCHAR*
		rectangles[i].pMsg[wstr.length()] = L'\0'; // ����ַ���������
#else	
		strncpy(pMsg, str.c_str(), msgSize);       // ʹ��strncpy��std::string���Ƶ�TCHAR*
		pMsg[str.length()] = '\0'; // ����ַ���������
#endif
		double width = (m_len / 4) * 0.6638 * str.length();//
		rectangles[i].position = AcGePoint3d(rectangles[i].m_points[0].x + (m_len / 2 - width / 2), rectangles[i].m_points[0].y + (m_len * 3 / 8), 0.0);
		rectangles[i].normal = AcGeVector3d(0.0, 0.0, 1.0);
		rectangles[i].direction = AcGeVector3d(1.0, 0.0, 0.0);
		rectangles[i].m_length = _tcslen(rectangles[i].pMsg);
		if (rectangles[i].pMsg == nullptr) {
			throw std::invalid_argument("msg cannot be null");
		}
		rectangles[i].raw = Adesk::kFalse;
		rectangles[i].textStyle.setTextSize(m_len / 4);
		//acutPrintf(_T("\ninitText"));
	}
}

//void gamedrawer::moveBlock(AcGePoint3d stPoint, int size) //�ƶ������̷�����С�ĸı䣩�����õĺ���
//{
//	for (int i = 0; i < 16; i++) {
//		m_len = size;  //���´�С
//		//updataArray();  //���µ������
//		initText();  //�����ı���λ��
//	}
//	
//}
//
//void gamedrawer::updateBlock() //���ʹ��WSADʱ���õĺ���
//{
//	for (int i = 0; i < 16; i++) {
//		rectangles[i].m_realNum = 0;  //������ʵֵ
//		initText();  //�����ı���λ��
//	}
//	
//}
>>>>>>> Stashed changes
