#include "OBJLoader.h"

OBJLoader::OBJLoader(string path_in)
{
	path = path_in;
}

vector<float> OBJLoader::parseObjFile()
{
	string line;
	ifstream objFile;

	vector<float> data = vector<float>();

	objFile.open(path);

	vector<glm::vec3> vertices_in(0);
	vector<glm::vec3> normals_in(0);
	vector<glm::vec2> tCoords_in(0);

	while (getline(objFile, line))
	{
		// Case v
		if (line.substr(0, 2) == "v ")
		{
			char junk;
			float x, y, z;
			strstream s;
			s << line;
			s >> junk >> x >> y >> z;
			vertices_in.push_back(glm::vec3(x, y, z));
		}
		// Case vt
		else if (line.substr(0, 2) == "vt")
		{
			char junk;
			float x, y, z;
			stringstream splitter;
			splitter << line.substr(3, line.size() - 3);
			string n;
			getline(splitter, n, ' ');
			x = stof(n);
			getline(splitter, n, ' ');
			y = stof(n);
			tCoords_in.push_back(glm::vec2(x, y));
		}
		// Case normal
		else if (line.substr(0, 2) == "vn")
		{
			char junk;
			float x, y, z;
			stringstream splitter;
			splitter << line.substr(3, line.size() - 3);
			string n;
			getline(splitter, n, ' ');
			x = stof(n);
			getline(splitter, n, ' ');
			y = stof(n);
			getline(splitter, n, ' ');
			z = stof(n);
			normals_in.push_back(glm::vec3(x, y, z));
		}
		// Case f
		else if (line.substr(0, 1) == "f")
		{
			vector<string> faces = split(line);
			for (int i = 0; i < faces.size() - 2;i++)
			{
				for (int j = 0; j < 3;j++)
				{
					int array_index = i + j;
					string v = faces[array_index];
					stringstream last;
					last << v;
					getline(last, v, '/');
					int index = (stoi(v) - 1); // vertex Index
					getline(last, v, '/');
					int index3 = (stoi(v) - 1); // texture Index
					getline(last, v, '/');
					int index2 = (stoi(v) - 1); // normal Index
					data.push_back(vertices_in[index].x);
					data.push_back(vertices_in[index].y);
					data.push_back(vertices_in[index].z);
					data.push_back(normals_in[index2].x);
					data.push_back(normals_in[index2].y);
					data.push_back(normals_in[index2].z);
					data.push_back(tCoords_in[index3].x);
					data.push_back(1-tCoords_in[index3].y);
				}	
			}
		}
	}

	return data;
}

vector<string> OBJLoader::split(string input)
{
	vector<string> output;

	string in = input.substr(2, input.size() - 2);
	string current = "";

	for (int i = 0; i < in.size();i++)
	{
		if (in[i] == ' ' && current.size() > 0)
		{
			output.push_back(current);
			current = "";
		}

		if (in[i] != ' ')
		{
			current.push_back(in[i]);
		}
	}

	if (current.size() > 0)
	{
		output.push_back(current);
	}

	return output;
}
