#pragma once
#ifndef DESERIALIZER_H
#define DESERIALIZER_H
//---------STL---------------
#include <string>  // std::string
#include <unordered_map> // std::unordered_map
#include <functional>  // std::function
#include <thread>  // std::thread
//---------------------------
#include "Namespace.h"

class conv::Deserializer
{
	json m_jsonFile;
	std::thread m_workerThread;
	std::unordered_map<std::string, std::string> m_allTagPattern;
	std::unordered_map<std::string, conv::parser_t> m_parsers;

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
	explicit Deserializer(const json& jsonFile);
	explicit Deserializer(json&& jsonFile);

	// copy Ctor
	Deserializer(const Deserializer& other) = delete;
	Deserializer(Deserializer&& other) = default;

	// assignement Ctor
	Deserializer& operator=(const Deserializer& other) = delete;
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

	/*
	 * we have a table of "tag"->"parser"
	 * adds a parser of a pattern for a feature expl(vertex, face, normal...etc)
	 * this function takes 3 params: the pattern, an input line and a reference to
	 * Mesh where to store the values
	 * @param tag: the tag which corresponds to a feature in the JSON template file
	 * @param parser: a function of signature boo(string, string, Mesh3D&)
	 * @return: conv::RET_COD
	 */
	conv::RET_CODE addParser(const std::string& tag, parser_t parser);

	virtual ~Deserializer() = default;

protected:
	/*
	 * a method to be run mainly in a worker thread to create a map between
	 * "tag" -> "pattern" for each element of the json file, in the background.
	 * @param: no params
	 * @returns: nothing
	 */
	virtual void tagPattern();

	/*
	 * given a TAG that denotes a Mesh feature (like a vertex), and a pattern of
	 * how this line should be parsed. extract the data and put it inside the Mesh3D
	 * container.
	 * @param inputLine: input line as a stream to be parsed.
	 * @param pattern: the way this data is laid out inside the 3D Mesh file
	 * @param dest: the 3D Mesh container where to save the data
	 * @returns: true for success, false otherwise
	 */
	virtual bool parsePattern(std::string& inputLine, Mesh3D<>& dest) const;
	virtual bool parsePattern(std::string& inputLine, Mesh3Df& dest) const;

	bool dispatcher(const std::string& tag, const std::string& inputLine, Mesh3D<>& dest) const;
};
#endif//DESERIALIZER_H

