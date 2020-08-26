#include "pickup2.h"

pickup2::pickup2(const engine::game_object_properties props) : engine::game_object(props)
{}

pickup2::~pickup2()
{}

void pickup2::init(engine::ref<engine::game_object> object)
{
	m_is_active = true;
	m_is_collected = false;
	m_object = object;
}

void pickup2::update(glm::vec3 c, float dt)
{
	if (m_is_active == false)
		m_is_collected = true;

	set_rotation_amount(rotation_amount() + dt * 4.5f);

	if (glm::distance(c, position()) < 1.0f) m_is_active = false;
	m_player_position = c;

	ai_behaviour();
}

void pickup2::set_collected(bool col) {
	m_is_collected = col;
}

engine::ref<pickup2> pickup2::create(const engine::game_object_properties& props)
{
	return std::make_shared<pickup2>(props);
}

void pickup2::ai_behaviour() {
	//m_object->set_velocity(m_object->forward() * 2.f);
	desired_velocity = normalize(m_player_position - m_object->position());
	steering = desired_velocity - m_object->velocity();

	//steering = trunc(steering, max_force);
	steering = steering / m_object->mass();
	m_object->set_angular_velocity(steering);

	m_object->set_velocity(-(m_object->velocity() + steering));

	m_object->position() = m_object->position() + m_object->velocity();

}



