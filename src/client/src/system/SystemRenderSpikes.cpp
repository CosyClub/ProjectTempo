#include <client/system/SystemRenderSpikes.hpp>

#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <irrlicht.h>  // :TODO: sort out includes

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>

namespace client
{
void SystemRenderSpikes::setup(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver)
{
	auto& entities = getEntities();
	irr::scene::IMesh *spike_mesh = smgr->getMesh("resources/meshes/spikes.obj");

	for (auto &entity : entities) {
		auto &spikes = entity.getComponent<tempo::ComponentSpikes>();
		auto &rend  = entity.getComponent<client::ComponentRenderSpikes>();

		auto &positions = spikes.spike_positions;

		for (uint32_t i=0; i<positions.size(); i++) {

			SpikeNode spikeNode;
			spikeNode.spikeNode = smgr->addMeshSceneNode(spike_mesh, 0);

			spikeNode.spikeNode->setPosition(
				irr::core::vector3df(positions[i].x, 0.0, positions[i].y));

			irr::video::SMaterial &material_spikes =
				spikeNode.spikeNode->getMaterial(0);
			material_spikes.Shininess = 0.5f;
			material_spikes.EmissiveColor.set(255, 100, 100, 100);
				rend.spikes.push_back(spikeNode);
		}
	}
}

void SystemRenderSpikes::updateSpikes(irr::video::IVideoDriver *driver, const glm::ivec2 playerpos)
{
	auto& entities = getEntities();
	for (auto entity : entities) {
		auto &comp = entity.getComponent<tempo::ComponentSpikes>();
		auto &rend  = entity.getComponent<client::ComponentRenderSpikes>();

		for (uint32_t i = 0; i<comp.spike_positions.size(); i++) {
			glm::ivec2 pos = comp.spike_positions[i];

			if (pos.x < playerpos.x - 24 || pos.x > playerpos.x + 7 ||
			    pos.y < playerpos.y - 33 || pos.y > playerpos.y + 33)
			{
				rend.spikes[i].spikeNode->setVisible(false);
				continue;
			}

			if (comp.isTriggered) {
				rend.spikes[i].spikeNode->setVisible(true);
			} else {
				rend.spikes[i].spikeNode->setVisible(false);
			}
		}

	}
}
}  // namespace client
