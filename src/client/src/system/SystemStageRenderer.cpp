#include <client/system/SystemStageRenderer.hpp>

#include <tempo/component/ComponentStage.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h> // :TODO: sort out includes

#include <glm/vec2.hpp>

#include <cstdio>

namespace {
	class FloorSceneNode : public irr::scene::ISceneNode {
	private:
		irr::core::aabbox3d<irr::f32>    bounding_box;
		irr::video::SMaterial            material;
		irr::video::S3DVertex*           vertices;
		irr::u32*                        indices;
		irr::u32                         num_tiles;
	public:
		FloorSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* smgr, tempo::stage_tiles& heights)
			: ISceneNode(parent, smgr) {

			bounding_box = irr::core::aabbox3d<irr::f32>();

			material.Wireframe = false;
			material.Lighting  = true;
			material.BackfaceCulling = false; // :TODO: re-enable

			num_tiles = heights.size();

			vertices = new irr::video::S3DVertex[heights.size() * 4]; // 4 verticies per tile
			indices  = new irr::u32             [heights.size() * 6]; // 2 triangles, so 6 verts per tile

			for(int i = 0; i < heights.size(); ++i){
				float x      = std::get<0>(heights[i]).x;
				float y      = std::get<0>(heights[i]).y;
				float height = std::get<1>(heights[i]);

				vertices[i*4 + 0] = irr::video::S3DVertex(x - 0.5f, height, y - 0.5f,  0,1,0,  irr::video::SColor(1, 1, 1, 1),  0,0);
				vertices[i*4 + 1] = irr::video::S3DVertex(x - 0.5f, height, y + 0.5f,  0,1,0,  irr::video::SColor(1, 1, 1, 1),  0,1);
				vertices[i*4 + 2] = irr::video::S3DVertex(x + 0.5f, height, y - 0.5f,  0,1,0,  irr::video::SColor(1, 1, 1, 1),  1,0);
				vertices[i*4 + 3] = irr::video::S3DVertex(x + 0.5f, height, y + 0.5f,  0,1,0,  irr::video::SColor(1, 1, 1, 1),  1,1);

				indices [i*6 + 0] = i*4 + 0;
				indices [i*6 + 1] = i*4 + 1;
				indices [i*6 + 2] = i*4 + 2;
				indices [i*6 + 3] = i*4 + 1;
				indices [i*6 + 4] = i*4 + 2;
				indices [i*6 + 5] = i*4 + 3;

				bounding_box.addInternalPoint(x - 0.5f, height, y - 0.5f);
				bounding_box.addInternalPoint(x + 0.5f, height, y + 0.5f);
			}
		}

		virtual void OnRegisterSceneNode() {
			if (IsVisible){
				SceneManager->registerNodeForRendering(this);
			}

			ISceneNode::OnRegisterSceneNode();
		}

		virtual void render() {
			irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

			driver->setMaterial(material);
			driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
			driver->drawVertexPrimitiveList(&vertices[0],
			                                num_tiles * 4, // num verticies
			                                &indices[0],
			                                num_tiles * 2, // num primitives
			                                irr::video::EVT_STANDARD,
			                                irr::scene::EPT_TRIANGLES,
			                                irr::video::EIT_32BIT
			                                );
		}

		virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const {
			return bounding_box;
		}

		virtual irr::u32 getMaterialCount() const {
			return 1;
		}

		virtual irr::video::SMaterial& getMaterial(irr::u32 i) {
			return material;
		}
	};
}

namespace client {
	void SystemStageRenderer::setup(irr::scene::ISceneManager* smgr){
		printf("SystemStageRenderer initializing\n");

		// generate testing hills plane mesh
		//irr::scene::IAnimatedMesh* mesh_hills = smgr->addHillPlaneMesh("test_hill_mesh",
		//                                                               irr::core::dimension2d<irr::f32>(  1,   1), // tile size
		//                                                               irr::core::dimension2d<irr::u32>(100, 100), // tile count
		//                                                               nullptr,  // material
		//                                                               1.0f      // hill height
		//                                                               );
		//irr::scene::IAnimatedMeshSceneNode* node_hills = smgr->addAnimatedMeshSceneNode(mesh_hills);
		//if (node_hills){
		//	node_hills->setPosition(irr::core::vector3df(0, 0, 0));
		//}

		auto entities = getEntities();
		auto entity = std::begin(entities);
		auto& stage   = entity->getComponent<tempo::ComponentStage>();

		tempo::stage_tiles tiles = stage.getHeights();

		FloorSceneNode* floor_node = new FloorSceneNode(smgr->getRootSceneNode(), smgr, tiles);
	}
} // namespace client
