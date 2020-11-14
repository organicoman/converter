#pragma once
#ifndef DESERIALIZER_H
#define DESERIALIZER_H
//---------STL---------------
#include <string>  // std::string
#include <utility> // std::pair
#include <vector>  // std::vector
#include <thread>  // std::thread
//---------------------------
#include "Namespace.h"

class conv::Deserializer
{
	json m_jsonFile;
	std::thread m_workerThread;
	std::pair<std::string, std::vector<std::string>> m_vertexPattern;
	std::pair<std::string, std::vector<std::string>> m_normalPattern;
	std::pair<std::string, std::vector<std::string>> m_colorPattern;
	std::pair<std::string, std::vector<std::string>> m_facePattern;
	std::pair<std::string, std::vector<std::string>> m_textCoordPattern;

public:
	explicit Deserializer(const std::string& jsonFile);
	Deserializer(const json& jsonFile);

	/*
	 * opens a file, then parse it according to the JSON fileformat provided in the
	 * consturctor, if any tag is not parsed the it is returned as a sting to be handled 
	 * in its derived overload function.
	 * @param filename: file containing the describtion of a 3D Mesh
	 * @param dest: a Mesh3D instance to be filled by the parsed data
	 * @returns: a string that was no handled by the parser or and empty string to denote
	 *           success.
	 */
	virtual std::string streamReader(const std::string& filename, Mesh3D<>& dest);
	virtual std::string streamReader(const std::string& filename, Mesh3Df& dest);

	virtual ~Deserializer() = default;

protected:
	/*
	 * a method to be run mainly in a worker thread to fill-in the data members
	 * with an array of sequential tokens for later parse.
	 * @param: no params
	 * @returns: nothing
	 */
	virtual void parseSeqTokens();

};
#endif//DESERIALIZER_H

