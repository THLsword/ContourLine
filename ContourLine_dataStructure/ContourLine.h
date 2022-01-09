#ifndef CONTOURLINE_H
#define CONTOURLINE_H

//--------------------- cgal heat method
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/Seam_mesh.h>
#include <CGAL/Heat_method_3/Surface_mesh_geodesic_distances_3.h>



using namespace std;

typedef CGAL::Simple_cartesian<double>   Kernel;
typedef Kernel::Point_2                  Point_2;
typedef Kernel::Point_3                  Point_3;
typedef Kernel::Line_2                   Line_2;
typedef Kernel::Vector_3				 Vector_3;
typedef CGAL::Surface_mesh<Point_3>      Triangle_mesh;

typedef boost::graph_traits<Triangle_mesh>::vertex_descriptor      vertex_descriptor;
typedef boost::graph_traits<Triangle_mesh>::face_descriptor        face_descriptor;
typedef boost::graph_traits<Triangle_mesh>::halfedge_descriptor	   halfedge_descriptor;
typedef boost::graph_traits<Triangle_mesh>::edge_descriptor		   edge_descriptor;

typedef Triangle_mesh::Property_map<vertex_descriptor, double>                Vertex_distance_map;
typedef CGAL::Heat_method_3::Surface_mesh_geodesic_distances_3<Triangle_mesh> Heat_method;

vector<vector<Point_3>> ContourLine(Triangle_mesh& tm_, Vertex_distance_map VD_map_, double , int quantity);




#endif // !CONTOURLINE_H
