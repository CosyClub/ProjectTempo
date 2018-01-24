////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file LevelRenderer.hpp
/// \author Jamie Terry
/// \date 2018/01/24
/// \brief Contains defintion of the LevelRenderer class, responsible for rendering
/// the tiles making up the level's floor
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_LEVELRENDERER_HPP
#define TEMPO_ENTITY_LEVELRENDERER_HPP

namespace tempo{

	// Forward declarations
	class LevelManager;

	/////////////////////////////////////////////////////////////////////
	/// \brief Class responsible for rendering the level's tiles
	/////////////////////////////////////////////////////////////////////
	class LevelRenderer {
	public:
		/////////////////////////////////////////////////////////////////////
		/// \brief Creates a new LevelRenderer
		/// \param root_floor_node The node that all floor tiles will be attached to
		/// \param level Pointer to the level manager, if not null updateFloorNodes
		/// will be called using the level
		/////////////////////////////////////////////////////////////////////
		LevelRenderer(Ogre::SceneNode* root_floor_node, LevelManager* level = nullptr);

		/////////////////////////////////////////////////////////////////////
		/// \brief Synchronises the floor node's state with that represented by
		/// the LevelManager. This includes:
		/// - creating nodes if level has entry, but renderer doesn't
		/// - deleting nodes if levels has no entry, but renderer does
		/// - changing heights of nodes if height is changed in level
		/////////////////////////////////////////////////////////////////////
		void updateFloorNodes(LevelManager& level);

		/////////////////////////////////////////////////////////////////////
		/// \brief Sets the material of some tile
		/////////////////////////////////////////////////////////////////////
		inline void setMaterial(Vec2s position, std::string material_name){
			setMaterial(position.x, position.y, material_name);
		}
		void setMaterial(int x, int y, std::string material_name);

	private:
		struct Tile {
			Tile() : node(nullptr), floorpiece(nullptr) { }
			Ogre::SceneNode* node;
			Ogre::Entity*    floorpiece;
		};

		/// \brief The root node that the floor tiles are attached to
		Ogre::SceneNode* root_floor_node;

		/// \brief 2d array of scene nodes for each tile
		std::vector<std::vector<Tile>> tile_nodes;
	};
}

#endif
