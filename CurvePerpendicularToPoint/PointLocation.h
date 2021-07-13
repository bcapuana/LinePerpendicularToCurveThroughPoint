#pragma once
using namespace EigenNet;
namespace CurvePerpendicualToPoint
{
	ref class PointLocation
	{
	private:
		String^ m_name;
		Vector3d^ m_nominalLocation = nullptr;
		Vector3d^ m_measuredLocation = nullptr;

	public:

		property String^ Name {String^ get() { return m_name; }}
		property Vector3d^ NominalLocation
		{
			Vector3d^ get() { return m_nominalLocation; }
		internal:
			void set(Vector3d^ value) { m_nominalLocation = value; }
		}
	public:
		property Vector3d^ MeasuredLocation {
			Vector3d^ get() { return m_measuredLocation; }
	internal:
		void set(Vector3d^ value) { m_measuredLocation = value; }
		}

	public:
		PointLocation(String^ name, Vector3d^ nominalLocation, Vector3d^ measuredLocation);


	};
}

