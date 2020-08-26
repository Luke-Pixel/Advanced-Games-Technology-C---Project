#include "pickup.h"

pickup::pickup(const engine::game_object_properties props) : engine::game_object(props)
{}

pickup::~pickup()
{}

void pickup::init()
{
	m_is_active = true;
	m_is_collected = false;
}

void pickup::update(glm::vec3 c, float dt)
{
	if (m_is_active == false)
		m_is_collected = true;

	set_rotation_amount(rotation_amount() + dt * 4.5f);

	if (glm::distance(c, position()) < 1.0f) m_is_active = false;
}

void pickup::set_collected(bool col) {
	m_is_collected = col;
}

engine::ref<pickup> pickup::create(const engine::game_object_properties& props)
{
	return std::make_shared<pickup>(props);
}
