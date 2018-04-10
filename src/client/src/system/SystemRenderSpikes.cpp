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
	auto entities = getEntities();

	for (auto &entity : entities) {
		auto &spikes = entity.getComponent<tempo::ComponentSpikes>();
		auto &rend  = entity.getComponent<client::ComponentRenderSpikes>();

		auto &positions = spikes.spike_positions;

		for (int i=0; i<positions.size(); i++) {

			irr::scene::IMesh *spike_mesh = smgr->getMesh("resources/meshes/spikes.obj");

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

void SystemRenderSpikes::updateSpikes(irr::video::IVideoDriver *driver)
{
	auto entities = getEntities();
	for (auto entity : entities) {
		auto &comp = entity.getComponent<tempo::ComponentSpikes>();
		auto &rend  = entity.getComponent<client::ComponentRenderSpikes>();

		if (comp.isTriggered) {
			for (int i = 0; i<comp.spike_positions.size(); i++) {
				rend.spikes[i].spikeNode->setPosition(irr::core::vector3df(
					comp.spike_positions[i].x, 0.0, comp.spike_positions[i].y));
			}
		}
		else {
			for (int i = 0; i<comp.spike_positions.size(); i++) {
				rend.spikes[i].spikeNode->setPosition(irr::core::vector3df(
					comp.spike_positions[i].x, -2.0f, comp.spike_positions[i].y));
			}
		}
	}
}
}  // namespace client
