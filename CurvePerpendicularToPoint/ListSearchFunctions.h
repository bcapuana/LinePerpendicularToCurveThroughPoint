#pragma once
#include "Section.h"
#include "PointLocation.h"
using namespace CMMDataAnalysisCommon::PointFiles;

public value class FindSectionByName
{
	System::String^ m_sectionName;
public:
	FindSectionByName(System::String^ name) { m_sectionName = name; }
	bool IsMatch(Section^ section) {
		return section->SectionName == m_sectionName;
	}
};


public value class FindLineStartingWith {
	System::String^ m_searchString;
public:
	FindLineStartingWith(System::String^ searchString) { m_searchString = searchString; }
	bool IsMatch(FileLine^ line) { return line->Text->StartsWith(m_searchString); }
};


public value class FindLinesInRange {
private:
	int m_start;
	int m_end;

public:
	FindLinesInRange(int start, int end) { m_start = start; m_end = end; }
	bool IsMatch(FileLine^ line) { return line->LineNumber >= m_start && line->LineNumber <= m_end; }

};

public value class FindPointByName 
{
private:
	String^ m_name;
public:
	FindPointByName(System::String^ searchString) { m_name = searchString; }
	bool IsMatch(CurvePerpendicualToPoint::PointLocation^ point) { return point->Name == m_name; }
};