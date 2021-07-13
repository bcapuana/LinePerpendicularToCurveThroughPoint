#pragma once
#include "PointLocationSections.h"

using namespace System::Collections::Generic;

namespace CurvePerpendicualToPoint
{
	private ref class PointLocations:public List<PointLocationSection^>
	{
	

	public:
		static bool Open(String^ fileName, PointLocations^% output, String^% error);



		
	};
}

