#include "pch.h"
#include "PointLocationSections.h"
#include "ListSearchFunctions.h"
using namespace System::Text::RegularExpressions;
CurvePerpendicualToPoint::PointLocationSection::PointLocationSection(String^ name, List<PointLocation^>^ points)
{
	m_sectionName = name;
	m_points = points;
}

bool CurvePerpendicualToPoint::PointLocationSection::ParseFromLines(List<CMMDataAnalysisCommon::PointFiles::FileLine^>^ sectionLines, PointLocationSection^% output, String^% error)
{
	if (sectionLines->Count == 0)
	{
		output = nullptr;
		error = "Section does not contain any data";
		return false;
	}
	int lineNumber = sectionLines[0]->LineNumber;
	try {

		String^ sectionName = sectionLines[0]->Text->Split(gcnew array<wchar_t>{' '})[1];
		List<PointLocation^>^ points = gcnew List<PointLocation^>();
		for (int i = 1; i < sectionLines->Count; i++)
		{

			String^ line = sectionLines[i]->Text;
			int lineNumber = sectionLines[i]->LineNumber;

			array<String^>^ splitLine = line->Split(gcnew array<wchar_t>{',', '=', '/'});

			// is it a nominal feature?
			if (Regex::IsMatch(splitLine[0], "^F\\(.*\\)"))
			{
				// yes
				String^ pointName;
				EigenNet::Vector3d^ location;
				if (!GetPointInfo(splitLine, pointName, location, lineNumber, error))
				{
					output = nullptr;
					return false;
				}

				PointLocation^ existingPoint = points->Find(gcnew System::Predicate<PointLocation^>(gcnew FindPointByName(pointName), &FindPointByName::IsMatch));
				if (existingPoint == nullptr)
					points->Add(gcnew PointLocation(pointName, location, nullptr));
				else
					existingPoint->NominalLocation = location;
			}
			else
			{
				// no
				String^ pointName;
				EigenNet::Vector3d^ location;
				if (!GetPointInfo(splitLine, pointName, location, lineNumber, error))
				{
					output = nullptr;
					return false;
				}

				PointLocation^ existingPoint = points->Find(gcnew System::Predicate<PointLocation^>(gcnew FindPointByName(pointName), &FindPointByName::IsMatch));
				if (existingPoint == nullptr)
					points->Add(gcnew PointLocation(pointName, nullptr, location));
				else
					existingPoint->MeasuredLocation = location;
			}
		}

		output = gcnew PointLocationSection(sectionName, points);
		error = nullptr;
		return true;
	}
	catch (Exception^ ex)
	{
		output = nullptr;
		error = String::Format("Error parsing file on line {0}: {1}", lineNumber, ex->Message);
	}
}

bool CurvePerpendicualToPoint::PointLocationSection::GetPointInfo(array<String^>^ splitLine, String^% pointName, EigenNet::Vector3d^% location, int lineNumber, String^% error)
{
	try {
		pointName = splitLine[0]->Substring(splitLine[0]->IndexOf("("))->Replace(")", String::Empty);
		array<double>^ values = gcnew array<double>(3);
		for (int i = 0; i < 3; i++)
			values[i] = Convert::ToDouble(splitLine[i + 4]);

		location = gcnew EigenNet::Vector3d(values);
		return true;
	}
	catch (Exception^ ex)
	{
		location = nullptr;
		pointName = nullptr;
		error = String::Format("Error parsing line {0}: {1}", lineNumber, ex->Message);
		return false;
	}


}
