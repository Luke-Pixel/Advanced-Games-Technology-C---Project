#pragma once

namespace engine
{
	class mesh;

	//brief class creating a tetrahedron object with a mesh of a specific size
	class tetrahedron
	{
	public:
		//brief constructor
		tetrahedron(std::vector<glm::vec3> vertices);

		//brief destructor
		~tetrahedron();

		//brief getter and seter methods
		std::vector<glm::vec3> vertices() const { return m_vertices; }
		ref<engine::mesh> mesh() const { return m_mesh; }



		static ref<tetrahedron> create(std::vector<glm::vec3> vertices);
	private:
		//brief fields
		//vertices of tetrahedron specified by vector positions
		std::vector<glm::vec3> m_vertices;

		ref<engine::mesh> m_mesh;
	};
}

