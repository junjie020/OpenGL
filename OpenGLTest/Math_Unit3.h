#ifndef _MATH_Unit3f_H_
#define _MATH_Unit3f_H_

#include <cmath>

namespace OGLKit
{
	template<class Type>
	class Unit3
	{
	public:

		typedef Type		value_type;
		typedef Type*		pointer;

		typedef const Type*	const_pointer;
		Unit3(){}
		Unit3(value_type defaultVal) 
		{
			x = y = z = defaultVal;
		}

		Unit3(Type xx, Type yy, Type zz)
		: x(xx), y(yy), z(zz)
		{}

		const_pointer GetData() const { return &x; }

		pointer GetData() { return &x; }

		//const_pointer GetData() { return &x; }

		Unit3<Type> operator-(const Unit3<Type> &rhs)
		{
			return Unit3f(x - rhs.x, y - rhs.y, z - rhs.z);
		}

		Unit3<Type>& operator=(const Unit3<Type> &rhs)
		{
			memcpy(&x, &rhs.x, sizeof(Type) * 3);
			return *this;
		}

		Unit3<Type> operator/(Type factor) const
		{
			return Unit3<Type>(x/factor, y/factor, z/factor);
		}

		Unit3<Type> operator*(Type factor) const
		{
			return Unit3<Type>(x*factor, y*factor, z*factor);
		}

		Unit3<Type> Dot(const Unit3<Type> &rhs);

		Type LengthSquare() const { return (x*x + y*y + z*z); }
		Type Length() const { return std::sqrt(LengthSquare()); }

		Unit3<Type> Unit() const
		{
			return this->operator /(Length());
		}

		Unit3<Type> Cross(const Unit3<Type> &rhs)
		{
			return Unit3<Type>
				( y*rhs.z - z*rhs.y,
				z*rhs.x - x*rhs.z,
				x*rhs.y - y*rhs.x );
		}

		Unit3<Type> UnitCross(const Unit3<Type> &rhs)
		{
			return Cross(rhs).Unit();
		}

		void Normalize()
		{
			const Type len = Length();
			x /= len;
			y /= len;
			z /= len;
		}

	public:
		operator value_type*() { return &x;}
		operator const value_type*() const { return &x; }
	public:
		Type x, y, z;
	};

	typedef Unit3<float> Unit3f;
	typedef Unit3f Point3D;
	typedef std::vector<Point3D>	Point3DArray;
	typedef std::list<Point3D>		Point3DList;

	typedef Unit3f Vector3D;
	typedef std::vector<Vector3D>	Vector3DArray;

	class Color3D
	{
		typedef Color3D* pointer;
	public:
		Color3D(BYTE rr, BYTE gg, BYTE bb, BYTE aa): r(rr), g(gg), b(bb), a(aa){}
		Color3D(BYTE rr, BYTE gg, BYTE bb) : r(rr), g(gg), b(bb), a(255){}
		Color3D(){}
		BYTE r, g, b, a;

		bool operator==(const Color3D &rhs) { return 0 == memcmp(*this, rhs, sizeof(Color3D));}

		operator pointer(){ return this; }
		operator const Color3D*() const { return this; }

		static Color3D White;
		static Color3D Black;

		static Color3D Red;
		static Color3D Green;
		static Color3D Blue;

		static Color3D None;

	};

	typedef std::vector<Color3D>	Color3DArray;
}

#endif	//_MATH_Unit3f_H_