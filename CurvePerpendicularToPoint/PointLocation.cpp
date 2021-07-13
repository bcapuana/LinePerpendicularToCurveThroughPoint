#include "pch.h"
#include "PointLocation.h"

CurvePerpendicualToPoint::PointLocation::PointLocation(String^ name, Vector3d^ nominalLocation, Vector3d^ measuredLocation)
{
	m_name = name;
	m_nominalLocation = nominalLocation;
	m_measuredLocation = measuredLocation;
}
