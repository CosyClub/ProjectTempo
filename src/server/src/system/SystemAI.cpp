#include <server/system/SystemAI.hpp>

#include <iostream>

namespace server
{
	
using tempo::operator<<;

bool ai_attack(anax::Entity entity, server::SystemAttack s_attack)
{
	if (entity.hasComponent<tempo::ComponentAttack>() && entity.hasComponent<tempo::ComponentWeapon>())
	{
		tempo::ComponentAttack &a = entity.getComponent<tempo::ComponentAttack>();
		tempo::ComponentWeapon &w = entity.getComponent<tempo::ComponentWeapon>();
		tempo::ComponentStageRotation &r = entity.getComponent<tempo::ComponentStageRotation>();

		//if we're already attacking then just keep going
		if (a.beats_until_attack > -1) return true;

		glm::ivec2 direction;
		if (s_attack.bestAttack(entity, direction))
		{
			if (r.facing != direction)
			{
				//face the player, no cheating here
				r.facing = direction;
				return true;
			}

			a.damage = w.damage;
			a.beats_until_attack = w.beats_until_attack;

			sf::Packet p;
			p << static_cast<uint32_t>(tempo::MessageAttack::UPDATE_INTENT);
			p << entity.getId();
			p << a.damage;
			p << a.beats_until_attack;
			
			tempo::Queue<sf::Packet> *q = get_system_queue(tempo::QueueID::SYSTEM_ATTACK);
			q->push(p);
			return true;
		}
	}
	return false;
}

glm::ivec2 move_to(glm::ivec2 current, glm::ivec2 target)
{
	glm::ivec2 move(0,0);
	glm::ivec2 delta = target - current;
	if (delta.x) move.x = delta.x / abs(delta.x);
	else if (delta.y) move.y = delta.y / abs(delta.y);

	return move;
}

glm::ivec2 random_move()
{
	int dir    = rand() % 2;            // between 0 and 1
	int amount = (rand() % 2) * 2 - 1;  //-1 or 1

	if (dir) {
		return glm::ivec2(amount, 0);
	} else {
		return glm::ivec2(0, amount);
	}
}

void generate_moves(tempo::ComponentAI &ai)
{
	std::deque<glm::ivec2> &path = ai.path;
	int moves_amount = 5;
	int attempts = 0;
	for (int I = path.size(); I < moves_amount && attempts < 40; I++)
	{
		glm::ivec2 finalpos = path.back();
		glm::ivec2 move = finalpos + random_move();
		if (std::find(path.begin(), path.end(), move) == path.end())
		{
			path.push_back(move);
		}
		attempts++;
	}
}

void SystemAI::update(server::SystemAttack s_attack)
{
	auto entities = getEntities();

	for (auto &entity : entities) {

		auto &st = entity.getComponent<tempo::ComponentStageTranslation>();
		auto &sp = entity.getComponent<tempo::ComponentStagePosition>();
		auto &sr = entity.getComponent<tempo::ComponentStageRotation>();
		auto &ai = entity.getComponent<tempo::ComponentAI>();

		if (ai_attack(entity, s_attack)) continue;

		switch(ai.type)
		{
			case tempo::MoveType::MOVE_NONE:
			{
				break;
			}
			case tempo::MoveType::MOVE_STRAIGHT:
			{
				if (ai.prevPos == sp.getOrigin())
				{
					sr.facing *= -1.f;
				}
				st.delta = sr.facing;
				break;
			}
			case tempo::MoveType::MOVE_PATH:
			{
				if (ai.prevPos == sp.getOrigin())
				{
					//We're stuck
					ai.path.clear();
					ai.path.push_back(sp.getOrigin());
				}
				//make sure we know where we want to go
				generate_moves(ai);
				glm::ivec2 target = ai.path.front();
				if (target == sp.getOrigin())
				{
					//we've got to where we were pathing to
					ai.path.pop_front();
					target = ai.path.front();
				}
				st.delta = move_to(sp.getOrigin(), target);
				generate_moves(ai);
				break;
			}
			case tempo::MoveType::MOVE_PATROL:
			{
				glm::ivec2 target = ai.path.front();
				if (target == sp.getOrigin())
				{
					ai.path.pop_front();
					ai.path.push_back(target);
					target = ai.path.front();
				}
				st.delta = move_to(sp.getOrigin(), target);
				break;
			}
			case tempo::MoveType::MOVE_WANDER:
			{
				st.delta = random_move();
				break;
			}
		}

		if (st.delta.x || st.delta.y) sr.facing = st.delta;
		st.moved = true;

		sf::Packet update_broadcast;
		update_broadcast << entity.getId() << sr.facing.x << sr.facing.y
		                 << st.delta.x << st.delta.y << true;

		tempo::broadcastMessage(tempo::QueueID::MOVEMENT_INTENT_UPDATES,
		                         update_broadcast);

		ai.prevPos = sp.getOrigin();

	}
}
}
