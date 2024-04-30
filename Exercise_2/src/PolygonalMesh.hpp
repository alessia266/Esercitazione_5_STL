#pragma once

#include <iostream>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary{

struct PolygonalMesh
{
    //cell0D
    unsigned int NumberCell0D = 0;
    std::map<unsigned int, list<unsigned int>> MarkersCell0D = {};  //map: {marker, {id}}
    std::map<unsigned int, unsigned int> IdMark0D = {};  //map: {id, marker}
    std::map<unsigned int, Vector2d> IdCoord0D = {};  //map: {id, Coordinates}

    //cell1D
    unsigned int NumberCell1D = 0;
    std::map<unsigned int, list<unsigned int>> MarkersCell1D = {}; //map: {marker, {id}}
    std::map<unsigned int, unsigned int> IdMark1D = {}; //map: {id, marker}
    std::map<unsigned int, Vector2i> IdVert1D = {};  //map: {id, Vertices}


    //cell2D
    unsigned int NumberCell2D = 0;
    std::vector<unsigned int> IdCell2D = {};
    std::vector<vector<unsigned int>> VerticesCell2D = {};
    std::vector<vector<unsigned int>> EdgesCell2D = {};

};
}
