#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <iostream>
#include <vector>
#include <string>
#include <strstream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

/*
 * This is a simple parse for OBJ files written from scratch.
 * He is not perfect but he can open without problems some basic files.
*/

class OBJLoader	{
	
	public:

		/**
		 * It works as follow
		 * 1) Instanciate the class
		 * 2) retrieve the data using parseObjFile
		 */

		OBJLoader(string path_in);
		vector<float> parseObjFile();

	private:
		// A small helper function to split the entry
		vector<string> split(string input);
		// The current path of the file (nothing more)
		string path;
};

#endif