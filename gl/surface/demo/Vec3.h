class Vec3 {
public:
	Vec3() : x(0), y(0), z(0) {}
	Vec3(double xx) : x(xx), y(xx), z(xx) {}
	Vec3(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}
	Vec3 operator + (const Vec3 &v) const				// Vector addition 
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}
	Vec3 operator - (const Vec3 &v) const 				// Vector substraction 
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}
	Vec3 operator * (const double &r) const				// Scaler multiplication 
	{
		return Vec3(x * r, y * r, z * r);
	}
	Vec3 operator / (const double &r) const				// Scaler Division
	{
		return Vec3(x / r, y / r, z / r);
	}
	bool operator == (const double &r) const		    	// Vector equality check
	{
		if (x == r && y == r && z == r)
			return true;
		else return false;
	}
	double dotProduct(const Vec3 &v) const				// Vector dot product
	{
		return x * v.x + y * v.y + z * v.z;
	}
	Vec3 crossProduct(const Vec3 &v) const				// Vector cross product
	{
		return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	double norm() const						// Norm of a vector ||v||
	{
		return x * x + y * y + z * z;
	}
	double length() const					       	// length of vector
	{
	  return sqrt(norm());
	}
	double& operator [] (int i) { switch (i){ case 0:return x; case 1: return y; case 2: return z; } } 	// Indexing x, y, z coordinate in a vector

	Vec3& normalize()						// Normalize a vector by dividing it with its length
	{
		double n = norm();
		if (n > 0) {
			double factor = sqrt(n);
			x /= factor, y /= factor, z /= factor;
		}

		return *this;
	}
	double x, y, z;
};
