
#ifndef Rocket_Graphics_Raster_H
#define Rocket_Graphics_Raster_H

#include <memory>

using namespace std;

#include "Transform.h"
#include "rocket/Core/vector.h"

namespace Rocket {
	namespace Graphics {

		// Raster - An abstract base class for anything drawn in pixel space
		class Raster {
		public:
			Raster( Transform * transform );	// transform - the transform to perform positioning, scaling, and rotation
			virtual ~Raster();

			Transform * getTransform();

			void setSize( Core::vec2i size );
			Core::vec2i getSize();

			void setPosition( Core::vec2i pos );
			Core::vec2i getPosition();

			void rotate( float deltaAngle );
			void setAngle( float angle );
			float getAngle();

			void hide();
			void show();
			bool isVisible();

			void zIndexer_Add( zIndexerType * zIndexer, int zIndexTag );
			void zIndexer_AddBehind( Transform * relativeTo );
			void zIndexer_AddInFront( Transform * relativeTo );

		protected:
			// Not shared for two reasons:
			// 1) Raster is an abstract class and shouldn't own any pointers because some other container should be holding the Transform object
			// 2) Sprite's constructor fails when it tries to initialize itself (the Transform) and Raster (if m_rasterTransform is a smart pointer)
			Transform * m_rasterTransform;

			Core::vec2i m_size;
			Core::vec2i m_position;
			float m_angle;
		};

	}
}


#endif