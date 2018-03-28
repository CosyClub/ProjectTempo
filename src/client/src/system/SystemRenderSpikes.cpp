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

	if (entities.size() == 0) {
		printf("\nThere does not seem to be any spikes\n");
	}

	for (auto &entity : entities) {
		auto &spikes = entity.getComponent<tempo::ComponentSpikes>();
		auto &rend  = entity.getComponent<client::ComponentRenderSpikes>();

		auto &positions = spikes.spike_positions;

		for (int i=0; i<positions.size(); i++) {
			irr::scene::IMesh *spike_mesh = smgr->getMesh("resources/meshes/spikes.obj");

			SpikeNode spikeNode;
			spikeNode.spikeNode = smgr->addMeshSceneNode(spike_mesh, 0);

			spikeNode.spikeNode->setPosition(
				irr::core::Vector3df(positions[i].x, 0.0, positions[i].y));

				rend.push_back(spikeNode);
		}
	}
}

void SystemRenderSpikes::updateSpikes(irr::video::IVideoDriver *driver)
{
	auto entities = getEntities();
	for (auto entity : entities) {
		auto &spikes = entity.getComponent<tempo::ComponentSpikes>();
		auto &rend  = entity.getComponent<client::ComponentRenderSpikes>();

	}
}
}  // namespace client
