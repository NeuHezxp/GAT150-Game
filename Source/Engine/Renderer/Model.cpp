#include "Model.h"
#include <sstream>
#include <iostream>
#include "Renderer.h"

namespace kiko
{
	bool Model::Create(std::string filename, ...)
	{
		return Load(filename);
	}

	// This function loads the model data from the specified file.
	// It reads the color, number of points, and vector2 points from the file and populates the corresponding member variables.
	bool Model::Load(const std::string filename)
	{
		std::string buffer;
		kiko::readFile(filename, buffer);

		std::cout << buffer << std::endl;

		std::istringstream stream(buffer);
		// Read color ( in the format "r g b a")
		stream >> m_color;

		// Read number of points from the next line
		std::string line;
		std::getline(stream, line);
		int numPoints = std::stoi(line);

		// Read vector2 points from the file and store them in m_points
		for (int i = 0; i < numPoints; ++i)
		{
			vec2 point;
			stream >> point;
			m_points.push_back(point);
		}

		return true;
	}

	// This function draws the model using the provided renderer at the specified position, rotation, and scale.
	// It loops through the points of the model, applies the rotation and scale transformations, and draws lines between consecutive points.
	void Model::Draw(Renderer& renderer, const vec2& position, float rotation, float scale)
	{
		if (m_points.empty()) return;

		// Set the renderer color based on the model's color
		renderer.setColor(Color::toInt(m_color.r), Color::toInt(m_color.g), Color::toInt(m_color.b), Color::toInt(m_color.a));

		// Draw lines between consecutive points after applying rotation and scale transformations
		for (int i = 0; i < m_points.size() - 1; ++i)
		{
			vec2 p1 = (m_points[i] * scale).Rotate(rotation) + position;
			vec2 p2 = (m_points[i + 1] * scale).Rotate(rotation) + position;

			renderer.DrawLine(p1.x, p1.y, p2.x, p2.y);
		}
	}
	void Model::Draw(Renderer& renderer, const Transform& transform)
	{
		//pasted the one from above
		if (m_points.empty()) return;

		mat3 mx = transform.GetMatrix();

		renderer.setColor(Color::toInt(m_color.r), Color::toInt(m_color.g), Color::toInt(m_color.b), Color::toInt(m_color.a));
		for (int i = 0; i < m_points.size() - 1; i++)
		{
			vec2 p1 = (mx * m_points[i] + transform.position);
			vec2 p2 = (mx * m_points[i + 1] + transform.position);

			renderer.DrawLine(p1.x, p1.y, p2.x, p2.y);
		}
	}

	// This function is an overloaded version of the Draw function that takes a Transform object as a parameter.
	// It calls the other Draw function, passing the transform's position, rotation, and scale as arguments.
	/*void Model::Draw(Renderer& renderer, const Transform& transform)
	{
		Draw(renderer, transform.position, transform.rotation, transform.scale);
	}*/

	// This function calculates and returns the radius of the model.
	// It finds the maximum length of all points in the model and uses it as the radius.
	// This can be useful for collision detection purposes, creating a circle around the model.
	float Model::GetRadius()
	{
		if (m_radius) return m_radius; // If the radius is already calculated, return it.

		// Calculate the maximum length among all points in the model
		for (auto point : m_points)
		{
			float length = point.length();
			m_radius = Max(m_radius, length); // Set the radius as the maximum value between m_radius and the length of the current point.
		}

		return m_radius;
	}
}