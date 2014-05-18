#ifndef ___INANITY_MATH_EIGEN_HPP___
#define ___INANITY_MATH_EIGEN_HPP___

/* Файл, включающий поддержку Eigen.

Стоит заметить, что Eigen не следует использовать где попало,
можно набрать себе много проблем с выравниванием, см. эпичную
страницу из документации:
http://eigen.tuxfamily.org/dox/TopicUnalignedArrayAssert.html

Поэтому везде, где нужно сохранять или передавать векторы/матрицы,
используем лёгкую математику (basic.hpp). Используем Eigen только там,
где производим вычисления, создаём объекты только на стеке,
не передаём их никуда (из/в функцию) и не сохраняем, полученный
результат преобразуем в легкую математику.
*/

#include "math.hpp"

#define EIGEN_MPL2_ONLY
#include "../deps/eigen/Eigen/Core"
#include "../deps/eigen/Eigen/Geometry"
#include "../deps/eigen/Eigen/LU"

BEGIN_INANITY_MATH

//******* Методы преобразования типов.

template <typename T, int n>
inline Eigen::Matrix<T, n, 1> toEigen(const xvec<T, n>& a)
{
	Eigen::Matrix<T, n, 1> b;
	for(int i = 0; i < n; ++i)
		b(i) = a(i);
	return b;
}

template <typename T, int n, int m>
inline Eigen::Matrix<T, n, m> toEigen(const xmat<T, n, m>& a)
{
	Eigen::Matrix<T, n, m> b;
	for(int j = 0; j < m; ++j)
		for(int i = 0; i < n; ++i)
			b(i, j) = a(i, j);
	return b;
}

template <typename T>
inline Eigen::Quaternion<T> toEigenQuat(const xquat<T>& a)
{
	return Eigen::Quaternion<T>(a.w, a.x, a.y, a.z);
}

template <typename T, int n>
inline xvec<T, n> fromEigen(const Eigen::Matrix<T, n, 1>& a)
{
	xvec<T, n> b;
	for(int i = 0; i < n; ++i)
		b(i) = a(i);
	return b;
}

template <typename T, int n, int m>
inline xmat<T, n, m> fromEigen(const Eigen::Matrix<T, n, m>& a)
{
	xmat<T, n, m> b;
	for(int j = 0; j < m; ++j)
		for(int i = 0; i < n; ++i)
			b(i, j) = a(i, j);
	return b;
}

template <typename T>
inline xquat<T> fromEigen(const Eigen::Quaternion<T>& a)
{
	return xquat<T>(a.x(), a.y(), a.z(), a.w());
}

END_INANITY_MATH

#endif
