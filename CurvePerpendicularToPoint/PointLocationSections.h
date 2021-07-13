#pragma once
#include "PointLocation.h"
using namespace CMMDataAnalysisCommon::PointFiles;
using namespace System::Collections::Generic;
namespace CurvePerpendicualToPoint
{
	private ref class PointLocationSection
	{
	private:
		String^ m_sectionName = nullptr;
		List<PointLocation^>^ m_points = gcnew List<PointLocation^>();


	public:
		property String^ SectionName {String^ get() { return m_sectionName; }}
		property List<PointLocation^>^ Points {List<PointLocation^>^ get() { return m_points; }}


	public:
		PointLocationSection(String^ name, List<PointLocation^>^ points);
		static bool ParseFromLines(List<CMMDataAnalysisCommon::PointFiles::FileLine^>^ sectionLines, PointLocationSection^% output, String^% error);
		static bool GetPointInfo(array<String^>^ splitLine, String^% pointName, EigenNet::Vector3d^% location, int lineNumber, String^% error);
	};
}

