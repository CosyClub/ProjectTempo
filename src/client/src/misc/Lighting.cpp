#include <client/misc/Lighting.hpp>

namespace client {


	void createLasers(irr::scene::ISceneManager *smgr,
					  irr::video::IVideoDriver * driver,
					  std::vector<glm::ivec2> positions,
					  glm::ivec2 playerPos) {

		irr::core::array<irr::video::ITexture*> texturesLaser;

		/// Create LASER LIGHTS:
		// Get images:
		for (irr::s32 n = 0; n < 13; n++) {
			irr::core::stringc tmp; tmp = "resources/materials/lasers/laser_light_"; tmp += n; tmp += ".jpg";
			irr::video::ITexture* tx = driver->getTexture(tmp.c_str());
			texturesLaser.push_back(tx);
		};

		for (glm::ivec2 pos : positions) {

			if (pos.x < playerPos.x - 20 || pos.x > playerPos.x + 20) {
				continue;
			}

			irr::scene::IVolumeLightSceneNode* nodeLaserA[10];

			for (int i = 0; i < 8; i++) {
				nodeLaserA[i] = smgr->addVolumeLightSceneNode(0, -1, 4, 4, irr::video::SColor(0, 255, 255, 255), irr::video::SColor(0, 0, 0, 0)); // Set: | ? | ? | Subdivisions on U axis | Subdivisions on V axis | foot color | tail color
				nodeLaserA[i]->setMaterialFlag(irr::video::EMF_LIGHTING, false); // Node is affected by LIGHT?
				nodeLaserA[i]->setScale(irr::core::vector3df(0.05f, 25.0f, 0.05f));
				nodeLaserA[i]->setPosition(irr::core::vector3df(pos.x, 12, pos.y));
				// Create texture animator
				irr::scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(texturesLaser, 250, true); // Time between images (textures)

				nodeLaserA[i]->addAnimator(glow);
				// Drop the animator because it was created with a create() function
				glow->drop();
			};

			if (nodeLaserA[0]) {
				irr::scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(irr::core::vector3df(0.1f, 0, 1.0f));
				nodeLaserA[0]->addAnimator(animLaser);
				animLaser->drop();
			};
			// Add the animator: Rotation:
			if (nodeLaserA[1]) {
				irr::scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(irr::core::vector3df(0.1f, 0, -1.0f));
				nodeLaserA[1]->addAnimator(animLaser);
				animLaser->drop();
			};
			// Add the animator: Rotation:
			if (nodeLaserA[2]) {
				irr::scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(irr::core::vector3df(-0.1f, 0, 0.2f));
				nodeLaserA[2]->addAnimator(animLaser);
				animLaser->drop();
			};
			// Add the animator: Rotation:
			if (nodeLaserA[3]) {
				irr::scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(irr::core::vector3df(-0.1f, 0, -0.2f));
				nodeLaserA[3]->addAnimator(animLaser);
				animLaser->drop();
			};
			// Add the animator: Rotation:
			if (nodeLaserA[4]) {
				irr::scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(irr::core::vector3df(0.1f, 0, 0.7f));
				nodeLaserA[4]->addAnimator(animLaser);
				animLaser->drop();
			};
			// Add the animator: Rotation:
			if (nodeLaserA[5]) {
				irr::scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(irr::core::vector3df(0.1f, 0, -0.7f));
				nodeLaserA[5]->addAnimator(animLaser);
				animLaser->drop();
			};
			// Add the animator: Rotation:
			if (nodeLaserA[6]) {
				irr::scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(irr::core::vector3df(-0.1f, 0, 1.7f));
				nodeLaserA[6]->addAnimator(animLaser);
				animLaser->drop();
			};
			// Add the animator: Rotation:
			if (nodeLaserA[7]) {
				irr::scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(irr::core::vector3df(-0.1f, 0, -1.7f));
				nodeLaserA[7]->addAnimator(animLaser);
				animLaser->drop();
			};
		}
	}


