#include <iostream>
#include "Utils.hpp"
#include "PolygonalMesh.hpp"


using namespace std;
using namespace PolygonalLibrary;
using namespace Eigen;


int main()
{
    PolygonalMesh mesh;

    string filepath = "PolygonalMesh";

    if(!ImportMesh(filepath,
                    mesh))
        return 1;

    return 0;
}
