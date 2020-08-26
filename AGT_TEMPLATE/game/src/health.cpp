#include "health.h"

health::health(const engine::game_object_properties props):engine::game_object(props)
{}

health::~health()
{}

void health::init()
{
	m_is_active = true;
	m_is_collected = false;
}

void health::update(glm::vec3 c, float dt)
{
	if (m_is_active == false)
		m_is_collected = true;

	set_rotation_amount(rotation_amount() + dt * 4.5f);

	if (glm::distance(c, position()) < 1.0f) {
		m_is_active = false;
		
	}
}

void health::set_collected(bool col) {
	m_is_collected = col;
}


engine::ref<health> health::create(const engine::game_object_properties& props)
{
	return std::make_shared<health>(props);
}
