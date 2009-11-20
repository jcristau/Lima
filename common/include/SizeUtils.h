#ifndef SIZEUTILS_H
#define SIZEUTILS_H

#include "Constants.h"
#include "Exceptions.h"
#include <ostream>

namespace lima
{


/*******************************************************************
 * \fn IsPowerOf2(x)
 * \brief Calculates if a given number is a power of 2
 *
 * This very nice implementation was taken from David Fernandez
 *******************************************************************/

inline bool IsPowerOf2(int x)
{
	return (x > 0) && (((x - 1) & x) == 0);
}


/*******************************************************************
 * \class Point
 * \brief Basic two-dimension arithmetic class
 *
 * This utility class provides basic arithmetic in two dimensions
 *******************************************************************/

class Point
{
 public:
	int x, y;

	Point()               : x(0),   y(0)   {}
	Point(int i)          : x(i),   y(i)   {}
	Point(int x0, int y0) : x(x0),  y(y0)  {}
	Point(const Point& p) : x(p.x), y(p.y) {}

	Point& operator  =(const Point& p)
	{ x  = p.x, y  = p.y; return *this; }

	Point& operator +=(const Point& p)
	{ x += p.x, y += p.y; return *this; }

	Point& operator -=(const Point& p)
	{ x -= p.x, y -= p.y; return *this; }

	Point& operator *=(const Point& p)
	{ x *= p.x, y *= p.y; return *this; }

	Point& operator /=(const Point& p)
	{ x /= p.x, y /= p.y; return *this; }

	int getArea() const
	{ return x * y; }

	bool isNull() const
	{ return (x == 0) && (y == 0); }

	bool contains(const Point& p) const
	{ return (p.x <= x) && (p.y <= y); }

	void alignTo(const Point& p, AlignDir align_dir);
};

inline Point operator +(const Point& p1, const Point& p2)
{
	return Point(p1) += p2;
}

inline Point operator -(const Point& p1, const Point& p2)
{
	return Point(p1) -= p2;
}

inline Point operator *(const Point& p1, const Point& p2)
{
	return Point(p1) *= p2;
}

inline Point operator /(const Point& p1, const Point& p2)
{
	return Point(p1) /= p2;
}

inline bool operator ==(const Point& p1, const Point& p2)
{
	return (p1.x == p2.x) && (p1.y == p2.y);
}

inline bool operator !=(const Point& p1, const Point& p2)
{
	return !(p1 == p2);
}

std::ostream& operator <<(std::ostream& os, const Point& p);



/*******************************************************************
 * \class Corner
 * \brief Basic type specifying a corner
 *
 * This class helps managing corners and borders
 *******************************************************************/

enum XBorder {
	Left        = 0,
	Right       = 1,
};

enum YBorder {
	Top         = 0,
	Bottom      = 1,
};

std::ostream& operator <<(std::ostream& os, XBorder xb);
std::ostream& operator <<(std::ostream& os, YBorder yb);

class Corner
{
 public:
	Corner();
	Corner(XBorder xb, YBorder yb);
	
	void set(XBorder xb, YBorder yb);

	XBorder getX() const;
	YBorder getY() const;

	bool operator ==(const Corner& c);

 private:
	int m_code;
};

inline Corner::Corner()
{
	set(Left, Top);
}

inline Corner::Corner(XBorder xb, YBorder yb)
{
	set(xb, yb);
}

inline void Corner::set(XBorder xb, YBorder yb)
{
	m_code = (int(xb) << 1) | int(yb);
}

inline XBorder Corner::getX() const
{
	return XBorder(m_code >> 1);
}

inline YBorder Corner::getY() const
{
	return YBorder(m_code & 1);
}

inline bool Corner::operator ==(const Corner& c)
{
	return m_code == c.m_code;
}

extern const Corner TopLeft, TopRight, BottomLeft, BottomRight;

std::ostream& operator <<(std::ostream& os, const Corner& c);


/*******************************************************************
 * \class Size
 * \brief Basic rectangle size class
 *
 * This class helps managing the size of rectangular objects
 *******************************************************************/

class Size
{
 public:
	Size()               : m_xy()                        {}
	Size(int w, int h)   : m_xy(checkValid(Point(w, h))) {}
	Size(const Point& p) : m_xy(checkValid(p))           {}
	Size(const Size& s)  : m_xy(s.m_xy)                  {}

