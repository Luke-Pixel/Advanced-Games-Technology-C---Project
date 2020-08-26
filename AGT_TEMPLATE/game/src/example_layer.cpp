#include "example_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"
#include "engine/utils/track.h"
#include "pickup.h"
#include "pickup2.h"
#include "health.h"

example_layer::example_layer()
	:m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f),
	m_3d_camera((float)engine::application::window().width(), (float)engine::application::window().height())



{

	m_fog = true;
	glm::vec3 m_fog_colour = glm::vec3(0.5f);
	int m_fog_factor_type = 3;

	// Hide the mouse and lock it inside the window
	//engine::input::anchor_mouse(true);
	engine::application::window().hide_mouse_cursor();

	// Initialise audio and play background music
	m_audio_manager = engine::audio_manager::instance();
	m_audio_manager->init();
	m_audio_manager->load_sound("assets/audio/bounce.wav", engine::sound_type::event, "bounce"); // Royalty free sound from freesound.org
	m_audio_manager->load_sound("assets/audio/music.mp3", engine::sound_type::track, "music");  // Royalty free music from https://freemusicarchive.org/music/BoxCat_Games
	m_audio_manager->load_sound("assets/audio/horn1.mp3", engine::sound_type::event, "horn"); //https://www.zapsplat.com/?s=car+horn&post_type=music&sound-effect-category-id= 
	m_audio_manager->load_sound("assets/audio/crash1.mp3", engine::sound_type::event, "crash"); //https://www.zapsplat.com/sound-effect-category/vehicle-impacts/ 
	m_audio_manager->load_sound("assets/audio/pickup.mp3", engine::sound_type::event, "pickup"); //https://www.zapsplat.com/?s=game+item&post_type=music&sound-effect-category-id= 
	
	//m_audio_manager->play("music");

	//create array with x and z cordinates for health and pickup items
	for (int i = 0; i <= 4; ++i) {
		pickup_pos[i] = generate_random(-2.8f, 3.5);
		health_pos[i] = generate_random(-2.8f, 3.5);
	}

	//create array with x and z cordinates for health and pickup items
	for (int i = 4; i <= 9; ++i) {
		pickup_pos[i] = generate_random(-80.f, -10.f);
		health_pos[i] = generate_random(-80.f, -10.f);
	}

	//Initialise the shaders, materials and lights
	auto mesh__material_shader = engine::renderer::shaders_library()->get("mesh_material");
	auto mesh_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");



	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.25f;
	m_directionalLight.DiffuseIntensity = 0.6f;
	m_directionalLight.Direction = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));

	//Spot light to be used with charecter model 
	m_spotLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_spotLight.AmbientIntensity = 0.55f;
	m_spotLight.DiffuseIntensity = 0.6f;
	m_spotLight.Position = glm::vec3(0.f, 4.f, -20.f);
	m_spotLight.Direction = glm::normalize(glm::vec3(.0f, .0f, -1.0f));
	m_spotLight.Cutoff = 0.5f;
	m_spotLight.Attenuation.Constant = 1.0f;
	m_spotLight.Attenuation.Linear = 0.1f;
	m_spotLight.Attenuation.Exp = 0.01f;


	//m_pointLight.submit(mesh__material_shader);
	// set color texture unit
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("lighting_on", true);
    m_directionalLight.submit(animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity",
		0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("transparency", 1.0f);

	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("lighting_on", true);
	m_directionalLight.submit(mesh__material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gMatSpecularIntensity",
		1.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("gSpecularPower", 10.f);
	//Fog effect 
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh__material_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gColorMap", 0);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("lighting_on", true);
    m_directionalLight.submit(mesh_lighting_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gMatSpecularIntensity",
		0.5f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("gSpecularPower", 5.f);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("transparency", 1.0f);
	//fog effect 
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("fog_colour", m_fog_colour);
	std::dynamic_pointer_cast<engine::gl_shader>(mesh_lighting_shader)->set_uniform("fog_factor_type", m_fog_factor_type);
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->bind();
	std::dynamic_pointer_cast<engine::gl_shader>(text_shader)->set_uniform("projection",
		glm::ortho(0.f, (float)engine::application::window().width(), 0.f,
		(float)engine::application::window().height()));
	m_material = engine::material::create(1.0f, glm::vec3(1.0f, 0.1f, 0.07f),
		glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);
	//m_material = engine::material::create(32.0f, glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(1.0f, 0.1f, 0.07f), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f);
	
	// Skybox texture from https://www.dropbox.com/sh/phslacd8v9i17wb/AABui_-C-yhKvZ1H2wb3NykIa?dl=0
	m_skybox = engine::skybox::create(50.f,
		{ engine::texture_2d::create("assets/textures/skybox/back.bmp",true),
		  engine::texture_2d::create("assets/textures/skybox/right.bmp",true),
		  engine::texture_2d::create("assets/textures/skybox/front.bmp",true),
		  engine::texture_2d::create("assets/textures/skybox/left.bmp",true),
		  engine::texture_2d::create("assets/textures/skybox/top.bmp",true),
		  engine::texture_2d::create("assets/textures/skybox/bottom.bmp",true)
		});

	m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
	m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");

	// Load the terrain texture and create a terrain mesh. Create a terrain object. Set its properties
	//terrain texture from https://opengameart.org/content/nature-texture-pack-256x256
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/snow1.png", true) };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 750.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	terrain_props.is_static = true;
	terrain_props.type = 0;
	terrain_props.friction = 0.0f;
	terrain_props.bounding_shape = glm::vec3(100.f, 0.5f, 750.f);
	terrain_props.restitution = 0.92f;
	m_terrain = engine::game_object::create(terrain_props);

	//tetrahedron
	std::vector<glm::vec3> tetrahedron_vertices;
	tetrahedron_vertices.push_back(glm::vec3(0.f, 0.5f, 0.f));//0
	tetrahedron_vertices.push_back(glm::vec3(0.f, 0.f, 0.5f));//1
	tetrahedron_vertices.push_back(glm::vec3(-0.5f, 0.f, -0.5f)); //2
	tetrahedron_vertices.push_back(glm::vec3(0.5f, 0.f, -0.5f)); //3

	//create vertices for roof for house
	std::vector<glm::vec3> roof_vertices;
	roof_vertices.push_back(glm::vec3(0.f, 1.f, -1.5f));//0
	roof_vertices.push_back(glm::vec3(0.f, 0.f, 1.f));//1
	roof_vertices.push_back(glm::vec3(-2.5f, 0.f, -1.5f)); //2
	roof_vertices.push_back(glm::vec3(2.5f, 0.f, -1.5f)); //3

	engine::ref<engine::tetrahedron> tetrahedron_shape =
		engine::tetrahedron::create(tetrahedron_vertices);
	engine::game_object_properties tetrahedron_props;
	tetrahedron_props.position = { 0.f, 0.5f, -20.0f };
	tetrahedron_props.meshes = { tetrahedron_shape->mesh() };
	m_tetrahedron = engine::game_object::create(tetrahedron_props);

	m_tetrahedron_material = engine::material::create(32.0f,
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(1.0f, 0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		0.3f);


	//load roofs, roof texture from https://forum.rising-world.net/attachment/18657-texture-roof-lugher-texture-library-new-ceramic-roof-tiles-texture-of-ceramic-ro/
	engine::ref<engine::tetrahedron> roof1_shape =
		engine::tetrahedron::create(roof_vertices);
	engine::ref<engine::texture_2d> roof_texture = engine::texture_2d::create("assets/textures/roof.jpg",true);
	engine::game_object_properties roof1_props;
	roof1_props.position = { 0.f, 0.5f, -20.0f };
	roof1_props.meshes = { roof1_shape->mesh() };
	roof1_props.textures = { roof_texture };
	m_roof1 = engine::game_object::create(roof1_props);
	engine::ref<engine::tetrahedron> roof2_shape =
		engine::tetrahedron::create(roof_vertices);
	engine::game_object_properties roof2_props;
	roof2_props.position = { 0.f, 0.5f, -20.0f };
	roof2_props.meshes = { roof2_shape->mesh() };
	roof2_props.textures = { roof_texture };
	m_roof1 = engine::game_object::create(roof2_props);

	//load player model from http://www.cadnav.com/3d-models/model-43219.html
	engine::ref<engine::model>player_model = engine::model::create("assets/models/static/Model_D1202A086/car.obj");
	engine::game_object_properties player_props;
	player_props.meshes = player_model->meshes();
	player_props.textures = player_model->textures();
	float player_scale = 1.f / glm::max(player_model->size().x, glm::max(player_model->size().y, player_model->size().z));
	player_props.position = { 0.f,0.5f,5.f };
	player_props.type = 0;
	player_props.bounding_shape = player_model->size();
	player_props.scale = glm::vec3(player_scale * 2.f);
	m_player = engine::game_object::create(player_props);
	m_player->set_offset(player_model->offset());
	//set bounding box for player model 
	m_player_box.set_box(player_props.bounding_shape.x * 2.f * player_scale,
		player_props.bounding_shape.y * 2.f * player_scale, player_props.bounding_shape.z * 2.f
		* player_scale, player_props.position - glm::vec3(0.f, m_player->offset().y, 0.f) *
		m_player->scale());
	player_pos_z = m_player->position().z;
 
	//load AI model
	/*the below code creates the AI objects for the game , i load the same models repetaedly and set them
	to object variables defined in the .h file, each AI is set to a different position */
	engine::ref <engine::model>  bus_model = engine::model::create("assets/models/static/GLX-400-3DS/GLX_400.3DS");
	engine::game_object_properties bus_props;
	bus_props.meshes = bus_model->meshes();
	bus_props.textures = bus_model->textures();
	float bus_scale = 1.f / glm::max(bus_model->size().x, glm::max(bus_model->size().y, bus_model->size().z));
	bus_props.position = { 2.5f,1.5f, -30.f };
	bus_props.scale = glm::vec3(bus_scale);
	//minivan_props.mass = 100.f;
	bus_props.type = 0;
	bus_props.bounding_shape = bus_model->size();
	m_bus = engine::game_object::create(bus_props);
	m_bus->set_offset(bus_model->offset());
	m_bus_box.set_box(bus_props.bounding_shape.x * 2.f * bus_scale,
		bus_props.bounding_shape.y * 2.f * bus_scale, bus_props.bounding_shape.z * 2.f
		* bus_scale, bus_props.position - glm::vec3(0.f, m_bus->offset().y, 0.f) *
		m_bus->scale());

	//load AI model https://free3d.com/3d-model/low-poly-car-40967.html
	engine::ref <engine::model>  ai_car_model = engine::model::create("assets/models/static/AI Car/Car1/Car.3ds");
	engine::game_object_properties ai_car_props;
	ai_car_props.meshes = ai_car_model->meshes();
	ai_car_props.textures = ai_car_model->textures();
	float ai_car_scale = 1.f / glm::max(ai_car_model->size().x, glm::max(ai_car_model->size().y, ai_car_model->size().z));
	ai_car_props.position = { m_player->position().x + .5f,1.5f, (m_player->position().z - 10.f) };
	ai_car_props.scale = glm::vec3(ai_car_scale);
	ai_car_props.type = 0;
	ai_car_props.bounding_shape = ai_car_model->size() / 2.f;
	m_ai_car1 = engine::game_object::create(ai_car_props);
	//m_ai_car1->set_offset(ai_car_model->offset());
	//m_ai_car_box.set_box(ai_car_props.bounding_shape.x * 2.f * ai_car_scale,
		//ai_car_props.bounding_shape.y * 2.f * ai_car_scale, ai_car_props.bounding_shape.z * 2.f
		//* ai_car_scale, ai_car_props.position - glm::vec3(0.f, m_ai_car1->offset().y, 0.f) *
	//	m_ai_car1->scale());

	//load AI model https://free3d.com/3d-model/low-poly-car-40967.html
	engine::ref <engine::model>  ai_car_model_2 = engine::model::create("assets/models/static/AI Car/Car1/Car.3ds");
	engine::game_object_properties ai_car_props_2;
	ai_car_props_2.meshes = ai_car_model_2->meshes();
	ai_car_props_2.textures = ai_car_model_2->textures();
	float ai_car_scale_2 = 1.f / glm::max(ai_car_model_2->size().x, glm::max(ai_car_model_2->size().y, ai_car_model_2->size().z));
	ai_car_props_2.position = { -1.5f, 1.5f, -25.f };
	ai_car_props_2.scale = glm::vec3(ai_car_scale_2);
	ai_car_props_2.type = 0;
	//ai_car_props_2.friction = 0;
	ai_car_props_2.bounding_shape = ai_car_model_2->size() / 2.f;
	m_ai_car1_2 = engine::game_object::create(ai_car_props_2);
	/*m_ai_car1_2->set_offset(ai_car_model_2->offset());
	m_ai_car_box2.set_box(ai_car_props_2.bounding_shape.x * 2.f * ai_car_scale_2,
		ai_car_props_2.bounding_shape.y * 2.f * ai_car_scale_2, ai_car_props_2.bounding_shape.z * 2.f
		* ai_car_scale_2, ai_car_props_2.position - glm::vec3(0.f, m_ai_car1_2->offset().y, 0.f) *
		m_ai_car1_2->scale());*/

	//load AI model https://www.turbosquid.com/FullPreview/Index.cfm/ID/1055619
	engine::ref <engine::model>  ai_car_model_3 = engine::model::create("assets/models/static/GLX-400-3DS/GLX_400.3DS");
	engine::game_object_properties ai_car_props_3;
	ai_car_props_3.meshes = ai_car_model_3->meshes();
	ai_car_props_3.textures = ai_car_model_3->textures();
	float ai_car_scale_3 = 1.f / glm::max(ai_car_model_3->size().x, glm::max(ai_car_model_3->size().y, ai_car_model_3->size().z));
	ai_car_props_3.position = { 0, 1.5, -39 };
	ai_car_props_3.scale = glm::vec3(ai_car_scale_3);
	ai_car_props_3.type = 0;
	ai_car_props_3.bounding_shape = ai_car_model_3->size() / 2.f;
	m_ai_car1_3 = engine::game_object::create(ai_car_props_3);
	m_ai_car1_3->set_offset(ai_car_model_3->offset());
	/*m_ai_car_box3.set_box(ai_car_props_3.bounding_shape.x * 2.f * ai_car_scale_3,
		ai_car_props_3.bounding_shape.y * 2.f * ai_car_scale_3, ai_car_props_3.bounding_shape.z * 2.f
		* ai_car_scale_3, ai_car_props_3.position - glm::vec3(0.f, m_ai_car1_3->offset().y, 0.f) *
		m_ai_car1_3->scale());*/

		//load AI model https://free3d.com/3d-model/low-poly-car-40967.html
	engine::ref <engine::model>  ai_car_model_4 = engine::model::create("assets/models/static/AI Car/Car1/Car.3ds");
	engine::game_object_properties ai_car_props_4;
	ai_car_props_4.meshes = ai_car_model_4->meshes();
	ai_car_props_4.textures = ai_car_model_4->textures();
	float ai_car_scale_4 = 1.f / glm::max(ai_car_model_4->size().x, glm::max(ai_car_model_4->size().y, ai_car_model_4->size().z));
	ai_car_props_4.position = { 1.5f, 1.5f, -60.f };
	ai_car_props_4.scale = glm::vec3(ai_car_scale_4);
	ai_car_props_4.type = 0;
	//ai_car_props_2.friction = 0;
	ai_car_props_4.bounding_shape = ai_car_model_4->size() / 2.f;
	m_ai_car1_4 = engine::game_object::create(ai_car_props_4);
	/*m_ai_car1_2->set_offset(ai_car_model_2->offset());
	m_ai_car_box2.set_box(ai_car_props_2.bounding_shape.x * 2.f * ai_car_scale_2,
		ai_car_props_2.bounding_shape.y * 2.f * ai_car_scale_2, ai_car_props_2.bounding_shape.z * 2.f
		* ai_car_scale_2, ai_car_props_2.position - glm::vec3(0.f, m_ai_car1_2->offset().y, 0.f) *
		m_ai_car1_2->scale());*/

		//load AI model https://free3d.com/3d-model/low-poly-car-40967.html
	engine::ref <engine::model>  ai_car_model_5 = engine::model::create("assets/models/static/AI Car/Car1/Car.3ds");
	engine::game_object_properties ai_car_props_5;
	ai_car_props_5.meshes = ai_car_model_5->meshes();
	ai_car_props_5.textures = ai_car_model_5->textures();
	float ai_car_scale_5 = 1.f / glm::max(ai_car_model_5->size().x, glm::max(ai_car_model_5->size().y, ai_car_model_5->size().z));
	ai_car_props_5.position = { -1.7f, 1.5f, -174.f };
	ai_car_props_5.scale = glm::vec3(ai_car_scale_5);
	ai_car_props_5.type = 0;
	//ai_car_props_2.friction = 0;
	ai_car_props_5.bounding_shape = ai_car_model_5->size() / 2.f;
	m_ai_car1_5 = engine::game_object::create(ai_car_props_5);
	/*m_ai_car1_2->set_offset(ai_car_model_2->offset());
	m_ai_car_box2.set_box(ai_car_props_2.bounding_shape.x * 2.f * ai_car_scale_2,
		ai_car_props_2.bounding_shape.y * 2.f * ai_car_scale_2, ai_car_props_2.bounding_shape.z * 2.f
		* ai_car_scale_2, ai_car_props_2.position - glm::vec3(0.f, m_ai_car1_2->offset().y, 0.f) *
		m_ai_car1_2->scale());*/

		//load AI model https://free3d.com/3d-model/low-poly-car-40967.html
	engine::ref <engine::model>  ai_car_model_6 = engine::model::create("assets/models/static/AI Car/Car1/Car.3ds");
	engine::game_object_properties ai_car_props_6;
	ai_car_props_6.meshes = ai_car_model_6->meshes();
	ai_car_props_6.textures = ai_car_model_6->textures();
	float ai_car_scale_6 = 1.f / glm::max(ai_car_model_6->size().x, glm::max(ai_car_model_6->size().y, ai_car_model_6->size().z));
	ai_car_props_6.position = { -1.7f, .5f, -234.f };
	ai_car_props_6.scale = glm::vec3(ai_car_scale_6);
	ai_car_props_6.type = 0;
	//ai_car_props_2.friction = 0;
	ai_car_props_6.bounding_shape = ai_car_model_5->size() / 2.f;
	m_ai_car1_6 = engine::game_object::create(ai_car_props_6);

	//load AI model https://www.turbosquid.com/FullPreview/Index.cfm/ID/1055619
	engine::ref <engine::model>  ai_car_model_7 = engine::model::create("assets/models/static/GLX-400-3DS/GLX_400.3DS");
	engine::game_object_properties ai_car_props_7;
	ai_car_props_7.meshes = ai_car_model_7->meshes();
	ai_car_props_7.textures = ai_car_model_7->textures();
	float ai_car_scale_7 = 1.f / glm::max(ai_car_model_7->size().x, glm::max(ai_car_model_7->size().y, ai_car_model_7->size().z));
	ai_car_props_7.position = { 1.7f, .5f, -350.f };
	ai_car_props_7.scale = glm::vec3(ai_car_scale_7);
	ai_car_props_7.type = 0;
	//ai_car_props_2.friction = 0;
	ai_car_props_7.bounding_shape = ai_car_model_7->size() / 2.f;
	m_ai_car1_7 = engine::game_object::create(ai_car_props_7);
	//End of AI

	//Load model for pickups 
	//laod pickup, pickup texture from https://www.rgbstock.com/photo/p7lQBLm/Seamless+Glass+Tile+7
	engine::ref<engine::cuboid> pickup_shape = engine::cuboid::create(glm::vec3(0.15f), false);
	engine::ref<engine::texture_2d> pickup_texture = engine::texture_2d::create("assets/textures/gemtexture.jpg",true);
	engine::game_object_properties pickup_props;
	pickup_props.position = { 0.f, 1.f, -50.f };
	pickup_props.meshes = { pickup_shape->mesh() };
	pickup_props.textures = { pickup_texture };
	m_pickup = pickup::create(pickup_props);
	m_pickup->init();

	//laod pickup, pickup texture from https://www.rgbstock.com/photo/p7lQBLm/Seamless+Glass+Tile+7
	engine::ref<engine::cuboid> pickup_2_shape = engine::cuboid::create(glm::vec3(0.15f), false);
	engine::ref<engine::texture_2d> pickup_2_texture = engine::texture_2d::create("assets/textures/gemtexture.jpg", true);
	engine::game_object_properties pickup_2_props;
	pickup_2_props.position = { 0.f, 1.5f, -335.f };
	pickup_2_props.meshes = { pickup_2_shape->mesh() };
	pickup_2_props.textures = { pickup_2_texture };
	m_pickup_2 = pickup::create(pickup_2_props);
	m_pickup_2->init();

	//laod pickup, pickup texture from https://www.rgbstock.com/photo/p7lQBLm/Seamless+Glass+Tile+7
	engine::ref<engine::cuboid> pickup_3_shape = engine::cuboid::create(glm::vec3(0.15f), false);
	engine::ref<engine::texture_2d> pickup_3_texture = engine::texture_2d::create("assets/textures/gemtexture.jpg", true);
	engine::game_object_properties pickup_3_props;
	pickup_3_props.position = { -2.f, 1.5f, -275.f };
	pickup_3_props.meshes = { pickup_3_shape->mesh() };
	pickup_3_props.textures = { pickup_3_texture };
	m_pickup_3 = pickup::create(pickup_3_props);
	m_pickup_3->init();

	//laod pickup, pickup texture from https://www.filterforge.com/filters/11319.jpg
	engine::ref<engine::cuboid> pickup4_shape = engine::cuboid::create(glm::vec3(0.15f), false);
	engine::ref<engine::texture_2d> pickup4_texture = engine::texture_2d::create("assets/textures/gemtexture2.jpg", true);
	engine::game_object_properties pickup_4_props;
	pickup_4_props.position = { -2.f, 1.f, -15.f };
	pickup_4_props.meshes = { pickup4_shape->mesh() };
	pickup_4_props.textures = { pickup4_texture };
	m_pickup_4 = pickup::create(pickup_4_props);
	m_pickup_4->init();

	//laod pickup, pickup texture from https://www.filterforge.com/filters/11319.jpg
	engine::ref<engine::cuboid> pickup5_shape = engine::cuboid::create(glm::vec3(0.15f), false);
	engine::ref<engine::texture_2d> pickup5_texture = engine::texture_2d::create("assets/textures/gemtexture2.jpg", true);
	engine::game_object_properties pickup_5_props;
	pickup_5_props.position = { -2.f, 1.f, -35.f };
	pickup_5_props.meshes = { pickup5_shape->mesh() };
	pickup_5_props.textures = { pickup5_texture };
	m_pickup_5 = pickup::create(pickup_5_props);
	m_pickup_5->init();

	/*
	//laod pickup, pickup texture from https://www.rgbstock.com/photo/p7lQBLm/Seamless+Glass+Tile+7
	engine::ref<engine::cuboid> pickup2_shape = engine::cuboid::create(glm::vec3(0.15f), false);
	engine::ref<engine::texture_2d> pickup2_texture = engine::texture_2d::create("assets/textures/gemtexture2.jpg", true);
	engine::game_object_properties pickup2_props;
	pickup2_props.position = { -2.f, 1.f, -15.f };
	pickup2_props.meshes = { pickup2_shape->mesh() };
	pickup2_props.textures = { pickup2_texture };
	m_pickup_t2 = pickup2::create(pickup2_props);
	m_pickup_t2_obj = engine::game_object::create(pickup2_props);
	m_pickup_t2->init(m_pickup_t2_obj);
	*/



	//load house body shape, texture from https://www.pinterest.com/pin/622974560931648071/
	engine::ref<engine::cuboid> house_shape = engine::cuboid::create(glm::vec3(0.8f), false);
	engine::ref<engine::texture_2d> house_texture = engine::texture_2d::create("assets/textures/cabin.jpg",true);
	engine::game_object_properties house_props;
	house_props.position = { 0.f, 1.8f, -800.f };
	house_props.meshes = { house_shape->mesh() };
	house_props.textures = { house_texture };
	m_house = pickup::create(house_props);

	//load house body shape, texture from https://www.pinterest.com/pin/622974560931648071/
	engine::ref<engine::cuboid> office_shape = engine::cuboid::create(glm::vec3(1.f), false);
	engine::ref<engine::texture_2d> office_texture = engine::texture_2d::create("assets/textures/office.jpg", true);
	engine::game_object_properties office_props;
	office_props.position = { 0.f, 1.8f, -690.f };
	office_props.meshes = { office_shape->mesh() };
	office_props.textures = { office_texture };
	m_office = pickup::create(office_props);

	// load healthkit model from https://www.turbosquid.com/FullPreview/Index.cfm/ID/514293
	engine::ref<engine::model> health_model = engine::model::create("assets/models/static/healthKit/health_pack.3ds");
	engine::game_object_properties health_props;
	health_props.meshes = health_model->meshes();
	health_props.textures = health_model->textures();
	float health_scale = 1.f / glm::max(health_model->size().x, glm::max(health_model->size().y, health_model->size().z));
	health_props.position = { 0.0f,1.2f,-30.0f };
	health_props.scale = glm::vec3(health_scale);
	m_health = health::create(health_props);
	m_health->init();

	
	
	

	// Load the tree model. Create a tree object. Set its properties
	//tree from https://free3d.com/3d-model/low-poly-tree-73217.html
	engine::ref <engine::model> tree_model = engine::model::create("assets/models/static/lowTree2/lowpolytree.3ds");
	engine::game_object_properties tree_props;
	tree_props.meshes = tree_model->meshes();
	tree_props.textures = tree_model->textures();
	float tree_scale = 3.f / glm::max(tree_model->size().x, glm::max(tree_model->size().y, tree_model->size().z));
	tree_props.position = { 4.f, 0.5f, -5.f };
	tree_props.type = 0;
	tree_props.restitution = 0.92f;
	tree_props.bounding_shape = tree_model->size() / 2.f * tree_scale;
	tree_props.scale = glm::vec3(tree_scale);
	m_tree = engine::game_object::create(tree_props);
	m_tree->set_offset(tree_model->offset());
	//m_tree_box.set_box(tree_props.bounding_shape.x * 2.f * tree_scale,
		//tree_props.bounding_shape.y * 2.f * tree_scale, tree_props.bounding_shape.z * 2.f
		//* tree_scale, tree_props.position - glm::vec3(0.f, m_tree->offset().y, 0.f) *
		//m_tree->scale());


	engine::ref<engine::sphere> sphere_shape = engine::sphere::create(10, 20, 0.5f);
	engine::game_object_properties sphere_props;
	sphere_props.position = { 0.f, 5.f, -5.f };
	sphere_props.meshes = { sphere_shape->mesh() };
	sphere_props.type = 1;
	sphere_props.bounding_shape = glm::vec3(0.5f);
	sphere_props.restitution = 0.92f;
	sphere_props.mass = 0.000001f;
	m_ball = engine::game_object::create(sphere_props);


	/*
	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_player);
	//m_game_objects.push_back(m_ai_car2);
	m_game_objects.push_back(m_bus);
	m_game_objects.push_back(m_ai_car1);
	m_game_objects.push_back(m_ai_car1_2);
	m_game_objects.push_back(m_ai_car1_3);
	//m_game_objects.push_back(m_tree);
	//m_game_objects.push_back(m_pickup);
	m_physics_manager = engine::bullet_manager::create(m_game_objects);
	*/
	

	m_text_manager = engine::text_manager::create();

	m_skinned_mesh->switch_animation(1);

	

	m_cross_fade = cross_fade::create("assets/textures/Red.bmp", 2.0f, 1.6f, 0.9f);
	m_end_game = end_game::create("assets/textures/Black.jpg", 2.0f, 1.6f, 0.9f);
	

	const aiNodeAnim* nodeAnim = m_skinned_mesh->animations().at(1)->mChannels[0];
	aiVector3D animStart = nodeAnim->mPositionKeys[0].mValue;
	aiVector3D animEnd = nodeAnim->mPositionKeys[nodeAnim->mNumPositionKeys - 1].mValue;
	aiVector3D displacement = animEnd - animStart;
	m_anim_displacement = glm::vec3(displacement.x, displacement.y, displacement.z);

	//unused
	fuel = 100.f;

	m_billboard = billboard::create("assets/textures/Explosion.tga", 4, 5, 16);

	m_alpha_sphere = alpha_sphere::create(glm::vec3(1.f, 0.f, 1.0f), glm::vec3(0.f, 0.f, 1.f), true, 0.25f, 3.0f);
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step)
{
	

	
	if (started == true) {
		//player.update_camera(m_3d_camera);
		m_physics_manager->dynamics_world_update(m_game_objects, double(time_step));
	}
	m_running_time += time_step;

	m_skinned_mesh->on_update(m_running_time);

	m_anim_timer += time_step;
	if (m_anim_timer > (float)m_skinned_mesh->animations().at(1)->mDuration) m_anim_timer -= m_skinned_mesh->animations().at(1)->mDuration;

	//update pickups
	m_pickup->update(m_player->position(), time_step);
	m_pickup_2->update(m_player->position(), time_step);
	m_pickup_3->update(m_player->position(), time_step);
	m_pickup_4->update(m_player->position(), time_step);
	m_pickup_5->update(m_player->position(), time_step);
	m_health->update(m_player->position(), time_step);
	

	

	update_headlight();

	update_camera();

	//update bounding box
	m_player_box.on_update(m_player->position() - glm::vec3(0.f, m_player->offset().y, 0.f) *
		m_player->scale() * 2.f, m_player->rotation_amount(), m_player->rotation_axis());

	m_bus_box.on_update(m_bus->position() - glm::vec3(0.f, m_bus->offset().y, 0.f) *
		m_bus->scale(), m_bus->rotation_amount(), m_bus->rotation_axis());

	//m_ai_car2_box.on_update(m_ai_car2->position() - glm::vec3(0.f, m_ai_car2->offset().y, 0.f) *
		//m_ai_car2->scale(), m_ai_car2->rotation_amount(), m_ai_car2->rotation_axis());

	if (started == true) {
		tick = tick + 1;
				player.on_update(time_step);

		//m_traffic1.on_update(time_step);
		//m_pickup_t2->update(m_player->position(),time_step);

		//update FX
		m_cross_fade->on_update(time_step);
		m_alpha_sphere->on_update(time_step);
		m_alpha_sphere->set_position(m_player->position());



		//check for powerups and action if collied
		if (m_health->active() == false && m_health->collected() == false)
			increase_health();

		//chek if pickup items have been collected by player
		if (m_pickup->active() == false && m_pickup->collected() == false)
			render_sphere();
		else if(m_pickup_2->active() == false && m_pickup_2->collected() == false)
			render_sphere();
		else if (m_pickup_3->active() == false && m_pickup_3->collected() == false)
			render_sphere();

		if (m_pickup_4->active() == false && m_pickup_4->collected() == false)
			m_spotLight.AmbientIntensity = 0.95f;
		else if (m_pickup_5->active() == false && m_pickup_5->collected() == false)
				m_spotLight.AmbientIntensity = 0.85f;

		if (m_alpha_sphere->get_s_active() == true)
			player.set_invincible(true);
		else
			player.set_invincible(false);

		m_traffic1.set_player_position(m_player->position());
		m_traffic1.on_update(time_step);
		//m_traffic2.on_update(time_step);

		//Code to detect collisions
		if (m_player->is_colliding())
		{
			if (m_player->collision_objects().size() > 1 && m_bus->collision_objects().size() > 1 && timer == 0) {
				if (m_alpha_sphere->get_s_active() == false) {
					decrease_health(20);
					m_cross_fade->activate();

					m_audio_manager->play("horn");
					timer = 60 * 4;
				}
			}
		}

		//loop array of AI and check if any have crashed 
		for (int i = 0; i < car_array.size(); i++) {
			car_array.at(i)->on_update(time_step);
			if (car_array.at(i)->get_state() == "Crashed") {
				//implement score and explosion code here
				car_array.at(i)->set_state(destroyed);
				//score = score + 5;
				if (car_array.at(i)->get_type() == 1)
					player.set_score(player.get_score() + 5);
				else
					player.set_score(player.get_score() - 5);

				if (player.get_score() <= 0)
					player.set_score(0);

				m_cross_fade->activate();
				m_audio_manager->play("crash");
				m_billboard->activate(car_array.at(i)->object()->position(), 4.f, 4.f);

				//m_traffic2.set_state("Destroyed");
			}
		}

		//check if player has collided and decrease health 
		if (m_player->is_colliding())
		{
			if (m_player->collision_objects().size() > 1 && timer == 0) {
				if (m_alpha_sphere->get_s_active() == false) {
					decrease_health(5);
					//m_audio_manager->play("crash");
					//m_audio_manager->play("horn");
					timer = 60 * 1;
				}
			}
		}

		if (tick == 60) {
			tick = 0;
			game_timer = game_timer - 1;
		}

		if (game_timer == 0 && game_over == false) {
			f_end_game();
		}
	}
		if (timer != 0)
		timer = timer - 1;
	//update strings for HUD
	healthstr = std::to_string(player.get_health());
	scorestr = std::to_string(player.get_score());
	timerstr = std::to_string(game_timer);


}
 
void example_layer::on_render()
{
	engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
	engine::render_command::clear();

	//const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
	//engine::renderer::begin_scene(m_3d_camera, textured_shader);


	const auto textured_lighting_shader = engine::renderer::shaders_library()->get("mesh_lighting");
	engine::renderer::begin_scene(m_3d_camera, textured_lighting_shader);

	// Set up some of the scene's parameters in the shader
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("fog_on", m_fog);
	// Position the skybox centred on the player and render it
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(textured_lighting_shader, m_skybox, skybox_tranform);
	std::dynamic_pointer_cast<engine::gl_shader>(textured_lighting_shader)->set_uniform("gNumSpotLights", (int)num_spot_lights);
	m_spotLight.submit(textured_lighting_shader, 0);

	engine::renderer::submit(textured_lighting_shader, m_terrain);

	//m_player_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);
	
	//m_bus_box.on_render(2.5f, 0.f, 0.f, textured_lighting_shader);

	//build wall of trees on left and right 
	for (int i = 0; i < 190; ++i) {
		glm::mat4 tree_transform2(1.0f);
		tree_transform2 = glm::translate(tree_transform2, glm::vec3(-4.f, 1.5f, 15.0f - (i * 2)));
		tree_transform2 = glm::rotate(tree_transform2, 55.f, glm::vec3(1, 0, 0));
		tree_transform2 = glm::scale(tree_transform2, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform2, m_tree);
	}

	for (int i = 0; i < 190; ++i) {
		glm::mat4 tree_transform2(1.0f);
		tree_transform2 = glm::translate(tree_transform2, glm::vec3(4.f, 1.5f, 15.0f - (i * 2)));
		tree_transform2 = glm::rotate(tree_transform2, 55.f, glm::vec3(1, 0, 0));
		tree_transform2 = glm::scale(tree_transform2, m_tree->scale());
		engine::renderer::submit(textured_lighting_shader, tree_transform2, m_tree);
	}


	//render body of 3 houses
	glm::mat4 house_transform(1.0f);
	house_transform = glm::translate(house_transform, m_house->position());
	house_transform = glm::rotate(house_transform, 0.785398f, glm::vec3(0, 1, 0));
	house_transform = glm::scale(house_transform, m_house->scale() * 2.f);
	engine::renderer::submit(textured_lighting_shader, house_transform, m_house);

	glm::mat4 house_transform2(1.0f);
	house_transform2 = glm::translate(house_transform2, glm::vec3(8.f, 1.8f, -30.f));
	house_transform2 = glm::rotate(house_transform2, 0.785398f, glm::vec3(0, 1, 0));
	house_transform2 = glm::scale(house_transform2, m_house->scale() * 2.f);
	engine::renderer::submit(textured_lighting_shader, house_transform2, m_house);

	glm::mat4 house_transform3(1.0f);
	house_transform3 = glm::translate(house_transform3, glm::vec3(-8.f, 1.8f, -60.f));
	house_transform3 = glm::rotate(house_transform3, 0.785398f, glm::vec3(0, 1, 0));
	house_transform3 = glm::scale(house_transform3, m_house->scale() * 2.f);
	engine::renderer::submit(textured_lighting_shader, house_transform3, m_house);

	glm::mat4 office_transform(1.0f);
	office_transform = glm::translate(office_transform, m_office->position());
	office_transform = glm::rotate(office_transform, m_office->rotation_amount(), m_office->rotation_axis());
	office_transform = glm::scale(office_transform, m_house->scale() * 2.f);
	engine::renderer::submit(textured_lighting_shader, office_transform, m_office);

	for (float i = 0; i <= 2; i = i + 2) {
		for (float j = 0; j <= 350; j = j + 2) {
			glm::mat4 office_transform(1.0f);
			office_transform = glm::translate(office_transform, glm::vec3(6 + i,1.8 , (-720 + j)));
			office_transform = glm::rotate(office_transform, m_office->rotation_amount(), m_office->rotation_axis());
			office_transform = glm::scale(office_transform, m_house->scale() * 2.f);
			engine::renderer::submit(textured_lighting_shader, office_transform, m_office);
		}
	}

	for (float i = 0; i <= 2; i = i + 2) {
		for (float j = 0; j <= 350; j = j + 2) {
			glm::mat4 office_transform(1.0f);
			office_transform = glm::translate(office_transform, glm::vec3(-6 - i, 1.8, (-720 + j)));
			office_transform = glm::rotate(office_transform, m_office->rotation_amount(), m_office->rotation_axis());
			office_transform = glm::scale(office_transform, m_house->scale() * 2.f);
			engine::renderer::submit(textured_lighting_shader, office_transform, m_office);
		}
	}

	//render steering AI
	glm::vec3 p = m_bus->position();
	glm::vec3 c = m_3d_camera.position();
	glm::vec3 v = c - p;
	glm::mat4 bus_transform(1.0f);
	float theta = atan2(v.x, v.z);
	bus_transform = glm::translate(bus_transform, m_bus->position() - m_bus->offset() *
		m_bus->scale());
	
	bus_transform = glm::rotate(bus_transform, 1.5708f *2.f, glm::vec3(0, 1, 0));
	bus_transform = glm::rotate(bus_transform, theta, glm::vec3(0, 1, 0));
	bus_transform = glm::scale(bus_transform, m_bus->scale() *2.f );
	engine::renderer::submit(textured_lighting_shader, bus_transform, m_bus);



	//render first health pickup
	if (m_health->active()) {
		glm::mat4 health_transform(1.0f);
		health_transform = glm::translate(health_transform, m_health->position());
		health_transform = glm::rotate(health_transform, m_health->rotation_amount(), m_health->rotation_axis());
		health_transform = glm::scale(health_transform, m_health->scale() / 2.f);
		engine::renderer::submit(textured_lighting_shader, health_transform, m_health);
	}

	
	//render player model 
	glm::mat4 player_transform(1.0f);
	player_transform = glm::translate(player_transform, m_player->position() - m_player->offset() *
		m_player -> scale());
	player_transform = glm::rotate(player_transform, m_player->rotation_amount(), m_player->rotation_axis());
	player_transform = glm::rotate(player_transform, 1.5708f * 2, glm::vec3(0, 1, 0));
	player_transform = glm::scale(player_transform, m_player->scale());
	engine::renderer::submit(textured_lighting_shader, player_transform, m_player);

	/*The below code renders the various AI into the game world and scaling and rotating as needed*/
	glm::mat4 ai_car_transform(1.0f);
	ai_car_transform = glm::translate(ai_car_transform, m_ai_car1->position() - m_ai_car1->offset() *
		m_ai_car1->scale());
	ai_car_transform = glm::rotate(ai_car_transform, -1.5708f, glm::vec3(1, 0, 0));
	ai_car_transform = glm::rotate(ai_car_transform, m_ai_car1->rotation_amount(), m_ai_car1->rotation_axis());
	ai_car_transform = glm::rotate(ai_car_transform, 1.5708f*2, glm::vec3(0, 0, 1));
	ai_car_transform = glm::scale(ai_car_transform, m_ai_car1->scale()*2.f);
	engine::renderer::submit(textured_lighting_shader, ai_car_transform, m_ai_car1);

	glm::mat4 ai_car_transform2(1.0f);
	ai_car_transform2 = glm::translate(ai_car_transform2, m_ai_car1_2->position() );
	ai_car_transform2 = glm::rotate(ai_car_transform2, m_ai_car1_2->rotation_amount(), m_ai_car1_2->rotation_axis());
	ai_car_transform2 = glm::rotate(ai_car_transform2, -1.5708f, glm::vec3(1, 0, 0));
	ai_car_transform2 = glm::rotate(ai_car_transform2, 1.5708f * 2, glm::vec3(0, 0, 1));
	ai_car_transform2 = glm::scale(ai_car_transform2, m_ai_car1_2->scale()*2.f);
	engine::renderer::submit(textured_lighting_shader, ai_car_transform2, m_ai_car1_2);

	glm::mat4 ai_car_transform3(1.0f);
	ai_car_transform3 = glm::translate(ai_car_transform3, m_ai_car1_3->position());
	ai_car_transform3 = glm::rotate(ai_car_transform3, m_ai_car1_3->rotation_amount(), m_ai_car1_3->rotation_axis());
	ai_car_transform3 = glm::scale(ai_car_transform3, m_ai_car1_3->scale() * 2.f);
	engine::renderer::submit(textured_lighting_shader, ai_car_transform3, m_ai_car1_3);


	glm::mat4 ai_car_transform4(1.0f);
	ai_car_transform4 = glm::translate(ai_car_transform4, m_ai_car1_4->position() );
	ai_car_transform4 = glm::rotate(ai_car_transform4, -1.5708f, glm::vec3(1, 0, 0));
	ai_car_transform4 = glm::rotate(ai_car_transform4, m_ai_car1_4->rotation_amount(), m_ai_car1_4->rotation_axis());
	ai_car_transform4 = glm::rotate(ai_car_transform4, 1.5708f * 2, glm::vec3(0, 0, 1));
	ai_car_transform4 = glm::scale(ai_car_transform4, m_ai_car1_4->scale() * 2.f);
	engine::renderer::submit(textured_lighting_shader, ai_car_transform4, m_ai_car1_4);

	glm::mat4 ai_car_transform5(1.0f);
	ai_car_transform5 = glm::translate(ai_car_transform5, m_ai_car1_5->position());
	ai_car_transform5 = glm::rotate(ai_car_transform5, -1.5708f, glm::vec3(1, 0, 0));
	ai_car_transform5 = glm::rotate(ai_car_transform5, m_ai_car1_5->rotation_amount(), m_ai_car1_5->rotation_axis());
	ai_car_transform5 = glm::rotate(ai_car_transform5, 1.5708f * 2, glm::vec3(0, 0, 1));
	ai_car_transform5 = glm::scale(ai_car_transform5, m_ai_car1_5->scale() * 2.f);
	engine::renderer::submit(textured_lighting_shader, ai_car_transform5, m_ai_car1_5);

	glm::mat4 ai_car_transform6(1.0f);
	ai_car_transform6 = glm::translate(ai_car_transform6, m_ai_car1_6->position());
	ai_car_transform6 = glm::rotate(ai_car_transform6, -1.5708f, glm::vec3(1, 0, 0));
	ai_car_transform6 = glm::rotate(ai_car_transform6, m_ai_car1_6->rotation_amount(), m_ai_car1_6->rotation_axis());
	ai_car_transform6 = glm::rotate(ai_car_transform6, 1.5708f * 2, glm::vec3(0, 0, 1));
	ai_car_transform6 = glm::scale(ai_car_transform6, m_ai_car1_6->scale() * 2.f);
	engine::renderer::submit(textured_lighting_shader, ai_car_transform6, m_ai_car1_6);

	glm::mat4 ai_car_transform7(1.0f);
	ai_car_transform7 = glm::translate(ai_car_transform7, m_ai_car1_7->position());
	ai_car_transform7 = glm::rotate(ai_car_transform7, m_ai_car1_7->rotation_amount(), m_ai_car1_7->rotation_axis());
	ai_car_transform7 = glm::scale(ai_car_transform7, m_ai_car1_7->scale() * 2.f);
	engine::renderer::submit(textured_lighting_shader, ai_car_transform7, m_ai_car1_7);
	

	/*the below code renders the pickup items in the game area*/
	if (m_pickup->active() == true) {
		m_pickup->textures().at(0)->bind();
		glm::mat4 pickup_transform(1.0f);
		pickup_transform = glm::translate(pickup_transform, m_pickup->position());
		pickup_transform = glm::rotate(pickup_transform, -1.5708f, glm::vec3(1, 0, 0));
		pickup_transform = glm::rotate(pickup_transform, 1.5708f, glm::vec3(1, 1, 0));
		pickup_transform = glm::rotate(pickup_transform, m_pickup->rotation_amount(), glm::vec3(0, 1, 0));
		engine::renderer::submit(textured_lighting_shader, m_pickup->meshes().at(0), pickup_transform);
	}

	if (m_pickup_2->active() == true) {
		m_pickup_2->textures().at(0)->bind();
		glm::mat4 pickup_transform2(1.0f);
		pickup_transform2 = glm::translate(pickup_transform2, m_pickup_2->position());
		pickup_transform2 = glm::rotate(pickup_transform2, -1.5708f, glm::vec3(1, 0, 0));
		pickup_transform2 = glm::rotate(pickup_transform2, 1.5708f, glm::vec3(1, 1, 0));
		pickup_transform2 = glm::rotate(pickup_transform2, m_pickup_2->rotation_amount(), glm::vec3(0, 1, 0));
		engine::renderer::submit(textured_lighting_shader, m_pickup_2->meshes().at(0), pickup_transform2);
	}

	

	if (m_pickup_3->active() == true) {
		m_pickup_3->textures().at(0)->bind();
		glm::mat4 pickup_transform3(1.0f);
		pickup_transform3 = glm::translate(pickup_transform3, m_pickup_3->position());
		pickup_transform3 = glm::rotate(pickup_transform3, -1.5708f, glm::vec3(1, 0, 0));
		pickup_transform3 = glm::rotate(pickup_transform3, 1.5708f, glm::vec3(1, 1, 0));
		pickup_transform3 = glm::rotate(pickup_transform3, m_pickup_3->rotation_amount(), glm::vec3(0, 1, 0));
		engine::renderer::submit(textured_lighting_shader, m_pickup_3->meshes().at(0), pickup_transform3);
	}

	if (m_pickup_4->active() == true) {
		m_pickup_4->textures().at(0)->bind();
		glm::mat4 pickup_transform4(1.0f);
		pickup_transform4 = glm::translate(pickup_transform4, m_pickup_4->position());
		pickup_transform4 = glm::rotate(pickup_transform4, -1.5708f, glm::vec3(1, 0, 0));
		pickup_transform4 = glm::rotate(pickup_transform4, 1.5708f, glm::vec3(1, 1, 0));
		pickup_transform4 = glm::rotate(pickup_transform4, m_pickup_4->rotation_amount(), glm::vec3(0, 1, 0));
		engine::renderer::submit(textured_lighting_shader, m_pickup_4->meshes().at(0), pickup_transform4);
	}

	if (m_pickup_5->active() == true) {
		m_pickup_5->textures().at(0)->bind();
		glm::mat4 pickup_transform5(1.0f);
		pickup_transform5 = glm::translate(pickup_transform5, m_pickup_5->position());
		pickup_transform5 = glm::rotate(pickup_transform5, -1.5708f, glm::vec3(1, 0, 0));
		pickup_transform5 = glm::rotate(pickup_transform5, 1.5708f, glm::vec3(1, 1, 0));
		pickup_transform5 = glm::rotate(pickup_transform5, m_pickup_5->rotation_amount(), glm::vec3(0, 1, 0));
		engine::renderer::submit(textured_lighting_shader, m_pickup_5->meshes().at(0), pickup_transform5);
	}


	
	//render roof pieces for houses
	m_roof1->textures().at(0)->bind();
	glm::mat4 roof1(1.0f);
	roof1 = glm::translate(roof1, glm::vec3(-1.5f, 3.3, -800));
	roof1 = glm::rotate(roof1, -1.5708f, glm::vec3(0, 1, 0));
	engine::renderer::submit(textured_lighting_shader, roof1, m_roof1);

	glm::mat4 roof2(1.0f);
	roof2 = glm::translate(roof2, glm::vec3(1.5f, 3.3, -800));
	roof2 = glm::rotate(roof2, -1.5708f * 3.f, glm::vec3(0, 1, 0));
	engine::renderer::submit(textured_lighting_shader, roof2, m_roof1);

	glm::mat4 roof3(1.0f);
	roof3 = glm::translate(roof3, glm::vec3(6.5f, 3.3, -30));
	roof3 = glm::rotate(roof3, -1.5708f, glm::vec3(0, 1, 0));
	engine::renderer::submit(textured_lighting_shader, roof3, m_roof1);

	glm::mat4 roof4(1.0f);
	roof4 = glm::translate(roof4, glm::vec3(9.5f, 3.3, -30));
	roof4 = glm::rotate(roof4, -1.5708f * 3.f, glm::vec3(0, 1, 0));
	engine::renderer::submit(textured_lighting_shader, roof4, m_roof1);

	glm::mat4 roof5(1.0f);
	roof5 = glm::translate(roof5, glm::vec3(-6.5f, 3.3, -60));
	roof5 = glm::rotate(roof5, -1.5708f * 3.f, glm::vec3(0, 1, 0));
	engine::renderer::submit(textured_lighting_shader, roof5, m_roof1);

	glm::mat4 roof6(1.0f);
	roof6 = glm::translate(roof6, glm::vec3(-9.5f, 3.3, -60));
	roof6 = glm::rotate(roof6, -1.5708f, glm::vec3(0, 1, 0));
	engine::renderer::submit(textured_lighting_shader, roof6, m_roof1);

	m_billboard->on_render(m_3d_camera, textured_lighting_shader);
	

	engine::renderer::end_scene();

	// Set up material shader. (does not render textures, renders materials instead)
	const auto textured_material_shader = engine::renderer::shaders_library()->get("mesh_material");
	engine::renderer::begin_scene(m_3d_camera, textured_material_shader);
	m_spotLight.submit(textured_material_shader, 0);
	std::dynamic_pointer_cast<engine::gl_shader>(textured_material_shader)->set_uniform("fog_on", m_fog);
	m_material->submit(textured_material_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(textured_material_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());
	m_alpha_sphere->on_render(textured_material_shader);

	//engine::renderer::submit(textured_material_shader, m_ball);

	//tetrahedron_ = glm::translate(m_tetrahedron, glm::vec3(0.0f, 20.0f, -20.0f));

	//m_tetrahedron_material ->submit(textured_material_shader);


	engine::renderer::end_scene();

	const auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");
	engine::renderer::begin_scene(m_3d_camera, animated_mesh_shader);
	std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gEyeWorldPos", m_3d_camera.position());

	glm::mat4 aniTransform = glm::mat4(1.0f);

	glm::vec3 anim_scale = glm::vec3(0.002f);
	glm::vec3 anim_position = glm::vec3(3.0f, 0.5f, -5.0f);
	float frame_time = m_anim_timer / (float)m_skinned_mesh->animations().at(1)->mDuration;

	aniTransform = glm::translate(aniTransform, (anim_position - frame_time * m_anim_displacement * anim_scale));
	aniTransform = glm::scale(aniTransform, anim_scale);
	m_spotLight.submit(animated_mesh_shader, 0);
	//m_skinned_mesh->on_render(aniTransform, animated_mesh_shader);

	engine::renderer::end_scene();

	// Render text 
	engine::renderer::begin_scene(m_2d_camera, textured_lighting_shader);

	//redering for crossfade and end game screen 
	m_cross_fade->on_render(textured_lighting_shader);
	m_end_game->on_render(textured_lighting_shader);

	engine::renderer::end_scene();
	//render score and timer text during gameplay
	const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	if (game_over == false) {
		m_text_manager->render_text(text_shader, "Health: " + healthstr, 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Score: " + scorestr, 145.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Time Remaining: " + timerstr, 255.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));
	}//show final score when game ends
	else if (game_over == true){
		m_text_manager->render_text(text_shader, "Game Over " , 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));
		m_text_manager->render_text(text_shader, "Score: " + final_score_str, 145.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(1.f, 1.f, 1.f, 1.f));
		
	}

	//remove intro text on start of the game 
	if (moveCamera == false) {
		//render intro screen text
		m_text_manager->render_text(text_shader, "Police Chase ", ((float)engine::application::window().width() / 2.f) - 90, (float)engine::application::window().height() - 150.f, .6f, glm::vec4(0.f, 0.f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Ram as many red cars off the road as possible before time runs out", ((float)engine::application::window().width() / 2.f) - 420, (float)engine::application::window().height() - 200.f, .6f, glm::vec4(0.f, 0.f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Controls", ((float)engine::application::window().width() / 2.f) - 65, (float)engine::application::window().height() - 260.f, .6f, glm::vec4(0.f, 0.f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Steer Left: A", ((float)engine::application::window().width() / 2.f) - 90, (float)engine::application::window().height() - 290.f, .6f, glm::vec4(0.f, 0.f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Steer Right: D", ((float)engine::application::window().width() / 2.f) - 90, (float)engine::application::window().height() - 320.f, .6f, glm::vec4(0.f, 0.f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Difficulty: W", ((float)engine::application::window().width() / 2.f) - 80, (float)engine::application::window().height() - 350.f, .6f, glm::vec4(0.f, 0.0f, 0.f, 1.f));
		m_text_manager->render_text(text_shader, "Press SPACE to start", ((float)engine::application::window().width() / 2.f) - 120, (float)engine::application::window().height() - 600.f, .6f, glm::vec4(0.f, 0.f, 0.f, 1.f));
	}
	

	engine::renderer::end_scene();
}

//key events include starting game and toggling fog 
void example_layer::on_event(engine::event& event)
{
	if (event.event_type() == engine::event_type_e::key_pressed)
	{
		auto& e = dynamic_cast<engine::key_pressed_event&>(event);
		if (e.key_code() == engine::key_codes::KEY_TAB)
		{
			engine::render_command::toggle_wireframe();
		}
		//start game on press of spacebar
		else if (e.key_code() == engine::key_codes::KEY_SPACE) {
			if (moveCamera != true) {
				moveCamera = true;
				start_game();
			}
		}
		else if (e.key_code() == engine::key_codes::KEY_W) {
			
			m_fog = !m_fog;
		}
		else if (e.key_code() == engine::key_codes::KEY_3) {
			m_alpha_sphere->activate(0.5f, glm::vec3(5.f, 0.5f, -7.5f));
		}
		else if (e.key_code() == engine::key_codes::KEY_P) {
			f_end_game();
		}
	}
}




//generate x and z co ordinates for items
float example_layer::generate_random(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));

}

//method to update the position of the point light to follow the player charecter
void example_layer::update_headlight() {
	m_spotLight.Position = m_player->position();
	m_spotLight.Position.z = m_player->position().z - 2.f;
}

//method called every frame to update camera position relative to player 
void example_layer::update_camera() {
	glm::vec3 camera_pos = m_player->position();
	camera_pos.y = camera_pos.y + 1.5f;
	camera_pos.z = camera_pos.z + 5.f;
	m_3d_camera.position(camera_pos);
	
}

void example_layer::increase_health() {
	player.set_health(player.get_health() + 10);
	update_screen_overlay();
}

void example_layer::decrease_health(int dec) {
	player.set_health(player.get_health() - dec);
}

void example_layer::update_screen_overlay() {
	/*const auto text_shader = engine::renderer::shaders_library()->get("text_2D");
	m_text_manager->render_text(text_shader, "Health: " + healthstr, 10.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));
	m_text_manager->render_text(text_shader, "Score: " + scorestr, 5.f, (float)engine::application::window().height() - 25.f, 0.5f, glm::vec4(0.f, 0.f, 0.f, 1.f));
	*/
}

void example_layer::render_sphere() {
	m_alpha_sphere->activate(0.5f, glm::vec3(5.f, 0.5f, -7.5f));
	m_audio_manager->play("pickup");
}

void ::example_layer::f_end_game() {
	
	final_score = score;
	final_score_str = std::to_string(player.get_score());
	player.set_game_end(true);
	game_over = true;
	m_audio_manager->stop_all();
	m_end_game->activate();

}

//method called to start game 
void example_layer::start_game()
{
	//initalise players and AI 
	player.initialise(m_player);
	m_traffic1.initialise(m_bus);
	m_traffic2->initialise(m_ai_car1, 1, 1);
	m_traffic2_2->initialise(m_ai_car1_2, 1, 1);
	m_traffic2_3->initialise(m_ai_car1_3, 1, 2);
	m_traffic2_4->initialise(m_ai_car1_4, 1, 1);
	m_traffic2_5->initialise(m_ai_car1_5, 1, 1);
	m_traffic2_6->initialise(m_ai_car1_6, 1, 1);
	m_traffic2_7->initialise(m_ai_car1_7, 1, 2);
	//m_traffic2_5->initialise(m_ai_car1_8, 1, 1);

	//add AI to array and 
	car_array.push_back(m_traffic2);
	car_array.push_back(m_traffic2_2);
	car_array.push_back(m_traffic2_3);
	car_array.push_back(m_traffic2_4);
	car_array.push_back(m_traffic2_5);
	car_array.push_back(m_traffic2_6);
	car_array.push_back(m_traffic2_7);

	//push objects into stack for bullet 
	m_game_objects.push_back(m_terrain);
	m_game_objects.push_back(m_player);
	//m_game_objects.push_back(m_ai_car2);
	m_game_objects.push_back(m_bus);
	m_game_objects.push_back(m_ai_car1);
	m_game_objects.push_back(m_ai_car1_2);
	m_game_objects.push_back(m_ai_car1_3);
	m_game_objects.push_back(m_ai_car1_4);
	m_game_objects.push_back(m_ai_car1_5);
	m_game_objects.push_back(m_ai_car1_6);
	m_game_objects.push_back(m_ai_car1_7);

	m_physics_manager = engine::bullet_manager::create(m_game_objects);

	//play music 
	m_audio_manager->play("music");
	//set started to true 
	started = true;

}

void example_layer::decrease_health() {
	if (m_alpha_sphere->get_s_active() == false) {
		player.set_health(player.get_health() - 5);
	}
}
