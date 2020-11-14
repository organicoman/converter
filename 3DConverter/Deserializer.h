#pragma once
#ifndef DESERIALIZER_H
#define DESERIALIZER_H
//---------STL---------------
#include <string>  // std::string
#include <unordered_map> // std::unordered_map
#include <vector>  // std::vector
#include <thread>  // std::thread
//---------------------------
#include "Namespace.h"

class conv::Deserializer
{
	json m_jsonFile;
	std::thread m_workerThread;
	std::unordered_map<std::string, std::string> m_allTagPattern;

public:
	/*
	 * constructs this object, by populating the m_jsonFile data member 
	 * from the argument file, then start a worker thread to fill in the 
	 * different patterns with sequential parsing tokens extracted from 
	 * the m_jsonFile structure.
	 * @param jsonFile: the name of the json file which define the template of
	 *                  the file to be parsed later.
	 * @returns: nothing! this is a constructor
	 */
	explicit Deserializer(const std::string& jsonFile);

	// on the fly constructors
	constexpr explicit Deserializer(const json& jsonFile);
	explicit constexpr Deserializer(json&& jsonFile);

	// copy Ctor
	Deserializer(const Deserializer& other) = default;
	Deserializer(Deserializer&& other) = default;

	// assignement Ctor
	Deserializer& operator=(const Deserializer& other) = default;
	Deserializer& operator=(Deserializer&& other) = default;

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
	 * a method to be run mainly in a worker thread to create a map between
	 * "tag" -> "pattern" for each element of the json file, in the background.
	 * @param: no params
	 * @returns: nothing
	 */
	virtual void tagPattern();

};
#endif//DESERIALIZER_H

