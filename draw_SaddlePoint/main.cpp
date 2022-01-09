#include <iostream>
#include "ContourLine.h"
#include "SaddlePoint.h"

//--------------------- easy3D
#include <easy3d/viewer/viewer.h>
#include <easy3d/renderer/camera.h>
#include <easy3d/renderer/drawable_lines.h>
#include <easy3d/renderer/drawable_points.h>
#include <easy3d/renderer/drawable_triangles.h>
#include <easy3d/fileio/resources.h>
#include <easy3d/core/surface_mesh.h>
#include <easy3d/renderer/renderer.h>
#include <easy3d/renderer/texture_manager.h>
#include <easy3d/fileio/surface_mesh_io.h>
#include <easy3d/core/types.h>
#include <easy3d/util/logging.h>

int main(int argc, char* argv[]) {
	//heat method 算出距離
	Triangle_mesh tm;
	const char* filename = (argc > 1) ? argv[1] : "../../../data/bunny.off";
	std::ifstream input(filename);
	if (!input || !(input >> tm) || tm.is_empty()) {
		std::cerr << "Not a valid off file." << std::endl;
		return 1;
	}
	Heat_method hm(tm);
	Vertex_distance_map vertex_distance = tm.add_property_map<vertex_descriptor, double>("v:distance", 0).first;

	//vertex_descriptor source = *(vertices(tm).first);
	vertex_descriptor source1(280);
	//vertex_descriptor source2(3269);
	hm.add_source(source1);
	//hm.add_source(source2);
	hm.estimate_geodesic_distances(vertex_distance);

	//遍歷一次，找出最遠點
	vertex_descriptor vfar;
	double sdistance = 0;
	for (vertex_descriptor vd : vertices(tm)) {
		if (get(vertex_distance, vd) > sdistance) {
			vfar = vd;
			sdistance = get(vertex_distance, vd);
		}
	}

	//設定等高線數量，算每層的距離
	int quantity = 100;
	double contour_interval = sdistance / quantity;

	//遍歷mesh，求出所有的線
	vector<vector<Point_3>> contour_line;
	contour_line = ContourLine(tm, vertex_distance, contour_interval, quantity);

	//找出saddlepoint
	vector<Point_3> saddlepoints = saddlepoint(tm, vertex_distance);
	cout << saddlepoints.size() << endl;

	//畫出等高線
	easy3d::logging::initialize();
	easy3d::Viewer viewer("contour");

	vector<easy3d::vec3> drawCL_easy3d;
	for (int i = 0; i < contour_line.size(); i++) {
		for (int j = 0; j < contour_line[i].size(); j++) {
			drawCL_easy3d.push_back(easy3d::vec3(contour_line[i][j].x(), contour_line[i][j].y(), contour_line[i][j].z()));
		}
	}
	auto SL_drawable = new easy3d::LinesDrawable("CL");
	SL_drawable->update_vertex_buffer(drawCL_easy3d);
	SL_drawable->set_uniform_coloring(easy3d::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	SL_drawable->set_line_width(1.0f);
	viewer.add_drawable(SL_drawable);


	//畫出saddlepoints
	vector<easy3d::vec3> drawSPs_easy3d;
	for (int i = 0; i < saddlepoints.size();i++) {
		drawSPs_easy3d.push_back(easy3d::vec3(saddlepoints[i].x(), saddlepoints[i].y(), saddlepoints[i].z()));
	}
	cout << drawSPs_easy3d.size() << endl;
	auto SPs_drawable = new easy3d::PointsDrawable("SPs");
	SPs_drawable->update_vertex_buffer(drawSPs_easy3d);
	SPs_drawable->set_uniform_coloring(easy3d::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	SPs_drawable->set_point_size(20.0f);
	SPs_drawable->set_impostor_type(easy3d::PointsDrawable::SPHERE);
	viewer.add_drawable(SPs_drawable);

	////畫出model的edges
	//easy3d::SurfaceMesh* model = dynamic_cast<easy3d::SurfaceMesh*>(viewer.add_model(filename, false));
	//easy3d::LinesDrawable* drawable = model->renderer()->add_lines_drawable("edges");
	//drawable->set_uniform_coloring(easy3d::vec4(1.0f, 0.2f, 0.2f, 1.0f));
	//drawable->set_line_width(2);





	viewer.fit_screen();
	return viewer.run();
}