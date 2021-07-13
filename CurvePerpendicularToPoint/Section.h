#pragma once
using CMMDataAnalysisCommon::PointFiles::MLEADA_PointFile::MLEADA_Section;
#include "pch.h"

private ref class Section : public MLEADA_Section
{

internal:
	NCollection_Haft<Handle(Geom_BSplineCurve)> m_insideCurve;
	NCollection_Haft<Handle(Geom_BSplineCurve)> m_outsideCurve;
		                                         

public:


public:
	Section(MLEADA_Section^ input);


internal:
	bool CreateSectionCurves();

private:
	bool CreateSectionCurve(System::Collections::Generic::List<EigenNet::Vector3d^>^ points, Handle(Geom_BSplineCurve)& curve);
	

};

