#include "pch.h"
#include "Vector3d.h"

using namespace EigenNet;
Vector3d::Vector3d(const Eigen::Vector3d vector)
{
	delete m_vector;
	m_vector = new Eigen::Vector3d(vector);
}

EigenNet::Vector3d::Vector3d(Vector3d^ v)
{
	x = v->x;
	y = v->y;
	z = v->z;
}

EigenNet::Vector3d::Vector3d(const double x, const double y, const double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

EigenNet::Vector3d::Vector3d(array<double>^ pointArray)
{
	for (int i = 0; i < 3; i++)
		(*m_vector)[i] = pointArray[i];
}

EigenNet::Vector3d::Vector3d()
{
	for (int i = 0; i < 3; i++)
		(*m_vector)[i] = 0;
}

Vector3d^ EigenNet::Vector3d::operator+(Vector3d^ v1, Vector3d^ v2)
{
	return gcnew Vector3d(*v1->m_vector + *v2->m_vector);
}

Vector3d^ EigenNet::Vector3d::operator-(Vector3d^ v1, Vector3d^ v2)
{
	return v1 + -v2;
}

Vector3d^ EigenNet::Vector3d::operator-()
{
	return gcnew Vector3d(*(m_vector) * -1);
}

Vector3d^ EigenNet::Vector3d::operator/(Vector3d^ v1, double d)
{
	return v1 * (1.0 / d);
}

Vector3d^ EigenNet::Vector3d::operator*(Vector3d^ v1, double d)
{
	return gcnew Vector3d(*v1->m_vector * d);
}

Vector3d^ EigenNet::Vector3d::operator*(double d, Vector3d^ v1)
{
	return v1 * d;
}

double EigenNet::Vector3d::Dot(Vector3d^ v2)
{
	return m_vector->dot(*v2->m_vector);
}

Vector3d^ EigenNet::Vector3d::Cross(Vector3d^ v2)
{
	return gcnew Vector3d(m_vector->cross(*v2->m_vector));
}

void EigenNet::Vector3d::Normalize()
{
	m_vector->normalize();
}

Vector3d^ EigenNet::Vector3d::Normalized()
{
	return gcnew Vector3d(m_vector->normalized());
}


System::String^ Vector3d::ToString()
{
	return System::String::Format("{0}, {1}, {2}", x, y, z);
}

Vector3d::~Vector3d()
{
	this->!Vector3d();
}

Vector3d::!Vector3d()
{
	delete m_vector;
}
