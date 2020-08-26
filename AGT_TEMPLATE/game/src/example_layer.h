#pragma once
#include <engine.h>
#include "player.h"
#include "bounding_box_bullet.h" 
#include "alpha_sphere.h"
#include "traffic1.h"
#include "traffic2.h"
#include "cross_fade.h"
#include <vector>
#include <iostream>
#include "billboard.h"
#include "end_game.h"

class pickup2;
class pickup;
class health;
class traffic1;
//class traffic2;

class example_layer : public engine::layer
{
public:
	example_layer();
	~example_layer();
	bool started = false;
	void on_update(const engine::timestep& time_step) override;
	void on_render() override;
	void on_event(engine::event& event) override;
	void update_headlight();
	void update_camera();
	void increase_health();
	void decrease_health(int dec);
	void update_screen_overlay();
	void f_end_game();
	void start_game();
	void decrease_health();
private:
	void render_sphere();
	float generate_random(float LO, float HI);
	
	
	float healthnum;
	float fuel;
	float left;
	float right;
	int score = 0;
	int final_score;
	float player_pos_z;
	bool game_over = false;
	int game_timer = 25;
	int tick = 0;
	std::string destroyed = "Destroyed";

	std::string healthstr;
	std::string scorestr;
	std::string timerstr;
	std::string final_score_str;

	std::vector <traffic2*> car_array;
	std::vector<pickup*> pickup_array;

	engine::ref<engine::game_object>m_tetrahedron{};
	engine::ref<engine::game_object> m_roof1{};
	engine::ref<engine::game_object> m_office{};
	engine::ref<engine::skybox>			m_skybox{};
	engine::ref<engine::game_object>	m_terrain{};
	engine::ref<engine::game_object>	m_cow{};
	engine::ref<engine::game_object>	m_tree{};
	engine::ref<engine::game_object>	m_ball{};
	engine::ref<engine::material>    m_tetrahedron_material{};
	engine::ref<engine::material>		m_material{};
	engine::ref<engine::game_object> m_bus{};


	engine::ref<cross_fade>							m_cross_fade{};
	engine::ref<end_game> m_end_game{};

	//bounding_box m_ai_car2_box;
	bounding_box m_player_box;
	//bounding_box m_ai_car_box;
	//bounding_box m_ai_car_box2;
	//bounding_box m_ai_car_box3;
	bounding_box m_bus_box;

	//glm::veglm::vec3(5.f, 0.5f, -7.5f);


	int timer = 0;
	//int game_timer;


	engine::ref<alpha_sphere>						m_alpha_sphere{};
	engine::ref<billboard>							m_billboard{};

	engine::ref<engine::game_object>m_tetrahedron1{};
	engine::ref<engine::game_object>m_tetrahedron2{};
	engine::ref<engine::game_object>m_tetrahedron3{};
	engine::ref<engine::game_object>m_tetrahedron4{};
	engine::ref<engine::game_object>    m_car{};

	engine::ref<engine::game_object> m_ai_car1{};
	engine::ref<engine::game_object> m_ai_car1_2{};
	engine::ref<engine::game_object> m_ai_car1_3{};
	engine::ref<engine::game_object> m_ai_car1_4{};
	engine::ref<engine::game_object> m_ai_car1_5{};
	engine::ref<engine::game_object> m_ai_car1_6{};
	engine::ref<engine::game_object> m_ai_car1_7{};
	engine::ref<engine::game_object> m_ai_car1_8{};

	engine::ref<engine::game_object> m_ai_car2;
	

	engine::ref<engine::game_object>    m_tree2{};
	engine::ref<health>    m_health{};

	
	engine::ref<engine::game_object>    m_player{};
	engine::ref<pickup>    m_pickup{};
	engine::ref<pickup>    m_pickup_2{};
	engine::ref<pickup>    m_pickup_3{};
	engine::ref<pickup>    m_pickup_4{};
	engine::ref<pickup>    m_pickup_5{};

	//engine::ref<pickup2> m_pickup_t2{};
	//engine::ref<pickup2> m_pickup_t2_2{};

	engine::ref<engine::game_object> m_pickup_t2_obj{};
	engine::ref<engine::game_object> m_house{};

	engine::ref<engine::bullet_manager> m_physics_manager{};
	player player;
	traffic1 m_traffic1;


	traffic2* m_traffic2 = new traffic2;
	traffic2* m_traffic2_2 = new traffic2;
	traffic2* m_traffic2_3 = new traffic2;
	traffic2* m_traffic2_4 = new traffic2;
	traffic2* m_traffic2_5 = new traffic2;
	traffic2* m_traffic2_6 = new traffic2;
	traffic2* m_traffic2_7 = new traffic2;



	engine::SpotLight					m_spotLight;
	uint32_t							num_spot_lights = 1;

	// boolean to start game
	bool moveCamera = false;
	bool m_fog;
	engine::ref<engine::skinned_mesh>   m_skinned_mesh;
	engine::DirectionalLight            m_directionalLight;

	engine::PointLight m_pointLight;
	uint32_t num_point_lights = 1;

	float                               m_running_time = 0.f;
	float								m_anim_timer = 0.0f;
	glm::vec3							m_anim_displacement;

	float pickup_pos[10];
	float health_pos[10];

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};


	engine::ref<engine::audio_manager>  m_audio_manager{};
	float								m_prev_sphere_y_vel = 0.f;
	engine::ref<engine::text_manager>	m_text_manager{};

	engine::orthographic_camera       m_2d_camera;
	engine::perspective_camera        m_3d_camera;
};
