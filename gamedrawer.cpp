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

GameDrawer::GameDrawer(){
	dataArray = { 0 };
	length = 1000;
	startPoint = { 0,0,0 };
	gridSize = 4;
	m_center.set(500,500,0);
}

GameDrawer::GameDrawer(const std::array<std::array<int, COLUMN>, ROW>& data,const AcGePoint3d& center, const double len, const int grid) {
	dataArray = data;
	length = len;
	m_center = center;
	gridSize = grid;
	startPoint.set(m_center.x - length / 2, m_center.y - length / 2, m_center.z);
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
	return Acad::eOk;
}
double GameDrawer::getLength() const {
	assertReadEnabled();
	return length;
}

double GameDrawer::getGridsize() const {
	return gridSize;
}

Acad::ErrorStatus GameDrawer::setdataArray(std::array<std::array<int, COLUMN>, ROW>& data) {
	dataArray = data;
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

Adesk::UInt32 GameDrawer::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return (AcDbEntity::subSetAttributes(traits));
}

