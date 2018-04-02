#include <client/component/ComponentKeyInput.hpp>
#include <client/component/ComponentRenderButtonGroup.hpp>
#include <client/component/ComponentRenderSceneNode.hpp>
#include <client/network/client.hpp>
#include <client/system/SystemAttack.hpp>
#include <client/system/SystemButtonRenderer.hpp>
#include <client/system/SystemCombo.hpp>
#include <client/system/SystemEntity.hpp>
#include <client/system/SystemGraphicsCreation.hpp>
#include <client/system/SystemMovement.hpp>
#include <client/system/SystemParseKeyInput.hpp>
#include <client/system/SystemRenderGUI.hpp>
#include <client/system/SystemRenderHealthBars.hpp>
#include <client/system/SystemRenderSceneNode.hpp>
#include <client/system/SystemStageRenderer.hpp>
#include <client/system/SystemUpdateKeyInput.hpp>
#include <client/system/SystemTranslationAnimation.hpp>

#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/song.hpp>
#include <tempo/system/SystemHealth.hpp>
#include <tempo/system/SystemTrigger.hpp>
#include <tempo/time.hpp>

#include <anax/Entity.hpp>
#include <anax/World.hpp>

#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include <irrlicht.h>
#include <vector3d.h>
#include <IVolumeLightSceneNode.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <chrono>
#include <cstdio>
#include <functional>  // maybe not needed (std::ref when calling listenForServerUpdates())
#include <iostream>
#include <thread>

#define BPM 138              // Beats per minutes
#define DELTA 100            // Delta around a beat a player can hit (millisecs)
#define TIME 60000000 / BPM  // Time between beats (microsecs)

#include <tempo/component/ComponentStage.hpp>
namespace client
{
	// TLDR: enforce the game logic on client side and server side
	class SystemLessJank
		: public anax::System<
		anax::Requires<
		tempo::ComponentStageTranslation,
		tempo::ComponentStagePosition,
		tempo::ComponentStage>
		>
	{
	public:
		void lessJank() {
			// uncomment this for more jank:
			//return;

			auto& entities = getEntities();

			for (auto& entity : entities) {
				tempo::ComponentStageTranslation& trans = entity.getComponent<tempo::ComponentStageTranslation>();
				tempo::ComponentStage& stage = entity.getComponent<tempo::ComponentStage>();
				glm::ivec2 origin = entity.getComponent<tempo::ComponentStagePosition>().getOrigin();



				glm::ivec2 dest = origin + trans.delta;

				if (!stage.existstTile(dest) || stage.getHeight(dest) >= 5) {
					// consume the moment before the server rejects you
					// currently combos aren't server protected, so maybe this should move into lib-tempo?
					// this produces a lovely jumping against the wall animation!
					trans.delta = glm::ivec2(0, 0);
					//if (entity.hasComponent<tempo::ComponentCombo>()) {
					//	// what the heck this is a jank class anyway
					//	tempo::ComponentCombo& combo = entity.getComponent<tempo::ComponentCombo>();
					//	combo.advanceBeat();
					//}
				}
			}
		}
	};

}  // namespace client

void sync_time(tempo::Clock &clock)
{
	sf::Int64 offset = tempo::timeSyncClient(&clock);
	clock.set_time(clock.get_time() + sf::microseconds(offset));
}

anax::Entity createEntityStage(anax::World &world)
{
	printf("Creating entity stage\n");
	anax::Entity entity_stage = world.createEntity();
	entity_stage.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
	entity_stage.activate();

	return entity_stage;
}

// anax::Entity createEntityPlayer(anax::World& world) {
// 	printf("Creating entity player\n");
// 	anax::Entity entity_player = world.createEntity();
// 	entity_player.addComponent<tempo::ComponentStage>("resources/levels/levelTest.bmp");
// 	entity_player.addComponent<tempo::ComponentStagePosition>(glm::ivec2(5, 5));
// 	entity_player.addComponent<client::ComponentRenderSceneNode>(nullptr);
// 	entity_player.addComponent<client::ComponentKeyInput>();
// 	entity_player.activate();

