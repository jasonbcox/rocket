
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Scissor.h"
#include "Universe.h"

#include "rocket/Core/debug.h"

namespace Rocket {
	namespace Graphics {

		bool Scissor::g_scissorState = false;
		int Scissor::g_scissorRegion[] = { 0, 0, 0, 0 };

		//! Create an orthographic Scissor
		Scissor::Scissor( float orthoWidth, float orthoHeight,
			int topLeftX, int topLeftY, int regionWidth, int regionHeight ) : Scene( orthoWidth, orthoHeight ) {
			m_topLeftX = topLeftX;
			m_topLeftY = topLeftY;
			m_width = regionWidth;
			m_height = regionHeight;
		}
		//! Create a perspective Scissor
		Scissor::Scissor( float FOVy, float aspectRatio, float nearClip, float farClip,
			int topLeftX, int topLeftY, int regionWidth, int regionHeight ) : Scene( FOVy, aspectRatio, nearClip, farClip ) {
			m_topLeftX = topLeftX;
			m_topLeftY = topLeftY;
			m_width = regionWidth;
			m_height = regionHeight;

			// Todo: Consider centering the viewpoint on the scissor region by default
		}
		Scissor::~Scissor() {
		}

		//! Render this Scissor
		void Scissor::draw( float elapsedMilliseconds, bool clearScreen ) {
			bool parentState = g_scissorState;
			int parentRegion[4];
			if ( parentState == true ) {
				parentRegion[0] = g_scissorRegion[0];
				parentRegion[1] = g_scissorRegion[1];
				parentRegion[2] = g_scissorRegion[2];
				parentRegion[3] = g_scissorRegion[3];
			}

			glEnable( GL_SCISSOR_TEST );
			g_scissorState = true;
			g_scissorRegion[0] = m_topLeftX;
			g_scissorRegion[1] = WindowHeight - m_topLeftY - m_height;
			g_scissorRegion[2] = m_width;
			g_scissorRegion[3] = m_height;
			glScissor( g_scissorRegion[0], g_scissorRegion[1], g_scissorRegion[2], g_scissorRegion[3] );
			Scene::draw( elapsedMilliseconds, clearScreen );

			g_scissorState = parentState;
			if ( parentState == true ) {
				glScissor( parentRegion[0], parentRegion[1], parentRegion[2], parentRegion[3] );
				g_scissorRegion[0] = parentRegion[0];
				g_scissorRegion[1] = parentRegion[1];
				g_scissorRegion[2] = parentRegion[2];
				g_scissorRegion[3] = parentRegion[3];
			} else {
				glDisable( GL_SCISSOR_TEST );
			}
		}

	}
}

