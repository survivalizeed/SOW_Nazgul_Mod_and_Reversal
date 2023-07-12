#pragma once

namespace modification {
	inline void modify_elven_run(uintptr_t* pos, __int64* time_func_param2, float elven_run_speed, int elven_run_smtd) {
		using namespace intern::TYPES;
		using namespace intern::FUNCTIONS;
		using namespace intern;

		if (pos == nullptr || time_func_param2 == nullptr) return;

		static Vec3_XZ prevPos{};
		static Vec3_XZ defPosition{ 0.f, 0.f , 0.f };

		static bool time_trigger_once = false;
		static int time_counter = 0;

		static int counter = 0;

		Vec3_XZ* position = (Vec3_XZ*)pos;
		if (counter == 2) {
			prevPos = *position;
			counter = 0;
		}
		Vec3_XZ diff = *position - prevPos;
		if (diff.len() > 20.f && GLOW::blocker) {
			time_counter = 0;
			if (!time_trigger_once) {
				TIME::detour(2, *time_func_param2);
				TIME::blocker = true;
				time_trigger_once = true;
			}
			diff.normalize();
			position->x = position->x + diff.x * elven_run_speed;
			position->z = position->z + diff.z * elven_run_speed;
		}
		else if (TIME::blocker) {
			if (time_counter == 200 + elven_run_smtd) {
				TIME::blocker = false;
				time_trigger_once = false;
				TIME::detour(2, 0);
			}
			time_counter++;
		}
		counter++;
	}

	inline void modify_entity_tp(uintptr_t* pos, float dash_max_distance) {
		using namespace intern::TYPES;
		using namespace intern::FUNCTIONS;
		using namespace intern;
		
		if (pos == nullptr || !GLOW::blocker) return;

		auto max_dist = dash_max_distance;
		Vec3* player = (Vec3*)pos;
		Vec3* closest_entity = nullptr;
		static std::vector<Vec3*> last_entitys;
		for (auto& entity : ENTITY::entity_list) {
			if (isBadReadPtr((void*)entity)) {
				continue;
			}
			auto current_entity = (Vec3*)entity;
			if (current_entity->x == 0.f || current_entity->y == 0.f || current_entity->z == 0.f)
				continue;
			auto dist = calc_vec_dist(player, current_entity);
			if (dist < max_dist && dist > 200.f && abs_vec_diff(player, current_entity).y < 400.f) {
				auto angle = calc_angle_player_camera_entity_horizontal(player, current_entity);
				if (angle < 30 || angle > 330) {
					max_dist = dist;
					closest_entity = current_entity;
				}
			}
		}	
		if (closest_entity != nullptr) {
			auto dir = calc_vec_dir(closest_entity, player, 100.f);
			for (int i = 0; i < 100; ++i) {
				player->y = closest_entity->y + (dir.x * 100.f);
				player->x = closest_entity->x + (dir.y * 100.f);
				player->z = closest_entity->z + (dir.z * 100.f);
				Sleep(1);
			}
			ENTITY::entity_list.clear();
		}
	}
}