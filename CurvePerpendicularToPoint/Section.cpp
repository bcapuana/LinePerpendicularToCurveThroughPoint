#include "pch.h"
#include "Section.h"

Section::Section(MLEADA_Section^ input)
{
	m_sectionName = input->SectionName;
	m_insidePoints = input->InsidePoints;
	m_outsidePoints = input->OutsidePoints;
}

bool Section::CreateSectionCurves()
{
	RemovePointsTooClose(0.0001);
	Handle(Geom_BSplineCurve) curve = nullptr;
	System::Console::WriteLine("{0} inside", SectionName);
	if (!CreateSectionCurve(InsidePoints, curve))
	{
		System::Console::WriteLine("Error creating inside curve");
		return false;
	}
	m_insideCurve() = curve;

	curve = nullptr;
	System::Console::WriteLine("{0} outside", SectionName);
	if (!CreateSectionCurve(OutsidePoints, curve))
	{
		System::Console::WriteLine("Error creating outside curve");
		return false;
	}

	m_outsideCurve() = curve;

	return true;

}

bool Section::CreateSectionCurve(System::Collections::Generic::List<EigenNet::Vector3d^>^ points, Handle(Geom_BSplineCurve)& curve)
{
	try {


		Handle(TColgp_HArray1OfPnt) gpPoints = new TColgp_HArray1OfPnt(1, points->Count);
		for (int i = 0; i < points->Count; i++)
			gpPoints->SetValue(i + 1, gp_Pnt((double)points[i]->x, (double)points[i]->y, (double)points[i]->z));

		/*for (int i = 0; i < points->Count; i++) {
			gp_Pnt pnt = gpPoints->Value(i + 1);
			System::Console::WriteLine("{0:F6},{1:F6},{2:F6}", pnt.X(), pnt.Y(), pnt.Z());
		}*/

		GeomAPI_Interpolate interpolate(gpPoints, Standard_False, 0.0001);
		interpolate.Perform();
		
		curve = interpolate.Curve();
		return true;
	}
	catch (std::exception ex)
	{
		System::Console::WriteLine("Error creating section curve for section {0}", SectionName);
		return false;
	}
}
