
#include <vector>

#include "Raster.h"
#include "Scene.h"

namespace Rocket {
	namespace Graphics {

		//! Uses transform as the Transform to perform positioning, scaling, and rotation for this Raster
		Raster::Raster( Transform * transform ) {
			m_rasterTransform = transform;
		}
		Raster::~Raster() {
		}

		//! Returns the Transform used for this Raster
		Transform * Raster::getTransform() {
			return m_rasterTransform;
		}

		//! Sets the render size of this Raster in pixels
		void Raster::setSize( Core::vec2i size ) {
			m_size = size;
			m_rasterTransform->scale( Core::vec3( (float)size.x(), (float)size.y(), 1.0f ) );
		}
		//! Returns the render size of this Raster in pixels
		Core::vec2i Raster::getSize() {
			return m_size;
		}

		//! Sets the position of this Raster in screen coordinates
		void Raster::setPosition( Core::vec2i pos ) {
			// Preserve z-axis for z-indexing
			m_rasterTransform->position( Core::vec3( pos.x() + m_size.x()/2.0f, pos.y() + m_size.y()/2.0f, m_rasterTransform->position().z() ) );
			m_position = pos;
		}
		//! Returns the position of this Raster in screen coordinates
		Core::vec2i Raster::getPosition() {
			return m_position;
		}

		//! Rotate this Raster by deltaAngle
		void Raster::rotate( float deltaAngle ) {
			m_angle += deltaAngle;
			m_rasterTransform->rotateRoll( deltaAngle );
		}
		//! Set the angle of rotation of this Raster
		void Raster::setAngle( float angle ) {
			m_rasterTransform->rotateRoll( angle - m_angle );
			m_angle = angle;
		}
		//! Returns the angle of rotation of this Raster
		float Raster::getAngle() {
			return m_angle;
		}

		//! Prevents this Raster from rendering
		void Raster::hide() {
			m_rasterTransform->hide();
		}
		//! Shows this Raster during rendering
		void Raster::show() {
			m_rasterTransform->show();
		}
		//! Returns true if this Raster is visible
		bool Raster::isVisible() {
			return m_rasterTransform->isVisible();
		}

		//! Adds this Raster to a zIndexer at the given location
		void Raster::zIndexer_Add( zIndexerType * zIndexer, int zIndexTag ) {
			m_rasterTransform->zIndexer_Add( zIndexer, zIndexTag );
		}
		//! Adds this Raster behind the given Transform (in the same zIndexer)
		void Raster::zIndexer_AddBehind( Transform * relativeTo ) {
			m_rasterTransform->zIndexer_AddBehind( relativeTo, NoZIndexTag );
		}
		//! Adds this Raster in front of the given Transform (in the same zIndexer)
		void Raster::zIndexer_AddInFront( Transform * relativeTo ) {
			m_rasterTransform->zIndexer_AddInFront( relativeTo, NoZIndexTag );
		}

	}
}
