
#ifndef Rocket_Graphics_Object_H
#define Rocket_Graphics_Object_H

#include <vector>
#include <memory>

using namespace std;

#include "rocket/Core/matrix.h"
#include "Mesh.h"
#include "Transform.h"
#include "Shader.h"
#include "Raster.h"

namespace Rocket {
	namespace Graphics {

		class Scene;
		class Object : public Transform { //, public enable_shared_from_this< Object > {
		public:
			Object( Mesh * mesh = nullptr );
			virtual ~Object();

			shared_ptr< Object > clone();
			shared_ptr< Object > cloneInScene( Scene * scene, Transform * parent, Core::vec3 scale, Core::vec4 rotation, Core::vec3 position );

			void calculateTransforms( float elapsedMilliseconds, const Core::mat4 & parent_orientation, bool parentCacheIsClean, bool applyUpdates ) override;

			Mesh * getMesh();

			void setShaderUniforms( ShaderUniforms * shaderUniforms );
			ShaderUniforms * getShaderUniforms();

			void enableTransparency();
			void disableTransparency();
			bool isTransparent();

			void removeOwner( Scene * scene );

		protected:
			shared_ptr< Mesh > m_mesh;
			shared_ptr< ShaderUniforms > m_shaderUniforms;

			bool m_transparencyEnabled;

			void clonePropertiesOnto( Object * cloneOnto );
		};

	}
}


#endif
