
#ifndef Rocket_Core_Matrix_H
#define Rocket_Core_Matrix_H

#include <math.h>
#include <iostream>
#include <vector>

#define EIGEN_MPL2_ONLY
#define EIGEN_DEFAULT_TO_ROW_MAJOR
#include <eigen3/Eigen/Dense>

#include "vector.h"
#include "mathconstants.h"

namespace Rocket {
	namespace Core {

		// All matrices are in row by column order:
		// 00 01 02 03
		// 10 11 12 13
		// 20 21 22 23
		// 30 31 32 33

		template< class T, unsigned int R, unsigned int C > class T_mat {
		public:
			Eigen::Matrix< T, R, C > m_matrix;

			// Constructors

			// Default is identity matrix
			T_mat< T, R, C >() { m_matrix.setIdentity(); }
			// Fill the matrix with an Eigen Matrix
			T_mat< T, R, C >( Eigen::Matrix< T, R, C > matrix ) { m_matrix = matrix; }

			// Constructor that takes in R x C number of elements
			inline void eigenCommaExpand( Eigen::CommaInitializer< Eigen::Matrix< T, R, C > > & commaInit ) {}
			template< typename... Args >
			inline void eigenCommaExpand( Eigen::CommaInitializer< Eigen::Matrix< T, R, C > > & commaInit, T && head, Args &&... tail ) {
				commaInit.operator , ( head );
				eigenCommaExpand( commaInit, std::forward< T >( tail )... );
			}
			template< typename... Args >
			T_mat< T, R, C >( T first, Args &&... args ) {
				auto commaInit = m_matrix.operator << ( first );
				eigenCommaExpand( commaInit, std::forward< T >( args )... );
			}

			// Constructor that takes in R number of C-vectors
			template< typename... Args >
			T_mat< T, R, C >( T_vec< T, C > first, Args &&... args ) {
				m_matrix.block( 0, 0, 1, C ) << first.m_elements;
				std::vector< T_vec< T, C > > otherVectors = { std::forward< T_vec< T, C > >( args )... };
				for ( unsigned int i = 1; i < otherVectors.size(); i++ ) {
					m_matrix.block( 0, i, 1, C ) << otherVectors[i].m_elements;
				}
			}

			// Array Subscript
			T & operator () ( unsigned int row, unsigned col ) { return m_matrix( row, col ); }

			// Arithmetic
			// M * V
			T_vec< T, C > operator * ( const T_vec< T, C > & vector ) const {
				return T_vec< T, C >( m_matrix * vector.m_elements );
			}
			// M * M
			template< unsigned int R2, unsigned int C2 >
			T_mat operator * ( const T_mat< T, R2, C2 > & r ) const {
				return T_mat< T, R, C2 >( m_matrix * r.m_matrix );
			}

			// Assignment Operators
			T_mat & operator *= ( const T_mat & r ) { *this = *this * r; return *this; }

			// Matrix Operations
			void transpose() { m_matrix.transposeInPlace(); }
			T_mat transposed() { return T_mat< T, R, C >( m_matrix.transpose() ); }
			T determinant() { return m_matrix.determinant(); }
			T_mat inverse() { return T_mat< T, R, C >( m_matrix.inverse() ); }

			// Returns this matrix with row and col removed
			// (minor) is required because sysmacros.h defines minor as a macro
			T_mat< T, R - 1, C - 1 > * (minor)( unsigned int row, unsigned int col ) {
				T_mat< T, R - 1, C - 1 > m = new T_mat< T, R - 1, C - 1 >();
				int x = 0, y = 0;
				for ( unsigned int r = 0; r < R; r++) {
					if ( r != row ) {
						for ( unsigned int c = 0; c < C; c++) {
							if ( c != col ) {
								m(x,y) = m_matrix(r,c);
								y++;
							}
						}
						x++;
						y = 0;
					}
				}
				return m;
			}

			// Conversion Operators
			operator Eigen::Matrix< T, R, C >() const { return m_matrix; }

			template< class U > operator T_mat< U, R, C >() const { return T_mat< U, R, C >( m_matrix.template cast< U >() ); }

			operator T * () { return m_matrix.data(); }

			friend std::ostream & operator << ( std::ostream& stream, const T_mat< T, R, C > & m ) {
				Eigen::IOFormat MatrixFormat( Eigen::FullPrecision, 0, ", ", "\n", "[", "]" );
				return stream << m.m_matrix.format( MatrixFormat );
			}
		};

		// Typedefs
		typedef T_mat< float, 2, 2 > mat2;
		typedef T_mat< double, 2, 2 > mat2d;
		
		typedef T_mat< float, 3, 3 > mat3;
		typedef T_mat< double, 3, 3 > mat3d;
		
		typedef T_mat< float, 4, 4 > mat4;
		typedef T_mat< double, 4, 4 > mat4d;

		// External Matrix Operations

		// Scale()
		// x 0 0 0
		// 0 y 0 0
		// 0 0 z 0
		// 0 0 0 1
		template< class T > T_mat< T, 4, 4 > Scale( const T_vec< T, 3 > & v ) {
			T_mat< T, 4, 4 > r;
			for ( unsigned int i = 0; i < 3; i++ ) {
				r.m_matrix.diagonal()[i] = v[i];
			}
			return r;
		}

		// Translate
		// 1 0 0 x
		// 0 1 0 y
		// 0 0 1 z
		// 0 0 0 1
		template< class T > T_mat< T, 4, 4 > Translate( const T_vec< T, 3 > & v ) {
			T_mat< T, 4, 4 > r;
			r.m_matrix.block( 0, 3, 3, 1 ) << v.m_elements;
			return r;
		}