	void createDiscoBalls(irr::scene::ISceneManager *smgr,
						  irr::video::IVideoDriver * driver,
						  std::vector<glm::ivec2> positions,
						  glm::ivec2 playerPos,
						  irr::scene::ISceneNode *parent) {

		irr::scene::IAnimatedMeshSceneNode* nodeDiscoBall1;

		bool created = false;

		for (glm::ivec2 pos : positions) {

			if (pos.x < playerPos.x - 20 || pos.x > playerPos.x + 20) {
				continue;
			}

			if (!created) {
				nodeDiscoBall1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("resources/meshes/disco_ball.obj"), parent);
				created = true;
			}

			nodeDiscoBall1->setPosition(irr::core::vector3df(pos.x, 10, pos.y));
			// nodeDiscoBall1->setRotation(vector3df(0, 180, 0));
			nodeDiscoBall1->setScale(irr::core::vector3df(0.1, 0.1, 0.1));
			nodeDiscoBall1->setMaterialFlag(irr::video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
			nodeDiscoBall1->setMaterialFlag(irr::video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
			nodeDiscoBall1->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, true); // Render both sides?
			nodeDiscoBall1->setMaterialFlag(irr::video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
			nodeDiscoBall1->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
			nodeDiscoBall1->setMaterialType(irr::video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

			for (irr::u32 i = 0; i<nodeDiscoBall1->getMaterialCount(); i++) {
				nodeDiscoBall1->getMaterial(i).setTexture(0, driver->getTexture("resources/materials/disco_ball.jpg")); // Apply texture to my specified material
				nodeDiscoBall1->getMaterial(i).getTextureMatrix(0).setTextureScale(3.0f, 1.0f);   /// Repeat (tile) the texture
				nodeDiscoBall1->getMaterial(i).setTexture(1, driver->getTexture("resources/materials/reflexes.jpg"));
				nodeDiscoBall1->getMaterial(i).getTextureMatrix(1).setTextureScale(0.015f, 0.015f); /// Repeat (tile) the texture

				nodeDiscoBall1->getMaterial(i).Lighting = true;
				nodeDiscoBall1->getMaterial(i).Shininess = 100.0f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
																   /// Affect the colour of the reflected light:
				nodeDiscoBall1->getMaterial(i).SpecularColor = irr::video::SColor(255, 250, 250, 250); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
																									   /// Tweak all the light related aspects of the material:
				nodeDiscoBall1->getMaterial(i).AmbientColor = irr::video::SColor(200, 200, 200, 200); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
				nodeDiscoBall1->getMaterial(i).DiffuseColor = irr::video::SColor(200, 200, 200, 200); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
				nodeDiscoBall1->getMaterial(i).EmissiveColor = irr::video::SColor(0, 0, 0, 0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
																							   /// End Colors

				for (irr::u32 t = 0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++) {
					nodeDiscoBall1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
					nodeDiscoBall1->getMaterial(i).TextureLayer[t].LODBias = 0;
				};
				nodeDiscoBall1->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
			};

			irr::scene::ISceneNodeAnimator* anim11 = smgr->createRotationAnimator(irr::core::vector3df(0, 1.0f, 0));
			nodeDiscoBall1->addAnimator(anim11);
			anim11->drop();

			/// Create particle system: (DISCO BALL N1 - reflexed lights)
			irr::scene::IParticleSystemSceneNode* ps60 = smgr->addParticleSystemSceneNode(false);

			irr::scene::IParticleEmitter* em60 = ps60->createSphereEmitter( /// SPHERE Emitter (Same geometry as the mesh is)
				irr::core::vector3df(0.0f, 0.0f, 0.0f),  // emitter position
				0.282f,                           // f32 radius
				irr::core::vector3df(0.0f, 0.0f, 0.0f),  // initial direction & speed (all set to 0.0f == particles stopped)
				0, 0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
												   // video::SColor(0,0,0,255),      // darkest color
				irr::video::SColor(0, 0, 0, 0),           // darkest color
				irr::video::SColor(255, 255, 255, 255),   // brightest color
				90, 200, 0,                         // min age, max age, angle
				irr::core::dimension2df(1.0f, 1.0f),  // min size
				irr::core::dimension2df(1.5f, 1.5f)); // max size

			ps60->setEmitter(em60); // Grabs the emitter
			em60->drop(); // We can drop it here, without deleting it

			ps60->setParent(nodeDiscoBall1); /// Attach these reflexed lights to the Disco Ball N1 (Match to the same position & rotation)
			ps60->setScale(irr::core::vector3df(41, 41, 41));
			ps60->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
			ps60->setMaterialTexture(0, driver->getTexture("resources/materials/estrela_squared.png"));
			ps60->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
			ps60->setMaterialFlag(irr::video::EMF_LIGHTING, true); // Node is affected by LIGHT?
			ps60->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

			/// Disabled:
			em60->setMinParticlesPerSecond(90);
			em60->setMaxParticlesPerSecond(100);
		}

	}
}
