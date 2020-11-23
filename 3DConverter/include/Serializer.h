#pragma once
#ifndef SERIALIZER_H
#define SERIALIZER_H

//---------------STL------------
#include <ostream>
#include <thread>
#include <string>
#include <unordered_map>
#include <unordered_set>
//------------------------------
#include "Namespace.h"

class conv::Serializer
{
	json m_jsonFile;
	std::thread m_workerThread;
	std::unordered_map<std::string, std::string> m_allTagPattern;
	std::unordered_map<std::string, conv::stamper_t> m_stampers;

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
	explicit Serializer(const std::string& jsonFile);

	// on the fly constructors
	explicit Serializer(const json& jsonFile);
	explicit Serializer(json&& jsonFile);

	// copy Ctor
	Serializer(const Serializer& other) = delete;
	Serializer(Serializer&& other) = default;

	// assignement Ctor
	Serializer& operator=(const Serializer & other) = delete;
	Serializer& operator=(Serializer && other) = default;

	/*
	 * Given a 3DMesh instance, get its vertices and faces and serialize them in
	 * the opened output stream following the pattern given in the json file.
	 * @param mesh: the 3DMesh object containing the verticies data
	 * @param dest: an output stream ready to receive formatted strings
	 * @returns: returns nothing given the stream did'n cause exceptions.
	 */
	virtual void streamWriter(const Mesh3D<>& mesh, std::ostream & dest);
	virtual void streamWriter(const Mesh3Df& mesh, std::ostream & dest);

	/*
	 * we have a table of "tag"->"parser"
	 * adds a parser of a pattern for a feature expl(vertex, face, normal...etc)
	 * this function takes 3 params: the pattern, an input line and a reference to
	 * Mesh where to store the values
	 * @param tag: the tag which corresponds to a feature in the JSON template file
	 * @param parser: a function of signature boo(string, string, Mesh3D&)
	 * @return: conv::RET_COD
	 */
	conv::RET_CODE addStamper(const std::string & tag, stamper_t stamper);

	virtual ~Serializer() = default;

protected:
	/*
	 * a method to be run mainly in a worker thread to create a map between
	 * "tag" -> "pattern" for each element of the json file, in the background.
	 * @param: no params
	 * @returns: nothing
	 */
	virtual void tagPattern();
private:
	// friend functions for test purpose only
	friend std::string dumpTheJsonFile(Serializer& self);
	friend std::unordered_set<std::string> dumpAllTags(Serializer& self);
	friend std::unordered_set<std::string> dumpAllPatterns(Serializer& self);
};
#endif//SERIALIZER_H


