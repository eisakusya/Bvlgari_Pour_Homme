#include "pch.h"
#include "gamedrawer.h"
//----------------------------------------------------------------------------
Adesk::UInt32 GameDrawer::kCurrentVersionNumber = 1 ;

//----------------------------------------------------------------------------
//---- runtime definition
ACRX_DXF_DEFINE_MEMBERS (
	GameDrawer, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, GAMEDRAWER, GAMEDRAWERAPP
)

//----------------------------------------------------------------------------
//---- construct & destruct

GameDrawer::GameDrawer() {
	setLength(1000.0);
	startPoint = { 0,0,0 };
	gridSize = 4;
	m_center.set(500, 500, 0);
	m_len = length / gridSize;
	updataLocation();
	for (int i = 0; i < 16; i++) {
		rectangles[i].m_realNum = i;
	}
	//updataArray();
	initText();
}

GameDrawer::GameDrawer(int num, AcGePoint3d center, const double len, const int grid) {
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

GameDrawer::~GameDrawer(){}

//----------------------------------------------------------------------------
//----- AcDbObject protocols
//---- Dwg Filing protocol
Acad::ErrorStatus GameDrawer::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled ();
	Acad::ErrorStatus es = AcDbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	if ((es = pFiler->writeUInt32(GameDrawer::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....
	pFiler->writeDouble(length);
	//pFiler->writePoint3d(startPoint);
	pFiler->writePoint3d(m_center);

	return (pFiler->filerStatus());
}

Acad::ErrorStatus GameDrawer::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > GameDrawer::kCurrentVersionNumber)
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

Acad::ErrorStatus GameDrawer::dxfOutFields(AcDbDxfFiler *pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dxfOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	es = pFiler->writeItem(AcDb::kDxfSubclass, _T("SampleCustEnt"));
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(AcDb::kDxfInt32, GameDrawer::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	return (pFiler->filerStatus());
}

Acad::ErrorStatus GameDrawer::dxfInFields(AcDbDxfFiler *pFiler) {
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
	if (version > GameDrawer::kCurrentVersionNumber)
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


Acad::ErrorStatus GameDrawer::applyPartialUndo(AcDbDwgFiler* undoFiler, AcRxClass* classObj)
{
	if (classObj != GameDrawer::desc())
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

Acad::ErrorStatus GameDrawer::setStart(AcGePoint3d& start) {
	startPoint = start;
	return Acad::eOk;
}
AcGePoint3d GameDrawer::getStartpoint() const {
	return startPoint;
}

Acad::ErrorStatus GameDrawer::setCenter(AcGePoint3d& center) {
	assertWriteEnabled(false);
	AcDbDwgFiler *pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(GameDrawer::desc());
		undoFiler()->writeItem((Adesk::Int16)kCenter);
		undoFiler()->writePoint3d(m_center);
	}
	m_center = center;
	return Acad::eOk;
}
AcGePoint3d GameDrawer::getCenter() const {
	assertReadEnabled();
	return m_center;
}

Acad::ErrorStatus GameDrawer::setLength(double len) {
	assertWriteEnabled(false);
	AcDbDwgFiler *pFiler = NULL;
	if ((pFiler = undoFiler()) != NULL) {
		undoFiler()->writeAddress(GameDrawer::desc());
		undoFiler()->writeItem((Adesk::Int16)kLength);
		undoFiler()->writeDouble(length);
	}
	length = len;
	m_len = length / gridSize;
	return Acad::eOk;
}
double GameDrawer::getLength() const {
	assertReadEnabled();
	return length;
}

double GameDrawer::getGridsize() const {
	return gridSize;
}

Acad::ErrorStatus GameDrawer::updataLocation()
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
	return Acad::eOk;
}

//----------------------------------------------------------------------------
//----- Graphics protocols


Acad::ErrorStatus GameDrawer::subGetGripPoints(
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

Acad::ErrorStatus GameDrawer::subMoveGripPointsAt(
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

Adesk::Boolean GameDrawer::subWorldDraw(AcGiWorldDraw * mode) {
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
	updataLocation();
	initText();
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

	for (int i = 0; i < gridSize * gridSize; ++i) {
		

		// ����Hatch��������������
		AcDbHatch *pHatch = new AcDbHatch();
		pHatch->setDatabaseDefaults();
		pHatch->setAssociative(Adesk::kFalse);
		pHatch->setPatternScale(1.0);
		pHatch->setPatternAngle(0.0);
		pHatch->setPattern(AcDbHatch::kPreDefined, _T("SOLID"));
		// �������εı߽�
		AcGePoint2dArray vertices;
		double side = length / gridSize / 9;
		int dir[][2] = { {1, 1}, {-1,1 }, {-1, -1}, {1, -1} };
		for (int j = 0; j < 4; ++j) {
			
			vertices.append(AcGePoint2d(rectangles[i].m_points[j].x + dir[j][0] * side, rectangles[i].m_points[j].y  +dir[j][1] * side));
		}
		// �պ϶���α߽�
		vertices.append(AcGePoint2d(rectangles[i].m_points[0].x + dir[0][0] * side, rectangles[i].m_points[0].y + dir[0][1] * side));
		// ����һ���߽绷
		AcGeDoubleArray array;
		for (int j = 0; j < 4; j++) {
			array.append(0);
		}
		pHatch->appendLoop(AcDbHatch::kExternal, vertices, array);
		// ���¼����������
		pHatch->evaluateHatch();
		mode->subEntityTraits().setColor(rectangles[i].m_color); // ������ɫrectangles[i].m_color
		pHatch->worldDraw(mode);
		delete pHatch;



		//�߿������
		mode->subEntityTraits().setColor(255); // ������ɫΪ7����ɫ���ɫ��ȡ���ڱ�����
		mode->geometry().polygon(4, rectangles[i].m_points); //��һ�������ζ����

		mode->geometry().text(rectangles[i].position, rectangles[i].normal, rectangles[i].direction, rectangles[i].pMsg,
			rectangles[i].m_length, Adesk::kFalse, rectangles[i].textStyle);
	}



	return (AcDbEntity::subWorldDraw(mode));
}

Adesk::UInt32 GameDrawer::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return (AcDbEntity::subSetAttributes(traits));
}

void GameDrawer::updataArray(std::array<std::array<int, ROW>, COLUMN> dataArray) //���������µ�������
{
	for (int i = 0; i < 16; i++) {
		rectangles[i].m_realNum = dataArray[3 - i / gridSize][i % gridSize];
	}
	initText();
	recordGraphicsModified();
}

void GameDrawer::initText() {
	for (int i = 0; i < 16; i++) {
		//������ɫ
		//������ɫ
		switch (rectangles[i].m_realNum) {
		case 0:
			rectangles[i].m_color = color0;
			break;
		case 2:
			rectangles[i].m_color = color2;
			break;
		case 4:
			rectangles[i].m_color = color4;
			break;
		case 8:
			rectangles[i].m_color = color8;
			break;
		case 16:
			rectangles[i].m_color = color16;
			break;
		case 32:
			rectangles[i].m_color = color32;
			break;
		case 64:
			rectangles[i].m_color = color64;
			break;
		case 128:
			rectangles[i].m_color = color128;
			break;
		case 256:
			rectangles[i].m_color = color256;
			break;
		case 512:
			rectangles[i].m_color = color512;
			break;
		case 1024:
			rectangles[i].m_color = color1024;
			break;
		default:
			std::cerr << "Invalid m_realNum value: " << rectangles[i].m_realNum << "\n";
			continue;
		}
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