	int getWidth() const
	{ return m_xy.x; }

	int getHeight() const
	{ return m_xy.y; }

	bool isEmpty() const
	{ return m_xy.getArea() == 0; }

	operator Point() const
	{ return m_xy; }

	Size& operator  =(const Point& p)
	{ m_xy = checkValid(p);        return *this; }

	Size& operator +=(const Point& p)
	{ m_xy = checkValid(m_xy + p); return *this; }

	Size& operator -=(const Point& p)
	{ m_xy = checkValid(m_xy - p); return *this; }

	Size& operator *=(const Point& p)
	{ m_xy = checkValid(m_xy * p); return *this; }

	Size& operator /=(const Point& p)
	{ m_xy = checkValid(m_xy / p); return *this; }

	void alignTo(const Point& p, AlignDir align_dir)
	{ m_xy.alignTo(p, align_dir); }

	Point getCornerCoords(const Point& p, const Corner& c);

 private:
	static bool isValidCoord(int i);
	static Point checkValid(const Point& p);

	Point m_xy;
};

inline bool Size::isValidCoord(int i)
{
	return (i >= 0);
}

inline Point Size::checkValid(const Point& p)
{
	if (!(isValidCoord(p.x) && isValidCoord(p.y)))
		throw LIMA_COM_EXC(InvalidValue, "Invalid size");
	return p;
}

std::ostream& operator <<(std::ostream& os, const Size& s);


/*******************************************************************
 * \class Bin
 * \brief Class holding a horizontal (X) and vertical (Y) binning
 *
 * Basic binning handling. The values must be power of 2
 *******************************************************************/

class Bin
{
 public:
	Bin()		    : m_xy(1)                       {}
	Bin(int x, int y)   : m_xy(checkValid(Point(x, y))) {}
	Bin(const Point& p) : m_xy(checkValid(p))           {}
	Bin(const Bin& b)   : m_xy(b.m_xy)                  {}

	int getX() const
	{ return m_xy.x; }

	int getY() const
	{ return m_xy.y; }

	bool isOne() const
	{ return ((m_xy.x==1) && (m_xy.y==1)); }

	void reset()
	{ m_xy.x= 1; m_xy.y= 1; }

	operator Point() const
	{ return m_xy; }

	Bin& operator  =(const Point& p)
	{ m_xy = checkValid(p);        return *this; }

	Bin& operator *=(const Point& p)
	{ m_xy = checkValid(m_xy * p); return *this; }

	Bin& operator /=(const Point& p)
	{ m_xy = checkValid(m_xy / p); return *this; }

 private:
	static bool isValidCoord(int i);
	static Point checkValid(const Point& p);

	Point m_xy;
};

inline bool Bin::isValidCoord(int i)
{
	return IsPowerOf2(i);
}

inline Point Bin::checkValid(const Point& p)
{
	if (!(isValidCoord(p.x) && isValidCoord(p.y)))
		throw LIMA_COM_EXC(InvalidValue, "Invalid binning");
	return p;
}

std::ostream& operator <<(std::ostream& os, const Bin& bin);


/*******************************************************************
 * \class Flip
 * \brief Class specifying horizontal and vertical flip modes
 *
 *******************************************************************/

class Flip
{
 public:
	bool x, y;

	Flip()                 : x(false), y(false) {}
	Flip(bool b)           : x(b),     y(b)     {}
	Flip(bool x0, bool y0) : x(x0),    y(y0)    {}
	Flip(const Flip& f)    : x(f.x),   y(f.y)   {}

	operator Point() const
	{ return Point(x, y); }

	Corner getRefCorner() const
	{ return Corner(x ? Right : Left, y ? Bottom : Top); }

	Flip& operator &=(const Flip& f)
	{ x ^= f.x; y ^= f.y; return *this; }
};

inline Flip operator &(const Flip& f1, const Flip& f2)
{
	Flip flip = f1;
	return flip &= f2;
}


/*******************************************************************
 * \class Roi
 * \brief Class specifying a rectangular region-of-interest
 *
 * The roi is represented in terms of origin (top_left) + size.
 * The bottom_right corner \b belongs to the roi: top_left + size - 1
 *******************************************************************/

class Roi
{
 public:
	Roi() {}

