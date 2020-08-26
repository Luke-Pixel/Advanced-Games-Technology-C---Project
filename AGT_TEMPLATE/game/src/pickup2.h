#pragma once
#include <engine.h>

class pickup2 : public engine::game_object {
public:
	pickup2(const engine::game_object_properties props);
	~pickup2();
	void init(engine::ref<engine::game_object> object);
	void update(glm::vec3 c, float dt);
	bool active() { return m_is_active; }
	bool collected() { return m_is_collected; }
	void set_collected(bool col);
	void ai_behaviour();
	void set_player_position(glm::vec3 pos) { m_player_position = pos; }
	static engine::ref<pickup2> create(const engine::game_object_properties& props);
private:
	bool m_is_active;		// Indicates if pickup is active
	bool m_is_collected;    //indicates is pickup item has been collected
	engine::ref<engine::game_object> m_object;
	glm::vec3 m_player_position;
	glm::vec3 max_velocity;
	glm::vec3 desired_velocity;
	glm::vec3 steering;
};



