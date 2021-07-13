#pragma once
#include <Dense>
namespace EigenNet {
	public ref class Vector3d
	{
	private:
		Eigen::Vector3d* m_vector = new Eigen::Vector3d();
		Vector3d(const Eigen::Vector3d vector);

	public:
		Vector3d(Vector3d^ v);
		Vector3d(const double x, const double y, const double z);
		Vector3d(array<double>^ pointArray);
		Vector3d();

		property double x {
			double get() { return m_vector->x(); }
			void set(double value) { (*m_vector)[0] = value; }
		}
		property double y {
			double get() { return m_vector->y(); }
			void set(double value) { (*m_vector)[1] = value; }
		}
		property double z {
			double get() { return m_vector->z(); }
			void set(double value) { (*m_vector)[2] = value; }
		}

		property const Eigen::Vector3d* Vector { const Eigen::Vector3d* get() { return m_vector; }}

		property double VectorLength {
			double get() { return m_vector->norm(); }
		}


		static Vector3d^ operator * (Vector3d^ v1, double d);
		static Vector3d^ operator * (double d, Vector3d^ v1);
		static Vector3d^ operator / (Vector3d^ v1, double d);
		static Vector3d^ operator + (Vector3d^ v1, Vector3d^ v2);
		static Vector3d^ operator - (Vector3d^ v1, Vector3d^ v2);
		Vector3d^ operator-();

		double Dot(Vector3d^ v2);
		Vector3d^ Cross(Vector3d^ v2);
		void Normalize();
		Vector3d^ Normalized();


		System::String^ ToString() override;
		~Vector3d();
		!Vector3d();

	};
}
