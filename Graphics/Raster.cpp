
#include <vector>

#include "Raster.h"
#include "Scene.h"

namespace Rocket {
	namespace Graphics {

		Raster::Raster( Transform * transform ) {
			m_rasterTransform = transform;
		}
		Raster::~Raster() {
		}

		Transform * Raster::getTransform() {
			return m_rasterTransform;
		}

		void Raster::setSize( Core::vec2i size ) {
			m_size = size;
			m_rasterTransform->scale( Core::vec3( (float)size.x(), (float)size.y(), 1.0f ) );
		}
		Core::vec2i Raster::getSize() {
			return m_size;
		}

		void Raster::setPosition( Core::vec2i pos ) {
			// Preserve z-axis for z-indexing
			m_rasterTransform->position( Core::vec3( pos.x() + m_size.x()/2.0f, pos.y() + m_size.y()/2.0f, m_rasterTransform->position().z() ) );
			m_position = pos;
		}
		Core::vec2i Raster::getPosition() {
			return m_position;
		}

		void Raster::rotate( float deltaAngle ) {
			m_angle += deltaAngle;
			m_rasterTransform->rotateRoll( deltaAngle );
		}
		void Raster::setAngle( float angle ) {
			m_rasterTransform->rotateRoll( angle - m_angle );
			m_angle = angle;
		}
		float Raster::getAngle() {
			return m_angle;
		}

		void Raster::hide() {
			m_rasterTransform->hide();
		}
		void Raster::show() {
			m_rasterTransform->show();
		}
		bool Raster::isVisible() {
			return m_rasterTransform->isVisible();
		}

		void Raster::zIndexer_Add( zIndexerType * zIndexer, int zIndexTag ) {
			m_rasterTransform->zIndexer_Add( zIndexer, zIndexTag );
		}
		void Raster::zIndexer_AddBehind( Transform * relativeTo ) {
			m_rasterTransform->zIndexer_AddBehind( relativeTo, NoZIndexTag );
		}
		void Raster::zIndexer_AddInFront( Transform * relativeTo ) {
			m_rasterTransform->zIndexer_AddInFront( relativeTo, NoZIndexTag );
		}

	}
}
