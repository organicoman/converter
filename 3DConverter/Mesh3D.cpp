#include "Mesh3D.h"
#include "Face.h"

template<typename T>
inline double conv::Mesh3D<T>::area() const
{
	double sum = 0.0;
	for (auto f : m_faceArr)
	{
		if (f.area() < 0)
			f.flipNormal();
		sum += f.area();
	}
	return sum;
}