#include "pch.h"
#include "ListSearchFunctions.h"
using namespace System;
using namespace CMMDataAnalysisCommon::PointFiles::MLEADA_PointFile;
using namespace System::Threading::Tasks;
using namespace System::Collections::Generic;


#include <GeomAPI_Interpolate.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <vector>
#include "Section.h"
#include "PointLocations.h"
int main(array<System::String^>^ args);
bool CreateSectionCurves(CMMDataAnalysisCommon::PointFiles::MLEADA_PointFile::MLEADA_PointFile^ file, List<Section^>^ output);
bool CreatePerpendicularLines(System::IO::StreamWriter^ sw, List<Section^>^ nominalSections, List<Section^>^ measuredSections, CurvePerpendicualToPoint::PointLocations^ points, String^% error);
bool GetLineParams(Section^ section, Vector3d^ location, Vector3d^% xyz, Vector3d^% ijk, Vector3d^ planeVector, String^% error);
bool GetPlaneVector(List<Vector3d^>^ points, Vector3d^% planeVector);

/// <summary>
/// Main entry point
/// </summary>
/// <param name="args">The command line arguments</param>
/// <returns></returns>
int main(array<System::String^>^ args)
{
	String^ exeLoc = System::IO::Path::GetDirectoryName(System::Reflection::Assembly::GetExecutingAssembly()->Location);
	Console::WriteLine("LinePerpendicularToCurveAtPoint.exe");
	{

		System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(exeLoc + "\\License.txt");
		Console::WriteLine(sr->ReadToEnd());
		Console::WriteLine();
		sr->Close();
	}

	if (args->Length < 4) {
		Console::WriteLine("Not enough arguments have been entered. Arguments are:");
		Console::WriteLine("1: Nominal Curve File Name");
		Console::WriteLine("2: Measured Curves File Name");
		Console::WriteLine("3: Point Locations File Name");
		Console::WriteLine("4: Output File Name");
		Console::WriteLine();
		Console::WriteLine("Press any key to continue...");
		Console::ReadKey();
		return 1;
	}

	// Read the arguments
 	String^ nominalFileName = args[0];
	String^ measuredFileName = args[1];
	String^ nominalLocations = args[2];
	String^ outputFile = args[3];

	// read the files, sorry about my spelling, i'm not fixing it right now.
	CMMDataAnalysisCommon::PointFiles::MLEADA_PointFile::MLEADA_PointFile^ nominalFile;
	CMMDataAnalysisCommon::PointFiles::MLEADA_PointFile::MLEADA_PointFile^ measuredFile;
	CurvePerpendicualToPoint::PointLocations^ locations;
	String^ error = nullptr;

	Console::WriteLine("Reading Nominal File...");
	if (!CMMDataAnalysisCommon::PointFiles::MLEADA_PointFile::MLEADA_PointFile::Open(nominalFileName, nominalFile, error))
	{
		Console::WriteLine(error);
		return 1;
	}

	Console::WriteLine("Reading Reading Measured file...");
	if (!CMMDataAnalysisCommon::PointFiles::MLEADA_PointFile::MLEADA_PointFile::Open(measuredFileName, measuredFile, error))
	{
		Console::WriteLine(error);
		return 1;
	}

	Console::WriteLine("Reading location file...");
	if (!CurvePerpendicualToPoint::PointLocations::Open(nominalLocations, locations, error))
	{
		Console::WriteLine(error);
		return 1;
	}


	// Create the section curves
	List<Section^>^ nominalSections = gcnew List<Section^>();
	List<Section^>^ measuredSections = gcnew List<Section^>();


	Console::WriteLine("Creating Section Curves...");
	if (!CreateSectionCurves(nominalFile, nominalSections)) return false;
	if (!CreateSectionCurves(measuredFile, measuredSections)) return false;


	// Write generate the perpendicular lines and write to dmi file.
	{ // equivilent of c# using(StreamReader ... )
		System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(outputFile);

		// DMIS Header
		sw->WriteLine("DMISMD/'PerpendicularPoints'");

		// Create the lines
		if (!CreatePerpendicularLines(sw, nominalSections, measuredSections, locations, error)) {
			Console::WriteLine(error);
			return 1;
		}
		
		// End of file so modus program can continue.
		sw->WriteLine("ENDFIL");
		sw->Flush();
		sw->Close();
	}

	// normal exit.
	return 0;
}

/// <summary>
/// Create Section Curves.
/// </summary>
/// <param name="file">The file containing the section curves.</param>
/// <param name="output">The list of sections with containing the OCCT BSplines</param>
/// <returns>True if successful, false otherwise.</returns>
bool CreateSectionCurves(CMMDataAnalysisCommon::PointFiles::MLEADA_PointFile::MLEADA_PointFile^ file, List<Section^>^ output)
{
	for each (MLEADA_Section ^ section in file->Sections)
	{
		Section^ s = gcnew Section(section);
		if (!s->CreateSectionCurves())
		{
			output->Clear();
			return false;
		}
		output->Add(s);
	}

	return true;
}