// 	return entity_player;
// }

int main(int argc, const char **argv)
{
	sf::SoundBuffer clickbuf;
	clickbuf.loadFromFile("resources/sound/tick.ogg");
	sf::Sound click;
	click.setBuffer(clickbuf);

	// Clock
	tempo::Clock clock = tempo::Clock(sf::microseconds(TIME), sf::milliseconds(DELTA));

	KeyInput             receiver;

	// This makes it full-screen
	// irr::IrrlichtDevice *nulldevice = irr::createDevice(irr::video::EDT_NULL);
	// irr::core::dimension2d<irr::u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();
	// nulldevice -> drop();
	// irr::IrrlichtDevice *device = irr::createDevice(
	//   irr::video::EDT_OPENGL, deskres, 16, true, false, false);


	irr::IrrlichtDevice *device = irr::createDevice(
	  irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1280, 720), 16, false, false, false);
	if (!device) {
		printf("Failed to create Irrlicht Device\n");
		return 1;
	}
	device->setWindowCaption(L"RaveCave");
	irr::video::IVideoDriver * driver  = device->getVideoDriver();
	irr::scene::ISceneManager *smgr    = device->getSceneManager();
	irr::gui::IGUIEnvironment *gui_env = device->getGUIEnvironment();
	// Debug
	smgr->setAmbientLight(irr::video::SColorf(0.3f, 0.3f, 0.3f));

	/////////////////////////////////////////////////
	// Setup ECS
	anax::World world;
	// tempo::SystemRender           system_render(app);
	tempo::SystemHealth            system_health;
	tempo::SystemTrigger           system_trigger(world);
	client::SystemAttack           system_attack;
	client::SystemButtonRenderer   system_button_renderer;
	client::SystemCombo             system_combo;
	client::SystemEntity           system_entity;
	client::SystemGraphicsCreation system_gc;
	client::SystemMovement         system_movement;
	client::SystemStageRenderer    system_stage_renderer;
	client::SystemParseKeyInput    system_parse_key_input;
	client::SystemRenderGUI        system_render_gui;
	client::SystemRenderHealthBars system_render_health_bars;
	client::SystemRenderSceneNode  system_render_scene_node;
	client::SystemUpdateKeyInput   system_update_key_input;
	client::SystemTranslationAnimation system_translation_animation(&world, device, clock);
	client::SystemLessJank system_less_jank;

	// Add Systems
	world.addSystem(system_attack);
	world.addSystem(system_entity);
	world.addSystem(system_combo);
	world.addSystem(system_health);
	world.addSystem(system_gc);
	world.addSystem(system_trigger);
	world.addSystem(system_button_renderer);
	world.addSystem(system_stage_renderer);
	world.addSystem(system_render_scene_node);
	world.addSystem(system_render_health_bars);
	world.addSystem(system_update_key_input);
	world.addSystem(system_parse_key_input);
	world.addSystem(system_movement);
	world.addSystem(system_translation_animation);
	world.addSystem(system_less_jank);

	anax::Entity entity_stage = createEntityStage(world);
	world.refresh();

	// Initialise Systems
	system_update_key_input.setup(device);
	system_stage_renderer.setup(smgr, driver,{255, 175, 0, 0}, {255, 50, 50, 50});
	system_render_scene_node.setup(smgr, driver);

	// must be after system_render_scene_node.setup(smgr);
	system_render_health_bars.setup(smgr);

	// Set up remote address, local ports and remote handshake port
	// Note, IF statement is to change ports for local development, bit
	// hacky and should be removed in due course!
	tempo::addr_r = "127.0.0.1";
	if (argc == 2)
		tempo::addr_r = argv[1];
	if (tempo::addr_r == "127.0.0.1") {
		std::srand(time(NULL));
		int d          = std::rand() % 10;
		tempo::port_ci = DEFAULT_PORT_IN + 10 + d;
		tempo::port_co = DEFAULT_PORT_OUT + 10 + d;
	} else {
		tempo::port_ci = DEFAULT_PORT_IN;
		tempo::port_co = DEFAULT_PORT_OUT;
	}
	// Other server ports aquired dynamically on handshake
	tempo::port_si = DEFAULT_PORT_IN;

	// Bind sockets
	tempo::bindSocket('i', tempo::port_ci);
	tempo::bindSocket('o', tempo::port_co);

	// Start Listener Thread to catch server updates after connecting
	std::atomic<bool> running(true);
	std::thread       listener(tempo::listenForServerUpdates, std::ref(running));
	// Hack to allow printouts to line up a bit nicer :)
	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	tempo::ClientRole     role     = tempo::ClientRole::PLAYER;
	tempo::ClientRoleData roleData = {"Bilbo Baggins"};

	// Connect to server and handshake information
	tempo::connectToAndSyncWithServer(role, roleData, world);

	// Sort out graphics after handshake
	system_gc.addEntities(driver, smgr, world);
	system_render_scene_node.setup(smgr, driver);
	system_render_health_bars.setup(smgr);
	system_button_renderer.setup(smgr, driver);

	// Start and Sync Song
	sync_time(clock);
	// long offset = 0;

	// Player
	// TODO: use better way to find out player, for now this is a search
	anax::Entity entity_player;
	for (auto &entity : world.getEntities()) {
		if (entity.hasComponent<tempo::ComponentPlayerLocal>()) {
			entity_player = entity;
			break;
		}
	}
	entity_player.addComponent<client::ComponentKeyInput>();
	entity_player.activate();
	client::ComponentRenderSceneNode &sn =
	  entity_player.getComponent<client::ComponentRenderSceneNode>();

	auto &combo = entity_player.getComponent<tempo::ComponentCombo>().comboCounter;
	auto &comp_health = entity_player.getComponent<tempo::ComponentHealth>();

	irr::scene::ICameraSceneNode *camera_node;
	if (false) {
		float rotateSpeed = 25.0f;
		float moveSpeed   = 0.1f;
		camera_node       = smgr->addCameraSceneNodeFPS(nullptr, rotateSpeed, moveSpeed);
		device->getCursorControl()->setVisible(false);
	} else {
		float rotate    = 0.0f;
		float translate = 0.0f;  //-100
		float zoom      = 0.0f;  // 100
		float distance  = 0.0f;
		// camera_node = smgr->addCameraSceneNodeMaya(sn.node, rotate, translate, zoom, -1,
		// distance); camera_node->setPosition(irr::core::vector3df(0.0f, 0.0f, 0.0f));
		camera_node = smgr->addCameraSceneNode();
		camera_node->setPosition(irr::core::vector3df(14, 9, 0));
		camera_node->setTarget(sn.node->getPosition());
		// camera_node->setRotation(irr::core::vector3df(0,0,90));
		device->getCursorControl()->setVisible(true);
	}

	// irr::scene::ISceneNode* camera_light;
	// camera_light = smgr->addLightSceneNode(camera_node,
	//                                        irr::core::vector3df(0.0f, 0.0f, 0.0f),
	//                                        irr::video::SColorf(0.8f, 0.8f, 0.8f),
	//                                        10.0f);
	// debug static light
	irr::scene::ILightSceneNode *light_node;
	light_node = smgr->addLightSceneNode(0, irr::core::vector3df(10.0f, 10.0f, 10.0f),
	                                     irr::video::SColorf(0.8f, 0.8f, 0.8f), 5.0f);
	// irr::video::SLight& light_data = light_node->getLightData();

	/// This is the main light responsible for all the stage colors:
	irr::scene::ILightSceneNode* light_ML1;
	irr::video::SLight data_ML1;

	/// Create LASER LIGHTS:
	// Get images:
	irr::core::array<irr::video::ITexture*> texturesLaser;
	for (irr::s32 n = 0; n<13; n++) {
		irr::core::stringc tmp; tmp = "resources/materials/lasers/laser_light_"; tmp += n; tmp += ".jpg";
		irr::video::ITexture* tx = driver->getTexture(tmp.c_str());
		texturesLaser.push_back(tx);
	};

	irr::scene::IVolumeLightSceneNode* nodeLaserA[10] = {};

	for (int i = 0; i<8; i++) {
		nodeLaserA[i] = smgr->addVolumeLightSceneNode(0, -1, 4, 4, irr::video::SColor(0, 255, 255, 255), irr::video::SColor(0, 0, 0, 0)); // Set: | ? | ? | Subdivisions on U axis | Subdivisions on V axis | foot color | tail color
																																// nodeLaserA[i]->enableCastShadow(false);
		nodeLaserA[i]->setMaterialFlag(irr::video::EMF_LIGHTING, false); // Node is affected by LIGHT?
		nodeLaserA[i]->setScale(irr::core::vector3df(0.1f, 900.0f, 0.1f));
		nodeLaserA[i]->setPosition(irr::core::vector3df(40, 10, 12)); /// Position Y = 384.1f is perfect!
																		// Create texture animator
		irr::scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(texturesLaser, 250, true); // Time between images (textures)
																								 // Add the animator: G/// Create LASER LIGHTS at the left:low
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

	/// Create (Loads Object) DISCO BALL N1:
	irr::scene::IAnimatedMeshSceneNode* nodeDiscoBall1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("resources/meshes/disco_ball.obj"));
	nodeDiscoBall1->setPosition(irr::core::vector3df(40, 10, 12));
	// nodeDiscoBall1->setRotation(vector3df(0, 180, 0));
	nodeDiscoBall1->setScale(irr::core::vector3df(0.1, 0.1, 0.1));
	nodeDiscoBall1->setMaterialFlag(irr::video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
	nodeDiscoBall1->setMaterialFlag(irr::video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
	nodeDiscoBall1->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false); // Render both sides?
	nodeDiscoBall1->setMaterialFlag(irr::video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
	nodeDiscoBall1->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	nodeDiscoBall1->setMaterialType(irr::video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	for (irr::u32 i = 0; i<nodeDiscoBall1->getMaterialCount(); i++) {
		nodeDiscoBall1->getMaterial(i).setTexture(0, driver->getTexture("resources/materials/disco_ball.jpg")); // Apply texture to my specified material
		nodeDiscoBall1->getMaterial(i).getTextureMatrix(0).setTextureScale(3.0f, 1.0f);   /// Repeat (tile) the texture
		nodeDiscoBall1->getMaterial(i).setTexture(1, driver->getTexture("resources/materials/reflexes.jpg"));
		nodeDiscoBall1->getMaterial(i).getTextureMatrix(1).setTextureScale(0.015f, 0.015f); /// Repeat (tile) the texture

																							/// Start Colors:
																							/// Set size of specular highlights
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
			nodeDiscoBall1->getMaterial(i).TextureLayer[t].LODBias = 128;
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
		irr::core::dimension2df(5.0f, 5.0f),  // min size
		irr::core::dimension2df(25.0f, 25.0f)); // max size

	ps60->setEmitter(em60); // Grabs the emitter
	em60->drop(); // We can drop it here, without deleting it

	ps60->setParent(nodeDiscoBall1); /// Attach these reflexed lights to the Disco Ball N1 (Match to the same position & rotation)
									 // ps60->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
									 // ps60->setScale(core::vector3df(100,100,100));
	ps60->setScale(irr::core::vector3df(.6, .6, .6));
	ps60->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
	ps60->setMaterialTexture(0, driver->getTexture("resources/materials/estrela_squared.png"));
	// ps60->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps60->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps60->setMaterialFlag(irr::video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps60->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps60);
	// ps60->getMaterial(0).Lighting = false;
	// ps60->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps60->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps60->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps60->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
	em60->setMinParticlesPerSecond(0);
	em60->setMaxParticlesPerSecond(0);

	/// Create Main Colored LIGHT, does cast shadows:
	irr::scene::ISceneNode* node_ML1 = 0;
	irr::scene::IBillboardSceneNode* billboard_ML1 = smgr->addBillboardSceneNode(node_ML1);
	billboard_ML1->setPosition(irr::core::vector3df(0, 190, -420));
	billboard_ML1->setMaterialTexture(0, driver->getTexture("dummy.png")); /// Invisible: "dummy.png" - Visible: "estrela.png"
	billboard_ML1->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	billboard_ML1->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	billboard_ML1->setScale(irr::core::vector3df(450, 450, 450));

	// Add animator:
	irr::scene::ISceneNodeAnimator* anim_ML1 = smgr->createFlyCircleAnimator(irr::core::vector3df(0, 190, -420), 3.0f, 0.0030f);
	billboard_ML1->addAnimator(anim_ML1);
	anim_ML1->drop();

	/// Create the light: (This is the main light responsible for all the stage colors)
	light_ML1 = smgr->addLightSceneNode(billboard_ML1, irr::core::vector3df(0, 0, 0), irr::video::SColorf(1.0, 1.0, 1.0, 0.0), 3000); // SColorf: 0.0f to 1.0f full color. SColorf = r,g,b,a
	light_ML1->enableCastShadow(true);
	light_ML1->setVisible(true);
	data_ML1.Type = irr::video::ELT_POINT; // ELT_POINT | ELT_SPOT | ELT_DIRECTIONAL
									  // data_ML1.Direction = vector3df(0,-1,0);
	data_ML1.AmbientColor = irr::video::SColorf(0, 0, 0, 0); // Will change in main loop
	data_ML1.SpecularColor = irr::video::SColorf(0, 0, 0, 0); // Will change in main loop
	data_ML1.DiffuseColor = irr::video::SColorf(0, 0, 0, 0); // Will change in main loop
	light_ML1->setLightData(data_ML1); // Apply data to the light (Will change in main loop)

	/////////////////////////////////////////////////
	// Main loop
	int       frame_counter = 0;
	sf::Clock fps_timer;
	// sf::Clock dt_timer;

	int j = 0;

	sf::Int64 tick               = clock.get_time().asMicroseconds() / sf::Int64(TIME);
	sf::Clock frame_clock        = sf::Clock();
	sf::Clock update_floor_clock = sf::Clock();
	frame_clock.restart();
	update_floor_clock.restart();

	int lastFPS = -1; // Will be set in the main loop...
	irr::u32 then = 0.0; // Will be set in the main loop...
	irr::f32 frameDeltaTime = 0.0f; // Will be set in the main loop...


	lastFPS = -1;
	then = device->getTimer()->getTime();
	frameDeltaTime = 0.0;

	printf("Entering main loop\n");
	while (device->run()) {
		// sf::Int64 tick1 = update_floor_clock.getElapsedTime().asMilliseconds();
		// float dt = dt_timer.getElapsedTime().asSeconds();
		// dt_timer.restart();

		// Work out a frame delta time.
		const irr::u32 now = device->getTimer()->getTime();
		/// frameDeltaTime = (f32)(now - then)/1000.f; // Time in seconds
		frameDeltaTime = (irr::f32)(now - then);
		then = now;

		////////////////
		// Events all the time
		{
			static int cqt_90 = 0;
			static int cqt_100 = 0;
			cqt_90 += 0.150*frameDeltaTime; if (cqt_90 >= 90) { cqt_90 = 90; }; /// <-- Force a smooth gradually start
			cqt_100 += 0.150*frameDeltaTime; if (cqt_100 >= 100) { cqt_100 = 100; }; /// <-- Force a smooth gradually start
			em60->setMinParticlesPerSecond(cqt_90); em60->setMaxParticlesPerSecond(cqt_100); /// disco ball n1 - lights reflexes

			// Check for new entities from server
			system_entity.creationCheck(world);
			system_entity.deletionCheck(world);

			// Initialise Graphics for new entities
			system_gc.addEntities(driver, smgr, world);
			system_render_scene_node.setup(smgr,driver);
			system_render_health_bars.setup(smgr);
			system_button_renderer.setup(smgr, driver);

			// Recieve updates from the server
			system_movement.processIntents(world);
			system_movement.processCorrections(world);
			system_combo.checkForUpdates(world);

			// Deal with local input
			system_update_key_input.clear();
			system_update_key_input.addKeys();
			system_parse_key_input.parseInput(clock, device);

			// Deprecated/To-be-worked-on
			system_health.CheckHealth();
			system_health.recieveHealth(world);

			system_less_jank.lessJank();
			// Update animations from translations received from server
			system_translation_animation.updateAnimations();

			// Graphics updates
			// std::cout << "START OF CRASH LINE 312 CLIENT MAIN.CPP" << std::endl;
			system_render_scene_node.update();
			// std::cout << "IF YOU SEE THIS AFTER A SECOND CLIENT CONNECTS YOU FIXED IT" <<
			// std::endl;
			system_render_health_bars.update();

			// TODO: Make a system for updating camera position
			irr::core::vector3df camera_target = sn.node->getAbsolutePosition();
			camera_node->setPosition(camera_target + irr::core::vector3df(14, 9, 0));
			camera_node->updateAbsolutePosition();
			camera_node->setTarget(camera_target);
		}

		////////////////
		// Events at "Delta Start"
		if (clock.passed_delta_start()) {
			// std::cout << "Start" << std::endl;
		}

		////////////////
		// Events at "Beat Passed"
		if (clock.passed_beat()) {
			// click.play();
			if (tick++ % 20 == 0)
				std::cout << "TICK (" << tick << ") " << clock.get_time().asMilliseconds()
				          << "+++++++++++++++" << std::endl;

			j++;
			j = j % 22;
			system_trigger.updateButtons(world);
			system_button_renderer.updateButtons(driver);
			system_translation_animation.endBeat();
			// sf::Int64 tick2 = update_floor_clock.getElapsedTime().asMilliseconds();
			// std::cout << "Time to update floor: " << (int)(tick2-tick1)<<"ms"
			// << std::endl;

			/// Animated light: ( Change to a random color... )
			irr::f32 red = ((1.0 - 0.0)*((float)rand() / RAND_MAX)) + 0.0; // from (0.0 to 1.0)
			irr::f32 green = ((1.0 - 0.0)*((float)rand() / RAND_MAX)) + 0.0; // from (0.0 to 1.0)
			irr::f32 blue = ((1.0 - 0.0)*((float)rand() / RAND_MAX)) + 0.0; // from (0.0 to 1.0)
			data_ML1.AmbientColor = irr::video::SColorf(red, green, blue, 0.0);
			data_ML1.SpecularColor = irr::video::SColorf(red, green, blue, 0.0);
			data_ML1.DiffuseColor = irr::video::SColorf(red, green, blue, 0.0);
			light_ML1->setLightData(data_ML1); // Apply data to the light

		}
		glm::ivec2 playerpos =
		  entity_player.getComponent<tempo::ComponentStagePosition>().getOrigin();

		system_stage_renderer.updateStage(smgr, driver, j, playerpos);

		////////////////
		// Events at "Delta End"
		if (clock.passed_delta_end()) {
			// std::cout << "End" << std::endl;
			system_combo.advanceBeat();
		}

		// Rendering Code
		// if (!device->isWindowActive()) {
		//	device->yield();
		//	continue;
		//}

		// sf::Int64 tick2 = update_floor_clock.getElapsedTime().asMilliseconds();
		// std::cout << "Time to update floor: " << (int)(tick2-tick1)<<"ms"
		// << std::endl;

		driver->beginScene(true, true);
		smgr->drawAll();
		gui_env->drawAll();

		system_render_gui.update(driver, gui_env, clock, combo, comp_health);
		driver->endScene();

		++frame_counter;
		if (fps_timer.getElapsedTime().asSeconds() > 1.0f) {
			float seconds = fps_timer.getElapsedTime().asSeconds();
			std::cout << "FPS: " << (int) (frame_counter / seconds) << std::endl;
			fps_timer.restart();
			frame_counter = 0;
		}

	}  // main loop
	running.store(false);
	printf("Left main loop\n");

	// Tell server we are gone
	tempo::disconnectFromServer(entity_player);

	// Close server listener and destroy the game
	listener.join();
	world.clear();
	device->drop();

	return 0;
}
