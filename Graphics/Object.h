
#ifndef Rocket_Graphics_Object_H
#define Rocket_Graphics_Object_H

#include "matrix.h"
#include "Mesh.h"
#include "Transform.h"
#include "Shader.h"

namespace Rocket {
	namespace Graphics {

		class Scene;
		class Object : public Transform {
		public:
			Object( Mesh * mesh = NULL );
			~Object();

			// Creates a copy of this object, sharing the mesh, and copying all object properties (transforms not included)
			Object * clone();
			// Clones the object into the specified Scene (transforms included)
			Object * cloneInScene( Scene * scene, Transform * parent, Core::vec3 scale, Core::vec4 rotation, Core::vec3 position );

			//void draw( Scene * drawer, Core::mat4 parent_orientation, bool opaquePass );
			void draw();
			// Calculate the object's world matrices before calling draw()
			void calculateTransforms( float elapsedMilliseconds, const Core::mat4 & parent_orientation, bool parentCacheIsClean, bool applyUpdates ) override;

			void setMesh( Mesh * mesh );
			Mesh * getMesh();

			void setShaderUniforms( ShaderUniforms * shaderUniforms );
			ShaderUniforms * getShaderUniforms();

			void enableTransparency();
			void disableTransparency();
			bool isTransparent();

		protected:
			Mesh * m_mesh;
			ShaderUniforms * m_shaderUniforms;

			bool m_transparencyEnabled;

			void clonePropertiesOnto( Object * cloneOnto );
		};

	}
}


#endif