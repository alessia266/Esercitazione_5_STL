#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <math.h>

namespace PolygonalLibrary{

bool ImportMesh(const string& filepath,
                PolygonalMesh& mesh)
{
    //machine precision
    float epsilon = 1.0;
    while ((1+epsilon) > 1.0)
        epsilon /= 2.0;

    //tolerance
    const float tau = 1.0e-10;
    const float tau_q = pow(tau, 2);


    if (!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        //check if all markers0D have been stored correctly
        cout << "Cell0D marker:" << endl;
        for (auto it = mesh.MarkersCell0D.begin(); it != mesh.MarkersCell0D.end(); it++)
        {
            cout << "Key:\t" << it -> first << "\t values";

            for (const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if (!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        //check if all markers1D have been stored correctly
        cout << "Cell1D marker:" << endl;
        for (auto it = mesh.MarkersCell1D.begin(); it != mesh.MarkersCell1D.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if (!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {

        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            //test if vertices of edges are listed in VerticesCell2D
            vector<unsigned int> edges = mesh.EdgesCell2D[c];

            for(unsigned int e = 0; e < edges.size(); e++)
            {
                const unsigned int origin = mesh.IdVert1D[edges[e]](0);
                const unsigned int end = mesh.IdVert1D[edges[e]](1);

                auto findOrigin = find(mesh.VerticesCell2D[c].begin(), mesh.VerticesCell2D[c].end(), origin);
                if (findOrigin == mesh.VerticesCell2D[c].end())
                {
                    cerr << "Wrong mesh at id " << c << endl;
                    return 2;
                }

                auto findEnd = find(mesh.VerticesCell2D[c].begin(), mesh.VerticesCell2D[c].end(), end);
                if (findEnd == mesh.VerticesCell2D[c].end())
                {
                    cerr << "Wrong mesh at id " << c << endl;
                    return 3;
                }

                //test if the edges of the polygon have non-zero length
                double xE_origin = mesh.IdCoord0D[origin](0);
                double xE_end = mesh.IdCoord0D[end](0);
                double yE_origin = mesh.IdCoord0D[origin](1);
                double yE_end = mesh.IdCoord0D[end](1);
                if (fmax(fabs(xE_end - xE_origin), fabs(yE_end - yE_origin)) < fmax(epsilon, tau))
                {
                    cerr << "Zero length edge at id " << edges[e] << endl;
                    return 4;
                }
            }

            //check if area of polygon is non-zero
            vector<unsigned int> vertices = mesh.VerticesCell2D[c];
            double area = 0;
            double xV0 = mesh.IdCoord0D[vertices[0]](0);
            double yV0 = mesh.IdCoord0D[vertices[0]](1);

            for (unsigned int v = 1; v < vertices.size() - 1; v++)
            {
                //compute area using cross product
                area += ((mesh.IdCoord0D[vertices[v]](0) - xV0)*(mesh.IdCoord0D[vertices[v+1]](1) - yV0) -
                       (mesh.IdCoord0D[vertices[v]](1) - yV0)*(mesh.IdCoord0D[vertices[v+1]](0) - xV0))/2;
            }

            if (fabs(area) < fmax(epsilon, tau_q))
            {
                cerr << "Zero area polygon at id " << c << endl;
                return 5;
            }


        }


    }

    return true;
}


//*************************************************************************************************************************************
bool ImportCell0Ds(const string& filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if (file.fail())
        return false;

    list<string> listLines;
    string line;

    while (getline(file, line))
    {
        replace(line.begin(), line.end(), ';', ' ');
        listLines.push_back(line);
    }

    file.close();

    //delete first line
    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    for(const string& line : listLines)
    {
        istringstream convert(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        convert >> id >> marker >> coord(0) >> coord(1);

        //test if marker is set according to the coordinates
        switch (marker)
        {
        case 0:
            if (coord(0) == 0.0 || coord(0) == 1.0 || coord(1) == 0.0 || coord(1) == 1.0)
            {
                cerr << "Marker Error in Vertex Id " << id << endl;
                return false;
            }
            break;
        case 1:
            if (!(coord(0) == 0.0 && coord(1) == 0.0))
            {
                cerr << "Marker Error in Vertex Id " << id << endl;
                return false;
            }
            break;
        case 2:
            if (!(coord(0) == 1.0 && coord(1) == 0.0))
            {
                cerr << "Marker Error in Vertex Id " << id << endl;
                return false;
            }
            break;
        case 3:
            if (!(coord(0) == 1.0 && coord(1) == 1.0))
            {
                cerr << "Marker Error in Vertex Id " << id << endl;
                return false;
            }
            break;
        case 4:
            if (!(coord(0) == 0.0 && coord(1) == 1.0))
            {
                cerr << "Marker Error in Vertex Id " << id << endl;
                return false;
            }
            break;
        case 5:
            if (!(coord(1) == 0.0 && (!(coord(0) == 0.0 || coord(0) == 1.0))))
            {
                cerr << "Marker Error in Vertex Id " << id << endl;
                return false;
            }
            break;
        case 6:
            if (!(coord(0) == 1.0 && (!(coord(1) == 0.0 || coord(1) == 1.0))))
            {
                cerr << "Marker Error in Vertex Id " << id << endl;
                return false;
            }
            break;
        case 7:
            if (!(coord(1) == 1.0 && (!(coord(0) == 0.0 || coord(0) == 1.0))))
            {
                cerr << "Marker Error in Vertex Id " << id << endl;
                return false;
            }
            break;
        case 8:
            if (!(coord(0) == 0.0 && (!(coord(1) == 0.0 || coord(1) == 1.0))))
            {
                cerr << "Marker Error in Vertex Id " << id << endl;
                return false;
            }
            break;
        default:
            cerr << "Marker Error in Vertex Id " << id << endl;
            return false;
        }

        //map with key = id, value = marker
        mesh.IdMark0D.insert({id, marker});

        //map with key = id, value = coordinates array
        mesh.IdCoord0D.insert({id, coord});

        if (marker != 0)
        {
            auto ret = mesh.MarkersCell0D.insert({marker, {id}});
            if (!ret.second)
                (ret.first)->second.push_back(id);
        }
    }
    file.close();
    return true;
}
//*************************************************************************************************************
bool ImportCell1Ds(const string& filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;

    while (getline(file, line))
    {
        replace(line.begin(), line.end(), ';', ' ');
        listLines.push_back(line);
    }

    //delete first line
    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    for(const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >> id >> marker >> vertices(0) >> vertices(1);

        //test if marker has been stored correctly
        unsigned int m0 = mesh.IdMark0D.find(vertices(0))->second;
        unsigned int m1 = mesh.IdMark0D.find(vertices(1))->second;
        switch (marker)
        {
        case 5:
            if (!((m0 == 5 || m0 == 1 || m0 == 2) && (m1 == 5 || m1 == 1 || m1 == 2)))
            {
                cerr << "Marker Error in Edge Id " << id << endl;
                return false;
            }
            break;
        case 6:
            if (!((m0 == 6 || m0 == 2 || m0 == 3) && (m1 == 6 || m1 == 2 || m1 == 3)))
            {
                cerr << "Marker Error in Edge Id " << id << endl;
                return false;
            }
            break;
        case 7:
            if (!((m0 == 7 || m0 == 3 || m0 == 4) && (m1 == 7 || m1 == 3 || m1 == 4)))
            {
                cerr << "Marker Error in Edge Id " << id << endl;
                return false;
            }
            break;
        case 8:
            if (!((m0 == 8 || m0 == 4 || m0 == 1) && (m1 == 8 || m1 == 4 || m1 == 1)))
            {
                cerr << "Marker Error in Edge Id " << id << endl;
                return false;
            }
            break;
        case 0:
            if (((m0 == 5 || m0 == 1 || m0 == 2) && (m1 == 5 || m1 == 1 || m1 == 2))
                || ((m0 == 6 || m0 == 2 || m0 == 3) && (m1 == 6 || m1 == 2 || m1 == 3))
                || ((m0 == 7 || m0 == 3 || m0 == 4) && (m1 == 7 || m1 == 3 || m1 == 4))
                || ((m0 == 8 || m0 == 4 || m0 == 1) && (m1 == 8 || m1 == 4 || m1 == 1)))
            {
                cerr << "Marker Error in Edge Id " << id << endl;
                return false;
            }
            break;
        default:
            cerr << "Marker Error in Edge Id " << id << endl;
            return false;
        }

        //map with key = id, value = begin, end of edges
        mesh.IdVert1D.insert({id, vertices});

        //map with key = id, value = marker
        mesh.IdMark1D.insert({id, marker});

        if (marker != 0)
        {
            auto ret = mesh.MarkersCell1D.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}
//***************************************************************************************************
bool ImportCell2Ds(const string& filename,
                   PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if (file.fail())
        return false;

    list<string> listLines;
    string line;

    while (getline(file, line))
    {
        replace(line.begin(), line.end(), ';', ' ');
        listLines.push_back(line);
    }

    //delete first line
    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.IdCell2D.reserve(mesh.NumberCell2D);
    mesh.VerticesCell2D.reserve(mesh.NumberCell2D);
    mesh.EdgesCell2D.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int num;
        unsigned int numberVertices;
        unsigned int numberEdges;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;

        converter >> id;

        //test if marker != 0
        converter >> marker;
        if (marker != 0)
        {
            cerr << "Marker Error in Polygon Id " << id << endl;
            return false;
        }

        converter >> numberVertices;
        vertices.reserve(numberVertices);
        for(unsigned int i = 0; i < numberVertices; i++)
        {
            converter >> num;
            vertices.push_back(num);
        }

        converter >> numberEdges;
        edges.reserve(numberEdges);
        for(unsigned int i = 0; i < numberEdges; i++)
        {
            converter >> num;
            edges.push_back(num);
        }

        mesh.IdCell2D.push_back(id);
        mesh.VerticesCell2D.push_back(vertices);
        mesh.EdgesCell2D.push_back(edges);
    }

    file.close();

    return true;
}

}
