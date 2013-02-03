
#ifndef Rocket_Graphics_Raster_H
#define Rocket_Graphics_Raster_H

#include "Transform.h"
#include "vector.h"

namespace Rocket {
	namespace Graphics {

		// Raster - An abstract base class for anything drawn in pixel space
		class Raster {
		public:
			Raster( Transform * transform );	// transform - the transform to perform positioning, scaling, and rotation
			virtual ~Raster();
			virtual void destroyFromBaseClass();

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

			void zIndexer_Add( std::list<std::pair< Transform*, int >> * zIndexer, int zIndexTag );
			void zIndexer_AddBehind( Transform * relativeTo );
			void zIndexer_AddInFront( Transform * relativeTo );

		protected:
			Transform * m_rasterTransform;

			Core::vec2i m_size;
			Core::vec2i m_position;
			float m_angle;
		};

	}
}


#endif