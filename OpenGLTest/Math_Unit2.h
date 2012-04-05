#ifndef	_MATH_UNIT2_H_
#define _MATH_UNIT2_H_

#include <cmath>

namespace OGLKit
{
	template<class TYPE>
	class Unit2
	{
		typedef TYPE value_type;
		typedef TYPE* pointer;
	public:
		Unit2(){}
		Unit2(const value_type &_x, const value_type &_y) : x(_x), y(_y){}

		operator pointer()
		{
			OGL_ASSERT(reinterpret_cast<void*>(this) == reinterpret_cast<void*>(&x));
			return this;
		}

	public:
		value_type x, y;
	};

	typedef Unit2<float>			Point2D;
	typedef std::vector<Point2D>	Point2DArray;

	template<class TYPE>
	class RectT
	{
		typedef TYPE value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
	public:
		//RectT(const Unit2<TYPE> &_lb, const Unit2<TYPE> &_rt) : lb(_lb), rt(_rt){}
		RectT(TYPE ll, TYPE bb, TYPE rr, TYPE tt) 
			: left(ll), bottom(bb), right(rr), top(tt)
		{}

		TYPE width() const { return (right - left);}
		TYPE height() const { return (top - bottom); }

		operator pointer() { return &left; }

		TYPE left, bottom, right, top;
	};

	typedef RectT<float>	Rect;
}

#endif	//_MATH_UNIT2_H_