	Roi(const Point& top_left, const Size& size)
		: m_top_left(checkCorner(top_left)),
		  m_size(size) {}

        Roi(const Point& p1, const Point& p2);

	Roi(const Roi& r)
		: m_top_left(r.m_top_left),
		  m_size(r.m_size) {}

	Roi& operator =(const Roi& r);

	const Point& getTopLeft() const;
	const Size& getSize() const;

	bool isEmpty() const;
	void reset();

	Point getTopRight() const;
	Point getBottomLeft() const;
	Point getBottomRight() const;

	void setTopLeft(const Point& top_left);
	void setSize(const Size& size);
	void setCorners(const Point& p1, const Point& p2);

	bool containsPoint(const Point& p) const;
	bool containsRoi(const Roi& r) const;

	Roi getBinned(const Bin& b) const;
	Roi getUnbinned(const Bin& b) const;

	bool isActive() const;

	void alignSizeTo(const Point& p, AlignDir align_dir);
	void alignCornersTo(const Point& p, AlignDir align_dir);

	Roi subRoiRel2Abs(const Roi& rel_sub_roi) const;
	Roi subRoiAbs2Rel(const Roi& abs_sub_roi) const;

 private:
	static bool isValidCoord(int i);
	static Point checkCorner(const Point& TopLeft);

	Point m_top_left;
	Size m_size;
};

inline Roi::Roi(const Point& p1, const Point& p2)
{
	setCorners(p1, p2);
}

inline bool Roi::isEmpty() const
{
	return m_size.isEmpty();
}

inline void Roi::reset()
{
	m_top_left = Point(0, 0);
	m_size = Size(0, 0);
}

inline Roi& Roi::operator =(const Roi& r)
{ 
	m_top_left = r.m_top_left; 
	m_size = r.m_size; 
	return *this; 
}

inline const Point& Roi::getTopLeft() const
{
	return m_top_left;
}

inline Point Roi::getTopRight() const
{
	return m_top_left + Point(m_size.getWidth() - 1, 0);
}

inline Point Roi::getBottomLeft() const
{
	return m_top_left + Point(0, m_size.getHeight() - 1);
}

inline Point Roi::getBottomRight() const
{
	return m_top_left + m_size - 1;
}

inline const Size& Roi::getSize() const
{
	return m_size;
}

inline void Roi::setTopLeft(const Point& top_left)
{
	m_top_left = checkCorner(top_left);
}

inline void Roi::setSize(const Size& size)
{
	m_size = size;
}

inline bool Roi::isValidCoord(int i)
{
	return (i >= 0);
}

inline Point Roi::checkCorner(const Point& p)
{
	if (!(isValidCoord(p.x) && isValidCoord(p.y)))
		throw LIMA_COM_EXC(InvalidValue, "Invalid corner coords");
	return p;
}

inline bool Roi::containsPoint(const Point& p) const
{
	return p.contains(getTopLeft()) && getBottomRight().contains(p);
}

inline bool Roi::containsRoi(const Roi& r) const
{
	return (containsPoint(r.getTopLeft()) && 
		containsPoint(r.getBottomRight()));
}

inline Roi Roi::getBinned(const Bin& b) const
{
	return Roi(m_top_left / b, Size(m_size / b));
}

inline Roi Roi::getUnbinned(const Bin& b) const
{
	return Roi(m_top_left * b, Size(m_size * b));
}

inline bool Roi::isActive() const
{
	return !m_size.isEmpty();
}

inline void Roi::alignSizeTo(const Point& p, AlignDir align_dir)
{
	
	m_size.alignTo(p, align_dir);
}

inline void Roi::alignCornersTo(const Point& p, AlignDir align_dir)
{
	Point br = getBottomRight() + 1;
	AlignDir tl_align_dir = (align_dir == Floor) ? Ceil : Floor;
	m_top_left.alignTo(p, tl_align_dir);
	br.alignTo(p, align_dir);
	m_size = br - m_top_left;
}


inline bool operator ==(const Roi& r1, const Roi& r2)
{
	return ((r1.getTopLeft() == r2.getTopLeft()) &&
		(r1.getSize() == r2.getSize()));
}

inline bool operator !=(const Roi& r1, const Roi& r2)
{
	return !(r1 == r2);
}

std::ostream& operator <<(std::ostream& os, const Roi& roi);


/*******************************************************************
 * \class FrameDim
 * \brief Class holding the Size and ImageType of a frame
 *
 * This class contains the X, Y and Z dimensions of a frame. For
 * practical reasons, it also contains the depth in bytes.
 *******************************************************************/

class FrameDim
{
 public:
	FrameDim();
	FrameDim(const Size& size, ImageType type);
	FrameDim(int width, int height, ImageType type);

