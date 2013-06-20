
#ifndef Rocket_Core_Vector_H
#define Rocket_Core_Vector_H

#include <math.h>
#include <iostream>

namespace Rocket {
	namespace Core {

		// Vector2
		// --------------------------------------------------------------------------------------------------------------------
		template <class T> class T_vec2 {
		public:
			union {
				struct {
					T x, y;
				};
				T m_elements[2];
			};

			// Constructors
			T_vec2() : x(0), y(0) { }
			T_vec2( T x, T y ) : x(x), y(y) { }

			// Array Subscript
			T & operator [] ( int index ) { return m_elements[index]; }

			// Unary Operators
			T_vec2 operator - () const { return T_vec2( -x, -y ); }

			// Arithmetic
			T_vec2 operator + ( const T_vec2 & R ) const { return T_vec2( x + R.x, y + R.y ); }
			T_vec2 operator - ( const T_vec2 & R ) const { return T_vec2( x - R.x, y - R.y ); }
			T_vec2 operator * ( T value ) const { return T_vec2( x * value, y * value ); }							// Left hand T_vec2
			friend T_vec2 operator * ( T value, const T_vec2 & R ) { return T_vec2( R.x * value, R.y * value ); }	// Right hand T_vec2

			// Equality
			bool operator == ( const T_vec2 & R ) const { return ((x == R.x) && (y == R.y)); }
			bool operator != ( const T_vec2 & R ) const { return ((x != R.x) || (y != R.y)); }

			// Assignment Operators
			T_vec2 & operator += ( const T_vec2 & R ) { x += R.x; y += R.y; return *this; }
			T_vec2 & operator -= ( const T_vec2 & R ) { x -= R.x; y -= R.y; return *this; }
			T_vec2 & operator *= ( T value ) { x *=	value; y *= value; return *this; }

			// Vector Operations
			T operator * ( const T_vec2 & R ) const { return x*R.x + y*R.y; }		// Dot Product
			T lengthSquared() const { return x*x + y*y; }
			T length() const { return sqrt( x*x + y*y ); }

			// Swizzle
			T_vec2<T> xy() const { return T_vec2<T>( x, y ); }
			T_vec2<T> yx() const { return T_vec2<T>( y, x ); }
			
			// Conversion Operators
			template <class U> operator T_vec2<U>() const { return T_vec2<U>( U(x), U(y) ); }	// Cast to different typed T_vec2
			operator T * () const { return (T*)&m_elements[0]; }

			friend std::ostream & operator << ( std::ostream& stream, const T_vec2<T> & v ) {
				return stream << "(" << v.x << "," << v.y << ")";
			}
		};


