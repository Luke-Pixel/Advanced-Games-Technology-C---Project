#pragma once
#include <engine.h>

class health : public engine::game_object {
public:
	health(const engine::game_object_properties props);
	~health();
	void init();
	void update(glm::vec3 c, float dt);
	bool active() { return m_is_active; }
	bool collected() { return m_is_collected; }
	void set_collected(bool col);
	static engine::ref<health> create(const engine::game_object_properties& props);
private:
	bool m_is_active;		// Indicates if pickup is active
	bool m_is_collected;
};


