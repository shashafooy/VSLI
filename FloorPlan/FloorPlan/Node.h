#pragma once
#include <string>
#include <utility>
#include <vector>
#include <cmath>


using namespace std;
struct Node
{
	Node(const char name, const double area, const double aspectRatio)
		: name(name), area(area)
	{
		GenerateDims(aspectRatio);
	}

	explicit Node(const char name) : name(name), area(0) { }

	Node() : name(' '), area(0)
	{ }

	~Node()
	{
		dims.clear();
	}


	

	struct Dim
	{
		double width;
		double height;

		friend bool operator==(const Dim& lhs, const Dim& rhs)
		{
			return lhs.width == rhs.width && lhs.height == rhs.height;
		}
	};

	void GenerateDims(const double aspectRatio)
	{
		Dim newDim;  // NOLINT(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
		newDim.width = sqrt(area / aspectRatio);
		newDim.height = sqrt(area*aspectRatio);
		dims.push_back(newDim);
		swap(newDim.height, newDim.width);
		dims.push_back(newDim);
	}


	char name;
	double area;
	vector<Dim> dims;


#pragma region Overloads
	friend bool operator==(const char& value, const Node& rhs)
	{
		return value == rhs.name;
	}

	friend bool operator==(const Node& rhs, const char& value)
	{
		return value == rhs.name;
	}

	friend bool operator!=(const char& value, const Node& rhs)
	{
		return value != rhs.name;
	}

	friend bool operator!=(const Node& rhs, const char& value)
	{
		return value != rhs.name;
	}


	friend bool operator<(const char& lhs, const Node& rhs)
	{
		return lhs < rhs.name;
	}

	friend bool operator<(const Node& lhs, const char& rhs)
	{
		return lhs.name < rhs;
	}

	friend bool operator<(const Node& lhs, const Node& rhs)
	{
		return lhs.name < rhs.name;
	}

#pragma endregion
};

