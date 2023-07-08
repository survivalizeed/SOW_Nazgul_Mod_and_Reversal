#pragma once

#include "../pch.h"

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
		if (diff.len() > 16.5f && GLOW::blocker) {
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

	inline void modify_entity_tp(uintptr_t* pos) {
		using namespace intern::TYPES;
		using namespace intern::FUNCTIONS;
		using namespace intern;
		
		if (pos == nullptr) return;

		auto min_dist = 9999999999.f;
		Vec3* player = (Vec3*)pos;
		Vec3* closest_entity = nullptr;
		for (auto& entity : ENTITY::entity_list) {
			if (isBadReadPtr((void*)entity))
				continue;
			auto current_entity = (Vec3*)entity;
			if (current_entity->x == 0.f || current_entity->y == 0.f || current_entity->z == 0.f)
				continue;
			auto dist = calc_vec_dist(player, current_entity);
			if (dist < min_dist && dist > 60.f) {
				auto angle = calc_vec_angle_horizontal(player, current_entity);
				if (angle < 25 || angle > 335) {
					min_dist = dist;
					closest_entity = current_entity;
				}
			}
		}
		if (closest_entity != nullptr) {
			for (int i = 0; i < 999999; ++i) {
				auto dir = calc_vec_dir(player, closest_entity);
				if (dir.x == 0.f && dir.y == 0.f && dir.z == 0.f)
					break;
				player->x += dir.x * 5.f;
				player->y += dir.y * 5.f;
				player->z += dir.z * 5.f;
				Sleep(1);
			}
		}
	}
}