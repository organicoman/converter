#pragma once
#include "include/Mesh3D.h"
#include "include/Vertex.h"
#include "include/Namespace.h"

// vertex reader tag = 'v' x y z 'w
bool OBJvertex(const std::string& pattern, const std::string& input, conv::Mesh3D<>& dest)
{
    conv::splitter<std::string> sIn{ input, ' ', input.begin() };
    if (sIn.next_word() != "v")
        return false;
    conv::splitter<std::string> sPat{ pattern, ' ', pattern.begin() };

    double x = 0, y = 0, z = 0;
    static uint64_t id = 1; // numbering starts form 1 and goes up.

    while (!sPat.next_word().empty())
    {
        if (sPat.curr_word() == conv::_X_)
            x = std::stod(sIn.curr_word());

        if (sPat.curr_word() == conv::_Y_)
            y = std::stod(sIn.curr_word());

        if (sPat.curr_word() == conv::_Z_)
            z = std::stod(sIn.curr_word());

        sIn.next_word();
    }
    dest.addVertex(conv::Vertex<>{id, x, y, z});
    ++id;

    return true;
}

// face reader tag = 'f' cases {v/vt/vn , v/vt , v//vn , v }
bool OBJface(const std::string& pattern, const std::string& input, conv::Mesh3D<>& dest)
{
    conv::splitter<std::string> sIn{ input, ' ', input.begin() };
    std::string word = sIn.next_word();
    if (word != "f")
        return false;
    conv::splitter<std::string> sPat{ pattern, ' ', pattern.begin() };

    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    static uint64_t id = 1; // not mentioned in OBJ specs but used here anyway!
    size_t totalVerts = dest.Vsize();
    while (!sPat.next_word().empty())
    {
        conv::splitter<std::string> sWord(word, '/', word.begin());
        if (sPat.curr_word() == conv::_V1_)
        {
            if (sWord.next_word().empty() && sWord.isFullsize())
                v1 = std::stoi(word);
            else
                v1 = std::stoi(sWord.curr_word());

            if (v1 < 0)
                v1 = totalVerts + v1;
        }

        if (sPat.curr_word() == conv::_V2_)
        {
            if (sWord.next_word().empty() && sWord.isFullsize())
                v2 = std::stoi(word);
            else
                v2 = std::stoi(sWord.curr_word());

            if (v2 < 0)
                v2 = totalVerts + v2;
        }

        if (sPat.curr_word() == conv::_V3_)
        {
            if (sWord.next_word().empty() && sWord.isFullsize())
                v3 = std::stoi(word);
            else
                v3 = std::stoi(sWord.curr_word());

            if (v3 < 0)
                v3 = totalVerts + v3;
        }

        word = sIn.next_word();
    }

    // adding the vertices, CCW
    dest.addFace(conv::Face{ id, (uint64_t)v1, (uint64_t)v2, (uint64_t)v3 });

    // if there is a fourth => vertex parse it
    if (!sIn.curr_word().empty())
    {
        auto word = sIn.curr_word();
        conv::splitter<std::string> sWord(word, '/', word.begin());
        if (sWord.next_word().empty() && sWord.isFullsize())
            v4 = std::stoi(word);
        else
            v4 = std::stoi(sWord.curr_word());

        if (v4 < 0)
            v4 = totalVerts + v4;
        dest.addFace(conv::Face{++id,(uint64_t) v1, (uint64_t)v3, (uint64_t)v4});
    }
    
    ++id;
    return true;
}

// normal reader tag = 'vn' i j k
bool OBJnormal(const std::string& pattern, const std::string& input, conv::Mesh3D<>& dest)
{
    conv::splitter<std::string> sIn{ input, ' ', input.begin() };
    if (sIn.next_word() != "vn")
        return false;
    conv::splitter<std::string> sPat{ pattern, ' ', pattern.begin() };

    double i = 0, j = 0, k = 0;
    static uint64_t id = 1; // numbering starts form 1 and goes up.

    while (!sPat.next_word().empty())
    {
        if (sPat.curr_word() == conv::_I_)
            i = std::stod(sIn.curr_word());

        if (sPat.curr_word() == conv::_J_)
            j = std::stod(sIn.curr_word());

        if (sPat.curr_word() == conv::_K_)
            k = std::stod(sIn.curr_word());

        sIn.next_word();
    }

    auto vertex = dest.getVertex(id);
    vertex.setNormal({i, j ,k});

    ++id;

    return true;
}

// textureCoordinate reader tag = 'vt' u v
bool OBJtextCoord(const std::string& pattern, const std::string& input, conv::Mesh3D<>& dest)
{
    conv::splitter<std::string> sIn{ input, ' ', input.begin() };
    if (sIn.next_word() != "vt")
        return false;
    conv::splitter<std::string> sPat{ pattern, ' ', pattern.begin() };

    double u = 0, v = 0;
    static uint64_t id = 1; // numbering starts form 1 and goes up.

    while (!sPat.next_word().empty())
    {
        if (sPat.curr_word() == conv::_U_)
            u = std::stod(sIn.curr_word());

        if (sPat.curr_word() == conv::_V_)
            v = std::stod(sIn.curr_word());

        sIn.next_word();
    }

    auto vertex = dest.getVertex(id);
    vertex.setTexCoord({ u, v });

    ++id;

    return true;
}