		template< class T > T_mat< T, 4, 4 > QuaternionRotate( T_vec< T, 4 > quat ) {
			T x = quat.x();
			T y = quat.y();
			T z = quat.z();
			T w = quat.w();

			T_mat< T, 4, 4 > r;
			r.m_matrix <<	1 - 2*(y*y + z*z),	2*(x*y - w*z),		2*(x*z + w*y),		0,
							2*(x*y + w*z),		1 - 2*(x*x + z*z),	2*(y*z - w*x),		0,
							2*(x*z - w*y),		2*(y*z + w*x),		1 - 2*(x*x + y*y),	0,
							0,					0,					0,					1;
			return r;
		}

		template< class T > T_mat< T, 4, 4 > Rotate( const T angle, const T_vec< T, 3 > & axis ) {
			return QuaternionRotate( Quaternion( angle, axis ) );
		}

		template< class T > T_vec< T, 4 > MatrixToQuaternion( T_mat< T, 4, 4 > m ) {
			float trace = m(0,0) + m(1,1) + m(2,2) + 1.0f;

			if (trace > MathConstants::ZeroTolerance) {
				float s = sqrt( trace ) * 2.0f;
				return normalize( T_vec< T, 4 >( (m(2,1) - m(1,2)) / s, (m(0,2) - m(2,0)) / s, (m(1,0) - m(0,1)) / s, s / 4.0f ) );
			} else if ( (m(0,0) > m(1,1)) && (m(0,0) > m(2,2)) ) {
				float s = sqrt( m(0,0) - m(1,1) - m(2,2) + 1.0f ) * 2.0f;
				return normalize( T_vec< T, 4 >( s / 4.0f, (m(1,0) + m(0,1)) / s, (m(0,2) + m(2,0)) / s, (m(2,1) - m(1,2)) / s ) );
			} else if (m(1,1) > m(2,2)) {
				float s = sqrt( m(1,1) - m(0,0) - m(2,2) + 1.0f ) * 2.0f;
				return normalize( T_vec< T, 4 >( (m(1,0) + m(0,1)) / s, s / 4.0f, (m(2,1) + m(1,2)) / s, (m(0,2) - m(2,0)) / s ) );
			} else {
				float s = sqrt( m(2,2) - m(0,0) - m(1,1) + 1.0f ) * 2.0f;
				return normalize( T_vec< T, 4 >( (m(0,2) + m(2,0)) / s, (m(2,1) + m(1,2)) / s, s / 4.0f, (m(1,0) - m(0,1)) / s ) );
			}
		}

		template< class T > T_vec< T, 3 > QuaternionToUnitVector( const T_vec< T, 4 > & quat ) {
			T_vec< T, 4 > r = QuaternionRotate( quat ) * T_vec< T, 4 >( 0.0f, 0.0f, -1.0f, 0.0f );
			return T_vec< T, 3 >( -r.x(), -r.y(), r.z() );
		}


		// Camera Matrix Operations
		/*template <class T> T_mat4<T> LookAt( const T_vec< T, 4 > & look, const T_vec< T, 4 > & up, const T_vec< T, 4 > & position ) {
			T_vec< T, 4 > n = normalize( look - position );
			T_vec< T, 4 > u = T_vec< T, 4 >( normalize( cross(up,n) ), 0.0 );
			T_vec< T, 4 > v = T_vec< T, 4 >( normalize( cross(n,u) ), 0.0 );
			T_mat4<T> r = T_mat4<T>( u, v, n, T_vec< T, 4 >( 0.0, 0.0, 0.0, 1.0 ) );
			return r * Translate( -look );
		}*/


		// Projection Matrix Operations
		template< class T > T_mat< T, 4, 4 > Orthographic( const T left, const T right, const T bottom, const T top, const T zNear, const T zFar ) {
			T_mat< T, 4, 4 > r;
			r.m_matrix(0,0) = 2.0f / (right - left),
			r.m_matrix(1,1) = 2.0f / (top - bottom),
			r.m_matrix(2,2) = 2.0f / (zNear - zFar),
			r.m_matrix.block( 0, 3, 3, 1 ) <<
				-(right + left) / (right - left),
				-(top + bottom) / (top - bottom),
				-(zFar + zNear) / (zFar - zNear);
			return r;
		}

		template< class T > T_mat< T, 4, 4 > Orthographic2D( const T left, const T right, const T bottom, const T top ) {
			return Orthographic( left, right, bottom, top, -1.0f, 1.0f );
		}

		template< class T > T_mat< T, 4, 4 > Frustum( const T left, const T right, const T bottom, const T top, const T zNear, const T zFar ) {
			T_mat< T, 4, 4 > r;
			r(0,0) = 2.0f*zNear / (right - left);
			r(0,2) = (right + left) / (right - left);
			r(1,1) = 2.0f*zNear / (top - bottom);
			r(1,2) = (top + bottom) / (top - bottom);
			r(2,2) = -(zFar + zNear) / (zFar - zNear);
			r(2,3) = -2.0f*zFar*zNear / (zFar - zNear);
			r(3,2) = -1.0f;
			r(3,3) = 0.0f;
			return r;
		}

		template< class T > T_mat< T, 4, 4 > Perspective( const T FOV_Y, const T AspectRatio, const T zNear, const T zFar) {
			T top = tan( FOV_Y * MathConstants::DegreesToRadians * 0.5f ) * zNear;
			T right = top * AspectRatio;

			T_mat< T, 4, 4 > r;
			r(0,0) = zNear / right;
			r(1,1) = zNear / top;
			r(2,2) = -(zFar + zNear) / (zFar - zNear);
			r(2,3) = -2.0f*zFar*zNear / (zFar - zNear);
			r(3,2) = -1.0f;
			return r;
		}

	}
}

#endif