		// Vector3
		// --------------------------------------------------------------------------------------------------------------------
		template <class T> class T_vec3 {
		public:
			union {
				struct {
					T x, y, z;
				};
				T m_elements[3];
			};

			// Constructors
			T_vec3() : x(0), y(0), z(0) { }
			T_vec3( T x, T y, T z ) : x(x), y(y), z(z) { }

			T_vec3( const T_vec2<T> & other, T z ) : x(other.x), y(other.y), z(z) { }

			// Array Subscript
			T & operator [] ( int index ) { return m_elements[index]; }

			// Unary Operators
			T_vec3 operator - () const { return T_vec3( -x, -y, -z ); }

			// Arithmetic
			T_vec3 operator + ( const T_vec3 & R ) const { return T_vec3( x + R.x, y + R.y, z + R.z ); }
			T_vec3 operator - ( const T_vec3 & R ) const { return T_vec3( x - R.x, y - R.y, z - R.z ); }
			T_vec3 operator * ( T value ) const { return T_vec3( x * value, y * value, z * value ); }							// Left hand T_vec3
			friend T_vec3 operator * ( T value, const T_vec3 & R ) { return T_vec3( R.x * value, R.y * value, R.z * value ); }	// Right hand T_vec3

			// Equality
			bool operator == ( const T_vec3 & R ) const { return ((x == R.x) && (y == R.y) && (z == R.z)); }
			bool operator != ( const T_vec3 & R ) const { return ((x != R.x) || (y != R.y) || (z != R.z)); }

			// Assignment Operators
			T_vec3 & operator += ( const T_vec3 & R ) { x += R.x; y += R.y; z += R.z; return *this; }
			T_vec3 & operator -= ( const T_vec3 & R ) { x -= R.x; y -= R.y; z -= R.z; return *this; }
			T_vec3 & operator *= ( T value ) { x *=	value; y *= value; z *= value; return *this; }

			// Vector Operations
			T operator * ( const T_vec3 & R ) const { return x*R.x + y*R.y + z*R.z; }	// Dot Product
			T lengthSquared() const { return x*x + y*y + z*z; }
			T length() const { return sqrt( x*x + y*y + z*z ); }

			// Swizzle
			T_vec2<T> xy() const { return T_vec2<T>( x, y ); }
			T_vec2<T> xz() const { return T_vec2<T>( x, z ); }
			T_vec2<T> yx() const { return T_vec2<T>( y, x ); }
			T_vec2<T> yz() const { return T_vec2<T>( y, z ); }
			T_vec2<T> zx() const { return T_vec2<T>( z, x ); }
			T_vec2<T> zy() const { return T_vec2<T>( z, y ); }

			T_vec3<T> xyz() const { return T_vec3<T>( x, y, z ); }
			T_vec3<T> zyx() const { return T_vec3<T>( z, y, x ); }

			// Conversion Operators
			template <class U> operator T_vec3<U>() const { return T_vec3<U>( U(x), U(y), U(z) ); }	// Cast to different typed T_vec3
			operator T * () const { return (T*)&m_elements[0]; }

			friend std::ostream & operator << ( std::ostream& stream, const T_vec3<T> & v ) {
				return stream << "(" << v.x << "," << v.y << "," << v.z << ")";
			}
		};


		// Vector4
		// --------------------------------------------------------------------------------------------------------------------
		template <class T> class T_vec4 {
		public:
			union {
				struct {
					T x, y, z, w;
				};
				T m_elements[4];
			};

			// Constructors
			T_vec4() : x(0), y(0), z(0), w(0) { }
			T_vec4( T x, T y, T z, T w ) : x(x), y(y), z(z), w(w) { }

			T_vec4( const T_vec4<T> & other, T z, T w ) : x(other.x), y(other.y), z(z), w(w) { }
			T_vec4( const T_vec3<T> & other, T w ) : x(other.x), y(other.y), z(other.z), w(w) { }
			T_vec4( const T_vec2<T> & left, const T_vec2<T> & right ) : x(left.x), y(left.y), z(right.x), w(right.y) { }

			// Array Subscript
			T & operator [] ( int index ) { return m_elements[index]; }

			// Unary Operators
			T_vec4 operator - () const { return T_vec4( -x, -y, -z, -w ); }

			// Arithmetic
			T_vec4 operator + ( const T_vec4 & R ) const { return T_vec4( x + R.x, y + R.y, z + R.z, w + R.w ); }
			T_vec4 operator - ( const T_vec4 & R ) const { return T_vec4( x - R.x, y - R.y, z - R.z, w - R.w ); }
			T_vec4 operator * ( T value ) const { return T_vec4( x * value, y * value, z * value, w * value ); }							// Left hand T_vec4
			friend T_vec4 operator * ( T value, const T_vec4 & R ) { return T_vec4( R.x * value, R.y * value, R.z * value, R.w * value ); }	// Right hand T_vec4

			// Equality
			bool operator == ( const T_vec4 & R ) const { return ((x == R.x) && (y == R.y) && (z == R.z) && (w == R.w)); }
			bool operator != ( const T_vec4 & R ) const { return ((x != R.x) || (y != R.y) || (z != R.z) || (w != R.w)); }

			// Assignment Operators
			T_vec4 & operator += ( const T_vec4 & R ) { x += R.x; y += R.y; z += R.z; w += R.w; return *this; }
			T_vec4 & operator -= ( const T_vec4 & R ) { x -= R.x; y -= R.y; z -= R.z; w -= R.z; return *this; }
			T_vec4 & operator *= ( T value ) { x *=	value; y *= value; z *= value; w *= value; return *this; }

			// Vector Operations
			T operator * ( const T_vec4 & R ) const { return x*R.x + y*R.y + z*R.z + w*R.w; }	// Dot Product
			T lengthSquared() const { return x*x + y*y + z*z + w*w; }
			T length() const { return sqrt( x*x + y*y + z*z + w*w ); }

			// Swizzle
			T_vec2<T> xy() const { return T_vec2<T>( x, y ); }
			T_vec2<T> xz() const { return T_vec2<T>( x, z ); }
			T_vec2<T> yx() const { return T_vec2<T>( y, x ); }
			T_vec2<T> yz() const { return T_vec2<T>( y, z ); }
			T_vec2<T> zx() const { return T_vec2<T>( z, x ); }
			T_vec2<T> zy() const { return T_vec2<T>( z, y ); }

			T_vec3<T> xyz() const { return T_vec3<T>( x, y, z ); }
			T_vec3<T> zyx() const { return T_vec3<T>( z, y, x ); }

			T_vec4<T> xyzw() const { return T_vec4<T>( x, y, z, w ); }
			T_vec4<T> wzyx() const { return T_vec4<T>( w, z, y, x ); }

			// Conversion Operators
			template <class U> operator T_vec4<U>() const { return T_vec4<U>( U(x), U(y), U(z), U(w) ); }	// Cast to different typed T_vec4
			operator T * () const { return (T*)&m_elements[0]; }

			friend std::ostream & operator << ( std::ostream& stream, const T_vec4<T> & v ) {
				return stream << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
			}
		};

		
		// Typedefs
		// --------------------------------------------------------------------------------------------------------------------
		typedef T_vec2<float> vec2;
		typedef T_vec2<double> vec2d;
		typedef T_vec2<int> vec2i;
		
