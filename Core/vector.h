
#ifndef Rocket_Core_Vector_H
#define Rocket_Core_Vector_H

#include <math.h>
#include <iostream>
#include <vector>

#define EIGEN_MPL2_ONLY
#include <eigen3/Eigen/Dense>
using namespace Eigen;

namespace Rocket {
	namespace Core {

		template< class T, unsigned int N > class T_vec {
			static_assert( N > 1, "Number of elements must be greater than 1" );

		public:
			Matrix< T, N, 1 > m_elements;

			// Constructors
			T_vec() { m_elements.setZero(); }
			T_vec( Matrix< T, N, 1 > elements ) { m_elements = elements; }

			template< unsigned int Nleft, unsigned int Nright >
			T_vec( const T_vec< T, Nleft > & left, const T_vec< T, Nright > & right ) {
				static_assert( Nleft + Nright == N, "Left and Right vectors do not contain the right number of elements" );
				m_elements = Matrix< T, N, 1 >();
				for ( unsigned int i = 0; i < Nleft; i++ ) {
					m_elements[i] = left[i];
				}
				for ( unsigned int i = Nleft; i < N; i++ ) {
					m_elements[i] = right[ i - Nleft ];
				}
			}

			template< unsigned int N2, typename... Args >
			T_vec( const T_vec< T, N2 > & v, Args &&... args ) {
				static_assert( sizeof...( Args ) == N - N2, "Invalid number of arguments" );
				m_elements = Matrix< T, N, 1 >();
				for ( unsigned int i = 0; i < N2; i++ ) {
					m_elements[i] = v[i];
				}
				std::vector< T > argsVector = { args... };
				for ( unsigned int i = N2; i < N; i++ ) {
					m_elements[i] = argsVector[ i - N2 ];
				}
			}

			template< typename... Args >
			T_vec( T first, Args &&... args ) {
				static_assert( sizeof...( Args ) == N - 1, "Invalid number of arguments" );
				m_elements = Matrix< T, N, 1 >( first, args... );
			}

			// Access Operators and Functions
			T & operator [] ( int index ) { return m_elements[index]; }
			const T & operator [] ( int index ) const { return m_elements[index]; }

			T x() const { return m_elements[0]; }
			T y() const { return m_elements[1]; }
			T z() const {
				static_assert( N > 2, "Element z does not exist in this vector" );
				return m_elements[2];
			}
			T w() const {
				static_assert( N > 3, "Element z does not exist in this vector" );
				return m_elements[3];
			}

			void x( T v ) { m_elements[0] = v; }
			void y( T v ) { m_elements[1] = v; }
			void z( T v ) {
				static_assert( N > 2, "Element z does not exist in this vector" );
				m_elements[2] = v;
			}
			void w( T v ) {
				static_assert( N > 3, "Element z does not exist in this vector" );
				m_elements[3] = v;
			}

			// Unary Operators
			T_vec operator - () const { return T_vec( -m_elements ); }

			// Arithmetic
			T_vec operator + ( const T_vec & R ) const { return T_vec( m_elements + R.m_elements ); }
			T_vec operator - ( const T_vec & R ) const { return T_vec( m_elements - R.m_elements ); }
			T_vec operator * ( T value ) const { return T_vec( m_elements * value ); }					// Left hand T_vec
			friend T_vec operator * ( T value, const T_vec & R ) { return T_vec( R * value ); }			// Right hand T_vec

			// Equality
			bool operator == ( const T_vec & R ) const { return m_elements == R.m_elements; }
			bool operator != ( const T_vec & R ) const { return m_elements != R.m_elements; }

			// Assignment Operators
			T_vec & operator += ( const T_vec & R ) { m_elements += R.m_elements; return *this; }
			T_vec & operator -= ( const T_vec & R ) { m_elements -= R.m_elements; return *this; }
			T_vec & operator *= ( T value ) { m_elements *= value; return *this; }

			// Vector Operations
			T lengthSquared() const { return m_elements.squaredNorm(); }
			T normSquared() const { return m_elements.squaredNorm(); }
			T length() const { return m_elements.norm(); }
			T norm() const { return m_elements.norm(); }
			T dot( const T_vec & R ) const { return m_elements.dot( R.m_elements ); }				// Dot Product
			T operator * ( const T_vec & R ) const { return dot( R ); }								// Dot Product
			T_vec cross( const T_vec & R ) const { return m_elements.cross( R.m_elements ); }		// Cross Product
			void normalize() { m_elements.normalize(); }
			T_vec normalized() const { return T_vec( m_elements.normalized() ); }

			// Swizzle
			T_vec< T, 2 > xy() const { return T_vec< T, 2 >( x(), y() ); }
			T_vec< T, 2 > yx() const { return T_vec< T, 2 >( y(), x() ); }

			T_vec< T, 2 > xz() const { return T_vec< T, 2 >( x(), z() ); }
			T_vec< T, 2 > yz() const { return T_vec< T, 2 >( y(), z() ); }
			T_vec< T, 2 > zx() const { return T_vec< T, 2 >( z(), x() ); }
			T_vec< T, 2 > zy() const { return T_vec< T, 2 >( z(), y() ); }

			T_vec< T, 3 > xyz() const { return T_vec< T, 3 >( x(), y(), z() ); }
			T_vec< T, 3 > zyx() const { return T_vec< T, 3 >( z(), y(), x() ); }

			T_vec< T, 4 > xyzw() const { return T_vec< T, 4 >( x(), y(), z(), w() ); }
			T_vec< T, 4 > wzyx() const { return T_vec< T, 4 >( w(), z(), y(), x() ); }

			// Conversion Operators
			template< class U >
			operator T_vec< U, N >() const {
				return T_vec< U, N >( m_elements.template cast< U >() );
			}

			operator T * () { return m_elements.data(); }

			friend std::ostream & operator << ( std::ostream& stream, const T_vec< T, N > & v ) {
				IOFormat VectorFormat( FullPrecision, 0, ", ", "\n", "(", ")" );
				return stream << v.m_elements.format( VectorFormat );
			}
		};

