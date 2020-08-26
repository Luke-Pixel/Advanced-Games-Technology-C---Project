#pragma once
#include <engine.h>

class quad;

class end_game
{
public:
	end_game(const std::string& path, float max_time, float width, float height);
	~end_game();

	void on_update(const engine::timestep& time_step);
	void on_render(engine::ref<engine::shader> shader);
	void activate();

	static engine::ref<end_game> create(const std::string& path, float max_time, float width, float height);

private:
	bool s_active;

	float m_timer;
	float m_max_time;

	engine::ref<engine::texture_2d> m_texture;
	float m_transparency;
	engine::ref<quad> m_quad;
};