	bool isValid() const;

	void setSize(const Size& size);
	const Size& getSize() const;

	void setImageType(ImageType image_type);
	ImageType getImageType() const;

	int getDepth() const;
	int getMemSize() const;

	static int getImageTypeBpp(ImageType type);
	static int getImageTypeDepth(ImageType type);

	FrameDim& operator *=(const Point& point);
	FrameDim& operator /=(const Point& point);

	void checkValidPoint(const Point& point, bool for_div);
	void checkValidRoi(const Roi& roi);

 private:
	Size m_size;
	ImageType m_type;
	int m_depth;
};

inline FrameDim::FrameDim()
	: m_size(), m_type(Bpp8)
{
	m_depth = getImageTypeDepth(m_type);
}

inline FrameDim::FrameDim(const Size& size, ImageType type)
	: m_size(size), m_type(type)
{
	m_depth = getImageTypeDepth(m_type);
}

inline FrameDim::FrameDim(int width, int height, ImageType type)
	: m_size(Size(width, height)), m_type(type)
{
	m_depth = getImageTypeDepth(m_type);
}

inline bool FrameDim::isValid() const
{
	return !m_size.isEmpty();
}

inline const Size& FrameDim::getSize() const
{
	return m_size;
}

inline void FrameDim::setSize(const Size& size)
{
	m_size = size;
}

inline void FrameDim::setImageType(ImageType image_type)
{
	m_type = image_type;
	m_depth = getImageTypeDepth(m_type);
}

inline ImageType FrameDim::getImageType() const
{
	return m_type;
}

inline int FrameDim::getDepth() const
{
	return m_depth;
}

inline int FrameDim::getMemSize() const
{
	return Point(m_size).getArea() * m_depth;
}

inline void FrameDim::checkValidPoint(const Point& point, bool for_div)
{
	if ((point.x <= 0) || (point.y <= 0))
		throw LIMA_COM_EXC(InvalidValue, "Invalid point");

	if (for_div && ((m_size.getWidth()  % point.x != 0) ||
			(m_size.getHeight() % point.y != 0)))
		throw LIMA_COM_EXC(InvalidValue, "FrameDim size not multiple "
						 "of point");
}

inline void FrameDim::checkValidRoi(const Roi& roi)
{
	Roi full_frame(0, m_size);
	if (!full_frame.containsRoi(roi))
		throw LIMA_COM_EXC(InvalidValue, "Roi does not fit in the "
				   "FrameDim");
}

inline FrameDim& FrameDim::operator *=(const Point& point)
{
	checkValidPoint(point, false);
	m_size *= point;
	return *this;
}

inline FrameDim& FrameDim::operator /=(const Point& point)
{
	checkValidPoint(point, true);
	m_size /= point;
	return *this;
}

inline bool operator ==(const FrameDim& f1, const FrameDim& f2)
{
	return ((f1.getSize()  == f2.getSize()) && 
		(f1.getDepth() == f2.getDepth()));
}

inline bool operator !=(const FrameDim& f1, const FrameDim& f2)
{	
	return !(f1 == f2);
}

inline FrameDim operator *(const FrameDim& fdim, const Point& point)
{
	FrameDim point_fdim = fdim;
	return point_fdim *= point;
}

inline FrameDim operator /(const FrameDim& fdim, const Point& point)
{
	FrameDim point_fdim = fdim;
	return point_fdim /= point;
}

std::ostream& operator <<(std::ostream& os, const FrameDim& fdim);


} // namespace lima

#endif // SIZEUTILS_H