		typedef T_vec3<float> vec3;
		typedef T_vec3<double> vec3d;
		typedef T_vec3<int> vec3i;
		
		typedef T_vec4<float> vec4;
		typedef T_vec4<double> vec4d;
		typedef T_vec4<int> vec4i;
		

		// External Vector Operations
		template <class T> T dot( const T_vec2<T> & a, const T_vec2<T> & b ) { return a * b; }
		template <class T> T dot( const T_vec3<T> & a, const T_vec3<T> & b ) { return a * b; }
		template <class T> T dot( const T_vec4<T> & a, const T_vec4<T> & b ) { return a * b; }

		template <class T> T_vec3<T> cross( const T_vec3<T> & a, const T_vec3<T> & b ) {
			return T_vec3<T>( a.y * b.z - b.y * a.z,
								a.x * b.z - b.x * a.z,
								a.x * b.y - b.x * a.y );
		}

		template <class T> T_vec2<T> normalize( const T_vec2<T> & v ) { T len = v.length(); return T_vec2<T>( v.x/len, v.y/len ); }
		template <class T> T_vec3<T> normalize( const T_vec3<T> & v ) { T len = v.length(); return T_vec3<T>( v.x/len, v.y/len, v.z/len ); }
		template <class T> T_vec4<T> normalize( const T_vec4<T> & v ) { T len = v.length(); return T_vec4<T>( v.x/len, v.y/len, v.z/len, v.w/len ); }

		template <class T> T_vec4<T> Quaternion( T angle, const T_vec3<T> & axis ) {
			// Returns a normalized quaternion
			T sinangle = sin( angle/2.0f );
			T_vec4<T> quat( axis.x*sinangle, axis.y*sinangle, axis.z*sinangle, cos(angle/2.0f) );
			quat = normalize( quat );
			return quat;
		}

		template <class T> T_vec3<T> QuaternionToEuler( const T_vec4<T> & quat2 ) {
			T_vec4<T> quat = quat2;
			quat = normalize( quat );
			T x = quat.x;
			T y = quat.y;
			T z = quat.z;
			T w = quat.w;

			float pitch = asin( 2.0f * (x*y + z*w) ); 
			float yaw = atan2( 2.0f * (y*w - x*z), 1.0f - 2.0f * (y*y - z*z) );
			float roll = atan2( 2.0f * (x*w - y*z) , 1.0f - 2.0f * (x*x - z*z) );
			// todo: fix; this function doesnt really work...
			// yaw is correct, but pitch and roll are not in the correct XYZ-axis order
			return T_vec3<T>( pitch, yaw, roll );
		}
		
	}
}

#endif