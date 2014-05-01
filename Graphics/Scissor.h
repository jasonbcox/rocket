
#ifndef Rocket_Graphics_Scissor_H
#define Rocket_Graphics_Scissor_H

#include "Scene.h"

#include "rocket/Core/debug.h"

namespace Rocket {
	namespace Graphics {

		class Scissor : public Scene {
		public:
			Scissor( float orthoWidth, float orthoHeight,
				int topLeftX, int topLeftY, int regionWidth, int regionHeight );
			Scissor( float FOVy, float aspectRatio, float nearClip, float farClip,
				int topLeftX, int topLeftY, int regionWidth, int regionHeight );
			~Scissor();

			virtual void draw( float elapsedMilliseconds, bool clearScreen );
			// elapsedMilliseconds since the last call to draw(). This value is used to correctly interpolate movement of objects within the scene.
	
		protected:
			static bool g_scissorState;
			static int g_scissorRegion[4];

			int m_topLeftX, m_topLeftY, m_width, m_height;

		};
	}
}


#endif
