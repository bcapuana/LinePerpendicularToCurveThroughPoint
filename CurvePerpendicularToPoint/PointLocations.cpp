#include "pch.h"
#include "PointLocations.h"
#include "ListSearchFunctions.h"
using namespace CMMDataAnalysisCommon::PointFiles;
using namespace System::Collections::Generic;
using namespace System::IO;
bool CurvePerpendicualToPoint::PointLocations::Open(String^ fileName, PointLocations^% output, String^% error)
{
	output = nullptr;
	try
	{
		if (!File::Exists(fileName)) {
			error = "File does not exist";
			return false;
		}

		List<FileLine^>^ fileLines = gcnew List<FileLine^>();

		{// this is how you do a c# using in c++/cli
			StreamReader^ sr = gcnew StreamReader(fileName);
			int lineNumber = 0;

			while (!sr->EndOfStream)
			{
				lineNumber++;
				String^ line = sr->ReadLine()->Trim();
				String^ upperLine = line->ToUpper();
				if ((upperLine->StartsWith("SECTION") || upperLine->StartsWith("FA(") || upperLine->StartsWith("F(")) && !String::IsNullOrEmpty(line) && !String::IsNullOrWhiteSpace(line))
					fileLines->Add(gcnew FileLine(lineNumber, line));
			}

			sr->Close();
		}

		List<FileLine^>^ sectionStarts = fileLines->FindAll(gcnew System::Predicate<FileLine^>(gcnew FindLineStartingWith("SECTION"), &FindLineStartingWith::IsMatch));
		output = gcnew PointLocations();
		for (int i = 0; i < sectionStarts->Count; i++)
		{
			int start = sectionStarts[i]->LineNumber;
			int end = i < sectionStarts->Count - 1 ? sectionStarts[i + 1]->LineNumber - 1 : fileLines[fileLines->Count - 1]->LineNumber;
			List<FileLine^>^ sectionLines = fileLines->FindAll(gcnew System::Predicate<FileLine^>(gcnew FindLinesInRange(start, end), &FindLinesInRange::IsMatch));
			PointLocationSection^ s;
			if (!PointLocationSection::ParseFromLines(sectionLines, s, error))
			{
				output = nullptr;
				return false;
			}
			output->Add(s);
		}

		error = nullptr;
		return true;
	}
	catch (System::Exception^ ex)
	{
		output = nullptr;
		error = String::Format("Error parsing file: {0}", ex->Message);
		return false;
	}



}
