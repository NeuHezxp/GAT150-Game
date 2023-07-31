#include "Model.h"
#include <sstream>
#include <iostream>

namespace kiko
{
	bool Model::Load(const std::string filename)
	{
		std::string buffer;
		kiko::readFile(filename, buffer);

		std::cout << buffer << std::endl;

		std::istringstream stream(buffer);
		//read color
		stream >> m_color;

		//read number points
		std::string line;
		std::getline(stream, line);
		int numPoints = std::stoi(line);

		//read vector2 points
		for (int i = 0; i < numPoints; ++i)
		{
			vec2 point;
			stream >> point;
			m_points.push_back(point);
		}

		return true;
	}

	void Model::Draw(Renderer& renderer, const vec2& position, float rotation, float scale)
	{
		if (m_points.empty()) return;

		renderer.setColor(Color::toInt(m_color.r), Color::toInt(m_color.g), Color::toInt(m_color.b), Color::toInt(m_color.a));
		for (int i = 0; i < m_points.size() - 1; ++i)
		{
			vec2 p1 = (m_points[i] * scale).Rotate(rotation) + position;
			vec2 p2 = (m_points[i + 1] * scale).Rotate(rotation) + position;

			renderer.DrawLine(p1.x, p1.y, p2.x, p2.y);
		}
	}

	void Model::Draw(Renderer& renderer, const Transform& transform) /// !! same as ^ but with different Parameters
	{
		Draw(renderer, transform.position, transform.rotation, transform.scale);
	}

	float Model::GetRadius()//creating a circle around model for collision
	{
		if (m_radius) return m_radius; //if radius is != 0 return the radius
		
		for (auto point : m_points)
		{
			float length = point.length();
 			m_radius = Max(m_radius, length); // raidus = radius or length
		}

		return m_radius;
	}
}