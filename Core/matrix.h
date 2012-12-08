
#ifndef Rocket_Core_Matrix_H
#define Rocket_Core_Matrix_H

#include <math.h>
#include <iostream>

#include "vector.h"
#include "mathconstants.h"

namespace Rocket {
	namespace Core {

		// All matrices are in row by column order:
		// 00 01 02 03
		// 10 11 12 13
		// 20 21 22 23
		// 30 31 32 33

		// Matrix 2x2
		// --------------------------------------------------------------------------------------------------------------------
		template <class T> class T_mat2 {
		public:
			T_vec2<T> m_rows[2];

			// Constructors
			T_mat2() { m_rows[0] = T_vec2<T>(1,0); m_rows[1] = T_vec2<T>(0,1); }
			T_mat2( T m00, T m01, T m10, T m11 ) { m_rows[0] = T_vec2<T>(m00,m01); m_rows[1] = T_vec2<T>(m10,m11); }
			T_mat2( T_vec2<T> r0, T_vec2<T> r1 ) { m_rows[0] = r0; m_rows[1] = r1; }

			// Array Subscript
			T_vec2<T> & operator [] ( int index ) { return m_rows[index]; }

			// Arithmetic
			// M * V
			T_vec2<T> operator * ( const T_vec2<T> & vector ) const {
				T_vec2<T> r(0,0);
				for (unsigned int i = 0; i < 2; i++) {
					for (unsigned int j = 0; j < 2; j++) {
						r.m_elements[i] += m_rows[i][j] * vector.m_elements[j];
					}	}
				return r;
			}
			// M * M
			T_mat2 operator * ( const T_mat2 & R ) const {
				T_mat2 r(0,0,0,0);
				for (unsigned int i = 0; i < 2; i++) {
					for (unsigned int j = 0; j < 2; j++) {
						for (unsigned int k = 0; k < 2; k++) {
							r.m_rows[i][j] += m_rows[i][k] * R.m_rows[k][j];
						}	}	}
				return r;
			}

			// Assignment Operators
			T_mat2 & operator *= ( const T_mat2 & R ) { *this = *this * R; return *this; }

			// Matrix Operations
			T_mat2<T> transpose() {
				return T_mat2( m_rows[0][0], m_rows[1][0], m_rows[0][1], m_rows[1][1] );
			}

			T determinant() {
				return m_rows[0][0]*m_rows[1][1] - m_rows[0][1]*m_rows[1][0];
			}

			T_mat2<T> inverse() {
				T adbc = 1/(m_rows[0][0]*m_rows[1][1] - m_rows[0][1]*m_rows[1][0]);
				return T_mat2( m_rows[1][1]*adbc, m_rows[0][1]*(-adbc), m_rows[1][0]*(-adbc), m_rows[0][0]*adbc );
			}
			
			
			// Conversion Operators
			template <class U> operator T_mat2<U>() const { return T_mat2<U>( T_vec2<U>((U)m_rows[0]), T_vec2<U>((U)m_rows[1]) ); }	// Cast to different typed T_mat2
			operator T * () const { return (T*)m_rows; }

			friend std::ostream & operator << ( std::ostream& stream, const T_mat2<T> & m ) {
				return stream << "[" << m.m_rows[0] << "\n " << m.m_rows[1] << "]";
			}
		};


