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
		if (a.beats_until_attack > -1) return false;

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
			return false;
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

std::vector<glm::ivec2> gen_moves(glm::ivec2 pos, tempo::ComponentStage &s)
{
	std::vector<glm::ivec2> moves;
	for (int I = -1; I < 2; I+=2)
	{
		for (int J = -1; J < 2; J+=2)
		{
			glm::ivec2 delta(I, J);
			glm::ivec2 move = pos + delta;
			if (s.isNavigable(move)) moves.push_back(move);
		}
	}

	return moves;
}

struct node {
	glm::ivec2 pos;
	int G;
	int H;
	int F;
	
	node()
	{
		pos = glm::ivec2(0, 0);
		G = 0;
		H = 0;
		F = 0;
	}
	node(glm::ivec2 p, int g, glm::ivec2 tgt)
	{
		pos = p;
		G = g;

		glm::ivec2 diff = tgt - pos;
		H = abs(diff.x) + abs(diff.y);

		F = G + H;
	}
};

node pop_min(std::vector<node>& list)
{
	int min = INT_MAX;
	node min_node;
	int min_index = 0;

	int index = 0;
	for (node n : list)
	{
		if (n.G + n.H < min)
		{
			min = n.G + n.H;
			min_node = n;
			min_index = index;
		}
		index++;
	}

	list.erase(list.begin() + min_index);
	return min_node;
}

bool contains (std::vector<node>& list, node m)
{
	for (node n : list)
	{
		if (n.pos == m.pos) return true;
	}

	return false;
}

bool update (std::vector<node>& list, node m)
{
	for (node& n : list)
	{
		if (n.pos == m.pos && n.F > m.F)
		{
			// n.
		}
	}

	return false;
}

void Astar_pathfind(glm::ivec2 pos, glm::vec2 tgt, std::deque<glm::ivec2> &path,
                    tempo::ComponentStage &s)
{
	std::vector<node> open;
	std::vector<node> closed;

	node n(pos, 0, tgt);
	open.push_back(n);

	node S = pop_min(open);
	closed.push_back(S);

	std::vector<glm::ivec2> moves = gen_moves(S.pos, s);
	for (glm::ivec2 T : moves)
	{
		node N(T, S.G + 1, tgt);
		if (  contains(closed, N)) continue;
		if (! contains(open,   N)) open.push_back(N);
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
			case tempo::MoveType::MOVE_SNAKE:
			{
				for (auto &parent : entities)
				{
					auto &p_ai = parent.getComponent<tempo::ComponentAI>();
					auto &p_sp = parent.getComponent<tempo::ComponentStagePosition>();

					if (p_ai.type == tempo::MoveType::MOVE_SNAKE &&
					    p_ai.index == ai.index - 1)
					{
						glm::ivec2 delta = p_sp.getOrigin() - sp.getOrigin();
						float len = glm::length((glm::vec2) delta);
						if ( len > 1 ) break;
						st.delta = delta;
						break;
					}
				}
				if(st.delta == glm::ivec2(0, 0))
				{
					//we're the head
					st.delta = random_move();
					if (st.delta == sr.facing * -1) st.delta *= -1; //Nudge to go forwards(ish)
				}
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
