#include "ContourLine.h"

vector<vector<Point_3>> ContourLine(Triangle_mesh &tm_, Vertex_distance_map VD_map_, double contour_interval_, int quantity) {
	vector<vector<Point_3>> contour_vertices_(quantity);

	for (face_descriptor fd : faces(tm_)) {
		// [Vertex 1 -> Vertex 2 -> Vertex 3 -> Vertex 1]
		std::vector<double>   vertexDistance;
		std::vector<Point_3>  thisFace;
		for (vertex_descriptor vd : vertices_around_face(tm_.halfedge(fd), tm_)) {
			float distance = get(VD_map_, vd);
			vertexDistance.push_back(distance);
			thisFace.push_back(tm_.point(vd));
		}
		vertexDistance.push_back(vertexDistance[0]);
		thisFace.push_back(thisFace[0]);

		//算出每個點在哪一層
		
		int Layer_a = (int)(vertexDistance[0] / contour_interval_);
		int Layer_b = (int)(vertexDistance[1] / contour_interval_);
		int Layer_c = (int)(vertexDistance[2] / contour_interval_);
		std::vector<int> layrt_list{ Layer_a, Layer_b, Layer_c, Layer_a };

		for (int i = 0; i < 3; i++) {	//triangle's 3 edges
			int layer_Difference = abs(layrt_list[i] - layrt_list[i + 1]);//difference
			int closer_point  = (layrt_list[i] < layrt_list[i + 1]) ? i : i + 1;
			int farther_point = (layrt_list[i] < layrt_list[i + 1]) ? i + 1 : i;
			for (int j = 1; j <= layer_Difference; j++) {
				if (layrt_list[closer_point] + j <= quantity) {//防止算上最遠點
					//求出離source的距離
					double layerDistance = (layrt_list[closer_point] + j) * contour_interval_;
					double weight = (layerDistance - vertexDistance[closer_point]) / (vertexDistance[farther_point] - vertexDistance[closer_point]);

					Point_3 vector_(thisFace[closer_point].x() + ((thisFace[farther_point].x() - thisFace[closer_point].x()) * weight),
									thisFace[closer_point].y() + ((thisFace[farther_point].y() - thisFace[closer_point].y()) * weight),
									thisFace[closer_point].z() + ((thisFace[farther_point].z() - thisFace[closer_point].z()) * weight));
					contour_vertices_[layrt_list[closer_point] + j - 1].push_back(vector_);
				}
			}
		}
	}
	std::cout << "contour_vertices_.size: " << contour_vertices_.size() << endl;
	for (int i = 0; i < contour_vertices_.size(); i++) {
		std::cout << "contour_vertices_[" << i << "]" << contour_vertices_[i].size()/2 << endl;
	}
	std::cout << endl;
	return contour_vertices_;
}