		// Matrix 3x3
		// --------------------------------------------------------------------------------------------------------------------
		template <class T> class T_mat3 {
		public:
			T_vec3<T> m_rows[3];

			// Constructors
			T_mat3() { m_rows[0] = T_vec3<T>(1,0,0); m_rows[1] = T_vec3<T>(0,1,0); m_rows[2] = T_vec3<T>(0,0,1); }
			T_mat3( T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22 ) { m_rows[0] = T_vec3<T>(m00,m01,m02); m_rows[1] = T_vec3<T>(m10,m11,m12); m_rows[2] = T_vec3<T>(m20,m21,m22); }
			T_mat3( T_vec3<T> r0, T_vec3<T> r1, T_vec3<T> r2 ) { m_rows[0] = r0; m_rows[1] = r1; m_rows[2] = r2; }

			// Array Subscript
			T_vec3<T> & operator [] ( int index ) { return m_rows[index]; }

			// Arithmetic
			// M * V
			T_vec3<T> operator * ( const T_vec3<T> & vector ) const {
				T_vec3<T> r(0,0,0);
				for (unsigned int i = 0; i < 3; i++) {
					for (unsigned int j = 0; j < 3; j++) {
						r.m_elements[i] += m_rows[i][j] * vector.m_elements[j];
					}	}
				return r;
			}
			// M * M
			T_mat3 operator * ( const T_mat3 & R ) const {
				T_mat3 r(0,0,0,0,0,0,0,0,0);
				for (unsigned int i = 0; i < 3; i++) {
					for (unsigned int j = 0; j < 3; j++) {
						for (unsigned int k = 0; k < 3; k++) {
							r.m_rows[i][j] += m_rows[i][k] * R.m_rows[k][j];
						}	}	}
				return r;
			}

			// Assignment Operators
			T_mat3 & operator *= ( const T_mat3 & R ) { *this = *this * R; return *this; }

			// Matrix Operations
			T_mat3<T> transpose() {
				return T_mat3( m_rows[0][0], m_rows[1][0], m_rows[2][0],
								m_rows[0][1], m_rows[1][1], m_rows[2][1],
								m_rows[0][2], m_rows[1][2], m_rows[2][2] );
			}

			T determinant() {
				return m_rows[0][0] * m_rows[1][1] * m_rows[2][2]
						- m_rows[0][0] * m_rows[1][2] * m_rows[2][1]
						- m_rows[0][1] * m_rows[1][0] * m_rows[2][2]
						+ m_rows[0][1] * m_rows[1][2] * m_rows[2][0]
						+ m_rows[0][2] * m_rows[1][0] * m_rows[2][1]
						- m_rows[0][2] * m_rows[1][1] * m_rows[2][0];
			}

			// Returns this matrix with row and col removed
			T_mat2<T> minor( int row, int col ) {
				T_mat2<T> r;
				int x = 0, y = 0;
				for (int i = 0; i < 3; i++) {
					if (i != row) {
						for (int j = 0; j < 3; j++) {
							if (j != col) {
								r[x][y] = m_rows[i][j];
								y++;
							}
						}
						x++;
						y = 0;
					}
				}
				return r;
			}

			T minorDeterminant( int row, int col ) {
				int r0 = ( row == 0 ) ? 1 : 0;
				int r1 = ( row == 1 ) ? 2 : 1+r0;
				int c0 = ( col == 0 ) ? 1 : 0;
				int c1 = ( col == 1 ) ? 2 : 1+c0;
				//return m_rows[0][0]*m_rows[1][1] - m_rows[0][1]*m_rows[1][0];
				return m_rows[r0][c0]*m_rows[r1][c1] - m_rows[r0][c1]*m_rows[r1][c0];
			}

			T_mat3<T> inverse() {
				T det = 1 / determinant();
				T negdet = -det;
				return T_mat3<T>(
					minorDeterminant(0,0)*det,		minorDeterminant(1,0)*negdet,	minorDeterminant(2,0)*det,
					minorDeterminant(0,1)*negdet,	minorDeterminant(1,1)*det,		minorDeterminant(2,1)*negdet,
					minorDeterminant(0,2)*det,		minorDeterminant(1,2)*negdet,	minorDeterminant(2,2)*det );
			}
			
			
			// Conversion Operators
			template <class U> operator T_mat3<U>() const { return T_mat3<U>( T_vec3<U>((U)m_rows[0]), T_vec3<U>((U)m_rows[1]), T_vec3<U>((U)m_rows[2]) ); }	// Cast to different typed T_mat3
			operator T * () const { return (T*)m_rows; }

			friend std::ostream & operator << ( std::ostream& stream, const T_mat3<T> & m ) {
				return stream << "[" << m.m_rows[0] << "\n " << m.m_rows[1] << "\n " << m.m_rows[2] << "]";
			}
		};


