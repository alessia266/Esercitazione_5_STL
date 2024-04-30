#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary{

//import the polygonal mesh and test if the mesh is correct
//return true if the reading is success and false otherwise
bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh);

//import the cell0D properties from Cell0Ds.csv file
//return true if the reading is success and false otherwise
bool ImportCell0Ds(const string& filepath,
                   PolygonalMesh& mesh);

//import the cell1D properties from Cell1Ds.csv file
//return true if the reading is success and false otherwise
bool ImportCell1Ds(const string& filepath,
                   PolygonalMesh& mesh);

//import the cell2D properties from Cell2Ds.csv file
//return true if the reading is success and false otherwise
bool ImportCell2Ds(const string& filepath,
                   PolygonalMesh& mesh);

}
