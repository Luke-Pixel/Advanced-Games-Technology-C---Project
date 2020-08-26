#pragma once
#include <engine.h>

class traffic1
{
public:
	traffic1();
	~traffic1();

	void initialise(engine::ref<engine::game_object> object);
	void throttle(const engine::perspective_camera& camera, float kick);
	void on_update(const engine::timestep& time_step);
	void on_render(const engine::ref<engine::shader>& shader);
	void steer_l(float angle);
	void ai_behaviour();
	void set_player_position(glm::vec3 pos) { m_player_position = pos; }
	glm::vec3 last_position() const { return m_last_position; }

	engine::ref<engine::game_object> object() const { return m_object; }

private:
	engine::ref<engine::game_object> m_object;

	glm::vec3 m_instantaneous_acceleration{ 0.f };
	glm::vec3 m_instantaneous_angular_acceleration{ 0.f };

	bool seen = false;

	float m_speed = 300.f;

	float m_contact_time = 0.0f;

	float m_rotational_inertia = 0.f;

	float m_phi, m_theta;

	glm::vec3 m_player_position;
	glm::vec3 m_my_position;

	glm::vec3 m_last_position{ 0.f };
	glm::vec3 max_velocity;
	glm::vec3 desired_velocity;
	glm::vec3 steering;
	glm::vec3 my_velocity;
	// Methods to handle collisions with the ground
	
};