		// Matrix 4x4
		// --------------------------------------------------------------------------------------------------------------------
		template <class T> class T_mat4 {
		public:
			T_vec4<T> m_rows[4];

			// Constructors
			T_mat4() { m_rows[0] = T_vec4<T>(1,0,0,0); m_rows[1] = T_vec4<T>(0,1,0,0); m_rows[2] = T_vec4<T>(0,0,1,0); m_rows[3] = T_vec4<T>(0,0,0,1); }
			T_mat4( T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33 ) {
				m_rows[0] = T_vec4<T>(m00,m01,m02,m03);
				m_rows[1] = T_vec4<T>(m10,m11,m12,m13);
				m_rows[2] = T_vec4<T>(m20,m21,m22,m23);
				m_rows[3] = T_vec4<T>(m30,m31,m32,m33);
			}
			T_mat4( T_vec4<T> r0, T_vec4<T> r1, T_vec4<T> r2, T_vec4<T> r3 ) { m_rows[0] = r0; m_rows[1] = r1; m_rows[2] = r2; m_rows[3] = r3; }

			// Array Subscript
			T_vec4<T> & operator [] ( int index ) { return m_rows[index]; }

			// Arithmetic
			// M * V
			T_vec4<T> operator * ( const T_vec4<T> & vector ) const {
				T_vec4<T> r(0,0,0,0);
				for (unsigned int i = 0; i < 4; i++) {
					for (unsigned int j = 0; j < 4; j++) {
						r.m_elements[i] += m_rows[i][j] * vector.m_elements[j];
					}	}
				return r;
			}
			// M * M
			T_mat4 operator * ( const T_mat4 & R ) const {
				T_mat4 r(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
				for (unsigned int i = 0; i < 4; i++) {
					for (unsigned int j = 0; j < 4; j++) {
						for (unsigned int k = 0; k < 4; k++) {
							r.m_rows[i][j] += m_rows[i][k] * R.m_rows[k][j];
						}	}	}
				return r;
			}

			// Assignment Operators
			T_mat4 & operator *= ( const T_mat4 & R ) { *this = *this * R; return *this; }

			// Matrix Operations
			T_mat4<T> transpose() {
				return T_mat4( m_rows[0][0], m_rows[1][0], m_rows[2][0], m_rows[3][0],
								m_rows[0][1], m_rows[1][1], m_rows[2][1], m_rows[3][1],
								m_rows[0][2], m_rows[1][2], m_rows[2][2], m_rows[3][2],
								m_rows[0][3], m_rows[1][3], m_rows[2][3], m_rows[3][3] );
			}

			T determinant() {
				return m_rows[0][0] * minorDeterminant(0,0) - m_rows[0][1] * minorDeterminant(0,1) + m_rows[0][2] * minorDeterminant(0,2) - m_rows[0][3] * minorDeterminant(0,3);
			}

			// Returns this matrix with row and col removed
			T_mat3<T> minor( int row, int col ) {
				T_mat3<T> r;
				int x = 0, y = 0;
				for (int i = 0; i < 4; i++) {
					if (i != row) {
						for (int j = 0; j < 4; j++) {
							if (j != col) {
								r[x][y] = m_rows[i][j];
								y++;
							}
						}
						x++;
						y = 0;
					}
				}
				return r;
			}

			T minorDeterminant( int row, int col ) {
				int r0 = ( row == 0 ) ? 1 : 0;
				int c0 = ( col == 0 ) ? 1 : 0;
				int r1 = ( row == 1 ) ? 2 : 1+r0;
				int c1 = ( col == 1 ) ? 2 : 1+c0;
				int r2 = ( row == 2 ) ? 3 : 1+r1;
				int c2 = ( col == 2 ) ? 3 : 1+c1;
				return m_rows[r0][c0] * m_rows[r1][c1] * m_rows[r2][c2]
						- m_rows[r0][c0] * m_rows[r1][c2] * m_rows[r2][c1]
						- m_rows[r0][c1] * m_rows[r1][c0] * m_rows[r2][c2]
						+ m_rows[r0][c1] * m_rows[r1][c2] * m_rows[r2][c0]
						+ m_rows[r0][c2] * m_rows[r1][c0] * m_rows[r2][c1]
						- m_rows[r0][c2] * m_rows[r1][c1] * m_rows[r2][c0];
			}

			T_mat4<T> inverse() {
				T det = 1 / determinant();
				T negdet = -det;
				return T_mat4<T>(
					minorDeterminant(0,0)*det,		minorDeterminant(1,0)*negdet,	minorDeterminant(2,0)*det,		minorDeterminant(3,0)*negdet,
					minorDeterminant(0,1)*negdet,	minorDeterminant(1,1)*det,		minorDeterminant(2,1)*negdet,	minorDeterminant(3,1)*det,
					minorDeterminant(0,2)*det,		minorDeterminant(1,2)*negdet,	minorDeterminant(2,2)*det,		minorDeterminant(3,2)*negdet,
					minorDeterminant(0,3)*negdet,	minorDeterminant(1,3)*det,		minorDeterminant(2,3)*negdet,	minorDeterminant(3,3)*det );
			}
			
			
			// Conversion Operators
			template <class U> operator T_mat4<U>() const { return T_mat4<U>( T_vec4<U>((U)m_rows[0]), T_vec4<U>((U)m_rows[1]), T_vec4<U>((U)m_rows[2]), T_vec4<U>((U)m_rows[3]) ); }	// Cast to different typed T_mat4
			operator T * () const { return (T*)m_rows; }

			friend std::ostream & operator << ( std::ostream& stream, const T_mat4<T> & m ) {
				return stream << "[" << m.m_rows[0] << "\n " << m.m_rows[1] << "\n " << m.m_rows[2] << "\n " << m.m_rows[3] << "]";
			}
		};


		// Matrix NxM
		// --------------------------------------------------------------------------------------------------------------------
		template <class T> class T_matNxM {
		public:
			T ** m_rows;
			unsigned int m_N, m_M;

			// Constructors
			T_matNxM( unsigned int rows, unsigned int columns ) {
				m_N = rows;
				m_M = columns;
				m_rows = new T * [rows];
				for ( unsigned int r = 0; r < rows; r++ ) {
					m_rows[r] = new T[columns];
					for ( unsigned int c = 0; c < columns; c++ ) { m_rows[r][c] = 0; }
				}
			}
			~T_matNxM() {
				for ( unsigned int r = 0; r < m_N; r++ ) {
					delete m_rows[r];
				}
				delete m_rows;
			}

			// Array Subscript
			T * operator [] ( unsigned int index ) { return m_rows[index]; }

			// Arithmetic
			// M * V
			void multiplyVector ( const T * vector, T * outvector ) const {
				for (unsigned int i = 0; i < m_N; i++) {
					for (unsigned int j = 0; j < m_M; j++) {
						outvector[i] += m_rows[i][j] * vector[j];
					}	}
				return outvector;
			}
			// M * M
			void multiplyMatrix ( const T_matNxM<T> & R, T_matNxM<T> * outmatrix ) const {
				if ( ( m_N != R.m_M ) || ( m_M != R.m_N ) ) return NULL;
				for (unsigned int i = 0; i < m_N; i++) {
					for (unsigned int j = 0; j < R.m_M; j++) {
						for (unsigned int k = 0; k < m_M; k++) {
							outmatrix[i][j] += m_rows[i][k] * R.m_rows[k][j];
						}	}	}
				return outmatrix;
			}

			// Matrix Operations
			void transpose() {
				// todo
				return;
			}

			T determinant() {
				T r = 0;
				int sign = 1;
				for ( unsigned int i = 0; i < m_M; i++ ) {
					r += sign * m_rows[0][i] * minorDeterminant(0,i);
					sign *= -1;
				}
				return r;
			}

			// Returns this matrix with row and col removed
			T_matNxM<T> * minor( unsigned int row, unsigned int col ) {
				T_matNxM<T> r = new T_matNxM<T>( m_N, m_M );
				int x = 0, y = 0;
				for ( unsigned int i = 0; i < m_N; i++) {
					if (i != row) {
						for ( unsigned int j = 0; j < m_M; j++) {
							if (j != col) {
								r[x][y] = m_rows[i][j];
								y++;
							}
						}
						x++;
						y = 0;
					}
				}
				return r;
			}

			T minorDeterminant( unsigned int row, unsigned int col ) {
				/*
				int r0 = ( row == 0 ) ? 1 : 0;
				int c0 = ( col == 0 ) ? 1 : 0;
				int r1 = ( row == 1 ) ? 2 : 1+r0;
				int c1 = ( col == 1 ) ? 2 : 1+c0;
				int r2 = ( row == 2 ) ? 3 : 1+r1;
				int c2 = ( col == 2 ) ? 3 : 1+c1;
				return m_rows[r0][c0] * m_rows[r1][c1] * m_rows[r2][c2]
						- m_rows[r0][c0] * m_rows[r1][c2] * m_rows[r2][c1]
						- m_rows[r0][c1] * m_rows[r1][c0] * m_rows[r2][c2]
						+ m_rows[r0][c1] * m_rows[r1][c2] * m_rows[r2][c0]
						+ m_rows[r0][c2] * m_rows[r1][c0] * m_rows[r2][c1]
						- m_rows[r0][c2] * m_rows[r1][c1] * m_rows[r2][c0];
				*/
				// todo
				return 0;
			}

			void inverse() {
				/*
				T det = 1 / determinant();
				T negdet = -det;
				return T_mat4<T>(
					minorDeterminant(0,0)*det,		minorDeterminant(1,0)*negdet,	minorDeterminant(2,0)*det,		minorDeterminant(3,0)*negdet,
					minorDeterminant(0,1)*negdet,	minorDeterminant(1,1)*det,		minorDeterminant(2,1)*negdet,	minorDeterminant(3,1)*det,
					minorDeterminant(0,2)*det,		minorDeterminant(1,2)*negdet,	minorDeterminant(2,2)*det,		minorDeterminant(3,2)*negdet,
					minorDeterminant(0,3)*negdet,	minorDeterminant(1,3)*det,		minorDeterminant(2,3)*negdet,	minorDeterminant(3,3)*det );
				*/
				// todo
				return;
			}
			

			friend std::ostream & operator << ( std::ostream& stream, const T_matNxM<T> & m ) {
				stream << "[";
				for ( unsigned int i = 0; i < m.m_N; i++ ) {
					stream << "[";
					for ( unsigned int j = 0; j < m.m_M; j++ ) {
						if ( j > 0 ) stream << ",";
						stream << m.m_rows[i][j];
					}
					stream << "]";
					if ( i < m.m_N-1 ) stream << "\n";
				}
				return stream << "]";
			}
		};


		// External Matrix Operations
		// --------------------------------------------------------------------------------------------------------------------
		template <class T> T_mat4<T> Scale( const T_vec3<T> & v ) {
			mat4 r;
			r[0][0] = v.x;
			r[1][1] = v.y;
			r[2][2] = v.z;
			return r;
		}

		template <class T> T_mat4<T> Translate( const T_vec3<T> & v ) {
			mat4 r;
			r[0][3] = v.x;
			r[1][3] = v.y;
			r[2][3] = v.z;
			return r;
		}

		template <class T> T_mat4<T> QuaternionRotate( T_vec4<T> quat ) {
			T x = quat.x;
			T y = quat.y;
			T z = quat.z;
			T w = quat.w;

			T_mat4<T> r;
			r[0][0] = 1 - 2*(y*y + z*z);	r[0][1] = 2*(x*y - w*z);		r[0][2] = 2*(x*z + w*y);
			r[1][0] = 2*(x*y + w*z);		r[1][1] = 1 - 2*(x*x + z*z);	r[1][2] = 2*(y*z - w*x);
			r[2][0] = 2*(x*z - w*y);		r[2][1] = 2*(y*z + w*x);		r[2][2] = 1 - 2*(x*x + y*y);

			return r;
		}

		template <class T> T_mat4<T> Rotate( const T angle, const T_vec3<T> & axis ) {
			return QuaternionRotate( Quaternion( angle, axis ) );
		}

		template <class T> T_vec4<T> MatrixToQuaternion( T_mat4<T> m ) {
			float trace = m[0][0] + m[1][1] + m[2][2] + 1.0f;

			if (trace > MathConstants::ZeroTolerance) {
				float s = sqrt( trace ) * 2.0f;
				return normalize( T_vec4<T>( (m[2][1] - m[1][2]) / s, (m[0][2] - m[2][0]) / s, (m[1][0] - m[0][1]) / s, s / 4.0f ) );
			} else if ( (m[0][0] > m[1][1]) && (m[0][0] > m[2][2]) ) {
				float s = sqrt( m[0][0] - m[1][1] - m[2][2] + 1.0f ) * 2.0f;
				return normalize( T_vec4<T>( s / 4.0f, (m[1][0] + m[0][1]) / s, (m[0][2] + m[2][0]) / s, (m[2][1] - m[1][2]) / s ) );
			} else if (m[1][1] > m[2][2]) {
				float s = sqrt( m[1][1] - m[0][0] - m[2][2] + 1.0f ) * 2.0f;
				return normalize( T_vec4<T>( (m[1][0] + m[0][1]) / s, s / 4.0f, (m[2][1] + m[1][2]) / s, (m[0][2] - m[2][0]) / s ) );
			} else {
				float s = sqrt( m[2][2] - m[0][0] - m[1][1] + 1.0f ) * 2.0f;
				return normalize( T_vec4<T>( (m[0][2] + m[2][0]) / s, (m[2][1] + m[1][2]) / s, s / 4.0f, (m[1][0] - m[0][1]) / s ) );
			}
		}

		template <class T> T_vec3<T> QuaternionToUnitVector( const T_vec4<T> & quat ) {
			T_vec4<T> r = QuaternionRotate( quat ) * T_vec4<T>( 0.0f, 0.0f, -1.0f, 0.0f );
			return T_vec3<T>( -r.x, -r.y, r.z );
		}


		// Camera Matrix Operations
		// --------------------------------------------------------------------------------------------------------------------
		/*template <class T> T_mat4<T> LookAt( const T_vec4<T> & look, const T_vec4<T> & up, const T_vec4<T> & position ) {
			T_vec4<T> n = normalize( look - position );
			T_vec4<T> u = T_vec4<T>( normalize( cross(up,n) ), 0.0 );
			T_vec4<T> v = T_vec4<T>( normalize( cross(n,u) ), 0.0 );
			T_mat4<T> r = T_mat4<T>( u, v, n, T_vec4<T>( 0.0, 0.0, 0.0, 1.0 ) );
			return r * Translate( -look );
		}*/


		// Projection Matrix Operations
		// --------------------------------------------------------------------------------------------------------------------
		template <class T> T_mat4<T> Orthographic( const T left, const T right, const T bottom, const T top, const T zNear, const T zFar ) {
			T_mat4<T> r;
			r[0][0] = 2.0f / (right - left);
			r[1][1] = 2.0f / (top - bottom);
			r[2][2] = 2.0f / (zNear - zFar);
			r[3][3] = 1.0f;
			r[0][3] = -(right + left) / (right - left);
			r[1][3] = -(top + bottom) / (top - bottom);
			r[2][3] = -(zFar + zNear) / (zFar - zNear);
			return r;
		}

		template <class T> T_mat4<T> Orthographic2D( const T left, const T right, const T bottom, const T top ) {
			return Orthographic( left, right, bottom, top, -1.0f, 1.0f );
		}

		template <class T> T_mat4<T> Frustum( const T left, const T right, const T bottom, const T top, const T zNear, const T zFar ) {
			T_mat4<T> r;
			r[0][0] = 2.0f*zNear / (right - left);
			r[0][2] = (right + left) / (right - left);
			r[1][1] = 2.0f*zNear / (top - bottom);
			r[1][2] = (top + bottom) / (top - bottom);
			r[2][2] = -(zFar + zNear) / (zFar - zNear);
			r[2][3] = -2.0f*zFar*zNear / (zFar - zNear);
			r[3][2] = -1.0f;
			r[3][3] = 0.0f;
			return r;
		}

		template <class T> T_mat4<T> Perspective( const T FOV_Y, const T AspectRatio, const T zNear, const T zFar) {
			T top = tan( FOV_Y * MathConstants::DegreesToRadians * 0.5f ) * zNear;
			T right = top * AspectRatio;

			T_mat4<T> r;
			r[0][0] = zNear / right;
			r[1][1] = zNear / top;
			r[2][2] = -(zFar + zNear) / (zFar - zNear);
			r[2][3] = -2.0f*zFar*zNear / (zFar - zNear);
			r[3][2] = -1.0f;
			return r;
		}


		// Typedefs
		// --------------------------------------------------------------------------------------------------------------------
		typedef T_mat2<float> mat2;
		typedef T_mat2<double> mat2d;

		typedef T_mat3<float> mat3;
		typedef T_mat3<double> mat3d;

		typedef T_mat4<float> mat4;
		typedef T_mat4<double> mat4d;

		typedef T_matNxM<float> matNxM;
		typedef T_matNxM<double> matNxMd;
	}
}

#endif