		// Typedefs
		typedef T_vec< float, 2 > vec2;
		typedef T_vec< double, 2 > vec2d;
		typedef T_vec< int, 2 > vec2i;
		
		typedef T_vec< float, 3 > vec3;
		typedef T_vec< double, 3 > vec3d;
		typedef T_vec< int, 3 > vec3i;
		
		typedef T_vec< float, 4 > vec4;
		typedef T_vec< double, 4 > vec4d;
		typedef T_vec< int, 4 > vec4i;
		

		// External Vector Operations
		template< class T, unsigned int N >
		T dot( const T_vec< T, N > & a, const T_vec< T, N > & b ) { return a.dot( b ); }

		template< class T, unsigned int N > T_vec< T, N >
		cross( const T_vec< T, N > & a, const T_vec< T, N > & b ) { return a.cross( b ); }

		template< class T, unsigned int N > T_vec< T, N >
		normalize( const T_vec< T, N > & v ) { return v.normalized(); }

		template< class T > T_vec< T, 4 >
		Quaternion( T angle, const T_vec< T, 3 > & axis ) {
			// Returns a normalized quaternion
			T sinangle = sin( angle/2.0f );
			T_vec< T, 4 > quat( axis.x()*sinangle, axis.y()*sinangle, axis.z()*sinangle, cos(angle/2.0f) );
			quat.normalize();
			return quat;
		}

		template< class T > T_vec< T, 3 > QuaternionToEuler( const T_vec< T, 4 > & quat2 ) {
			T_vec< T, 4 > quat = quat2;
			quat.normalize();
			T x = quat.x();
			T y = quat.y();
			T z = quat.z();
			T w = quat.w();

			float pitch = asin( 2.0f * ( x*y + z*w ) );
			float yaw = atan2( 2.0f * ( y*w - x*z ), 1.0f - 2.0f * ( y*y - z*z ) );
			float roll = atan2( 2.0f * ( x*w - y*z ) , 1.0f - 2.0f * ( x*x - z*z ) );
			// Todo: fix; this function doesnt really work...
			// yaw is correct, but pitch and roll are not in the correct XYZ-axis order
			return T_vec< T, 3 >( pitch, yaw, roll );
		}
		
	}
}

#endif