/// <summary>
/// Create the perpendicular lines and write them to the file.
/// </summary>
/// <param name="sw">The streamwriter to write the lines to.</param>
/// <param name="nominalSections">The nominal sections</param>
/// <param name="measuredSections">The measured sections</param>
/// <param name="points">The file containing the points through which the lines should go.</param>
/// <param name="error">any resulting errors</param>
/// <returns>True if successful, false otherwise.</returns>
bool CreatePerpendicularLines(System::IO::StreamWriter^ sw, List<Section^>^ nominalSections, List<Section^>^ measuredSections, CurvePerpendicualToPoint::PointLocations^ points, String^% error)
{
	// loop through the sections in the points file.
	for each (CurvePerpendicualToPoint::PointLocationSection ^ sec in points)
	{
		// find the matching nominal section curve
		Section^ nominalSection = nominalSections->Find(gcnew Predicate<Section^>(gcnew FindSectionByName(sec->SectionName), &FindSectionByName::IsMatch));

		// was the section found?
		if (nominalSection == nullptr)
		{
			// no, look for it again using the AA-AA format.
			nominalSection = nominalSections->Find(gcnew Predicate<Section^>(gcnew FindSectionByName(sec->SectionName + "-" + sec->SectionName), &FindSectionByName::IsMatch));

			// was the section found?
			if (nominalSection == nullptr)
			{
				//no, return error.
				Console::WriteLine("Could not find nominal section {0}", sec->SectionName);
				return false;
			}
		}

		// find the matching measured section curve
		Section^ measuredSection = measuredSections->Find(gcnew Predicate<Section^>(gcnew FindSectionByName(sec->SectionName), &FindSectionByName::IsMatch));
		// was the section found?
		if (measuredSection == nullptr)
		{
			// no, look for it again using the AA-AA format.
			measuredSection = measuredSections->Find(gcnew Predicate<Section^>(gcnew FindSectionByName(sec->SectionName + "-" + sec->SectionName), &FindSectionByName::IsMatch));
			// was the section found?
			if (measuredSection == nullptr)
			{
				// no, return error
				Console::WriteLine("Could not find measured section {0}", sec->SectionName);
				return false;
			}
		}

		// get the plane vectors
		Vector3d^ nominalPlaneVector, ^ measuredPlaneVector;
		GetPlaneVector(nominalSection->OutsidePoints, nominalPlaneVector);
		GetPlaneVector(measuredSection->OutsidePoints, measuredPlaneVector);
		
		// loop through each point and create the line
		for each (CurvePerpendicualToPoint::PointLocation ^ p in sec->Points)
		{
			// get the point name
			String^ pointName = p->Name;

			// get the point index
			int pointIndex = Convert::ToDouble(p->Name->Substring(p->Name->LastIndexOf("_") + 1));
			Vector3d^ xyz, ^ ijk;

			// create the feature name
			String^ featureName = String::Format("{0}_LN_PERP_{1}", sec->SectionName, (pointIndex < 10 ? "0" + pointIndex : pointIndex.ToString()));

			// find the nominal line and write to file
			GetLineParams(nominalSection, p->NominalLocation, xyz, ijk, nominalPlaneVector, error);
			sw->WriteLine("F({0})=FEAT/LINE,UNBND,CART,{1},{2},{3}", featureName, xyz, ijk, nominalPlaneVector);

			// find the measured line and write to file
			GetLineParams(measuredSection, p->MeasuredLocation, xyz, ijk, measuredPlaneVector, error);
			sw->WriteLine("FA({0})=FEAT/LINE,UNBND,CART,{1},{2},{3}", featureName, xyz, ijk, measuredPlaneVector);
		}
		
	}
	return true;
}

/// <summary>
/// Calculates the plane vector from 3 points
/// </summary>
/// <param name="points">The curve points.</param>
/// <param name="planeVector">The resulting plane vector</param>
/// <returns>True</returns>
bool GetPlaneVector(List<Vector3d^>^ points, Vector3d^% planeVector)
{
	Vector3d^ p1 = points[0];
	Vector3d^ p2 = points[points->Count / 2];
	Vector3d^ p3 = points[points->Count - 1];

	Vector3d^ v1 = (p1 - p2)->Normalized();
	Vector3d^ v2 = (p3 - p2)->Normalized();

	planeVector = v1->Cross(v2)->Normalized();
	return true;
}

bool GetLineParams(Section^ section, Vector3d^ location, Vector3d^% xyz, Vector3d^% ijk, Vector3d^ planeVector, String^% error)
{
	Handle(Geom_BSplineCurve) outsideCurve = section->m_outsideCurve();
	
	GeomAPI_ProjectPointOnCurve ppc(gp_Pnt(location->x, location->y, location->z), outsideCurve);

	double closestParam = ppc.LowerDistanceParameter();
	double distance = ppc.LowerDistance();

	gp_Pnt firstPoint, lastPoint, closestPoint;
	gp_Vec firstVector, lastVector, closestVector;
	
	outsideCurve->D1(outsideCurve->FirstParameter(), firstPoint, firstVector);
	outsideCurve->D1(closestParam, closestPoint, closestVector);
	outsideCurve->D1(outsideCurve->LastParameter(), lastPoint, lastVector);


	gp_Pnt output;
	gp_Vec outputVec;

	double d1 = (gcnew Vector3d(firstPoint.X(), firstPoint.Y(), firstPoint.Z()) - location)->VectorLength;
	double d2 = (gcnew Vector3d(closestPoint.X(), closestPoint.Y(), closestPoint.Z()) - location)->VectorLength;
	double d3 = (gcnew Vector3d(lastPoint.X(), lastPoint.Y(), lastPoint.Z()) - location)->VectorLength;

	if (d1 < d2 && d1 < d3) 
	{
		output = firstPoint;
		outputVec = firstVector;
	}

	else if (d2 < d1 && d2 < d3)
	{
		output = closestPoint;
		outputVec = closestVector;
	}

	else
	{
		output = lastPoint;
		outputVec = lastVector;
	}

	xyz = gcnew Vector3d(output.X(), output.Y(), output.Z());
	ijk = planeVector->Cross(gcnew Vector3d(outputVec.X(), outputVec.Y(), outputVec.Z()))->Normalized();

	if (planeVector->Dot(ijk) < 0)
		ijk = -ijk;

	return true;
}

