////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \brief Contains custom scene node which can render some section of floor
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_IRRLICHT_TEST_FLOORSCENENODE_HPP
#define TEMPO_IRRLICHT_TEST_FLOORSCENENODE_HPP

#include <irrlicht.h>

class FloorSceneNode : public irr::scene::ISceneNode {
private:
	irr::core::aabbox3d<irr::f32>    bounding_box;
	irr::video::SMaterial            material;
	irr::video::S3DVertex*           vertices;
	irr::u32*                        indices;
	irr::core::dimension2d<irr::u32> size;
public:
	FloorSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* smgr, int width, int height)
		: ISceneNode(parent, smgr) {
		size = irr::core::dimension2d<irr::u32>(width, height);

		// :TODO: memleak
		vertices = new irr::video::S3DVertex[width * height * 4];
		indices  = new irr::u32             [width * height * 6];

		memset(vertices, 0, width * height * 4);
		memset(indices , 0, width * height * 6);

		bounding_box = irr::core::aabbox3d<irr::f32>(0, 0, 0, width+1, height+1, 0);

		material.Wireframe = false;
		material.Lighting  = true;
		material.BackfaceCulling = false; // :TODO: re-enable
	}

	void updateTiles(float* heights){
		bounding_box.MaxEdge.Z = 0;
		for(int i = 0; i < this->size.getArea(); ++i){
			if(bounding_box.MaxEdge.Z < heights[i]){
				bounding_box.MaxEdge.Z = heights[i];
			}
		}

		irr::video::SColor white(255, 255, 255, 255);

		for(int x = 0; x < this->size.Width; ++x){
			for(int y = 0; y < this->size.Height; ++y){
				int i = y + x*this->size.Height;
				vertices[i*4 + 0] = irr::video::S3DVertex(x+0, heights[i], y+0, 0, 1, 0, white, 0, 0);
				vertices[i*4 + 1] = irr::video::S3DVertex(x+1, heights[i], y+0, 0, 1, 0, white, 1, 0);
				vertices[i*4 + 2] = irr::video::S3DVertex(x+0, heights[i], y+1, 0, 1, 0, white, 0, 1);
				vertices[i*4 + 3] = irr::video::S3DVertex(x+1, heights[i], y+1, 0, 1, 0, white, 1, 1);

				indices [i*6 + 0] = i*4 + 0;
				indices [i*6 + 1] = i*4 + 1;
				indices [i*6 + 2] = i*4 + 2;
				indices [i*6 + 3] = i*4 + 1;
				indices [i*6 + 4] = i*4 + 2;
				indices [i*6 + 5] = i*4 + 3;
			}
		}
		printf("Done update of tiles\n");
	}

	virtual void OnRegisterSceneNode() {
		if (IsVisible){
			SceneManager->registerNodeForRendering(this);
		}

		ISceneNode::OnRegisterSceneNode();
	}

	/*
	In the render() method most of the interesting stuff happens: The
	Scene node renders itself. We override this method and draw the
	tetraeder.
	*/
	virtual void render() {
		irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(material);
		driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(&vertices[0],
		                                size.getArea() * 4, // num verticies
		                                &indices[0],
		                                size.getArea() * 2, // num primitives
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

#endif
