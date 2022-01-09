#include"SaddlePoint.h"

vector<Point_3> saddlepoint(Triangle_mesh& tm_, Vertex_distance_map VD_map_) {
	vector<Point_3> temp;
	//遍歷mesh的所有vertices
	for (vertex_descriptor vd : vertices(tm_)) {
		double D = get(VD_map_, vd);
		//遍歷每個vertex的一環領域(1-ring)
		CGAL::Vertex_around_target_circulator<Triangle_mesh> vbegin(tm_.halfedge(vd), tm_), done(vbegin);
		double pre = D - get(VD_map_, *vbegin);
		int count = 0;
		do {
			*vbegin++;
			double distance = get(VD_map_, *vbegin);
			//如果 pre-distance 的符號變化兩次以上，則認為是saddlepoint
			if (pre * (D-distance) <= 0) count++;
			pre = D - distance;
		} while (vbegin != done);
		if (count > 2) {
			temp.push_back(tm_.point(vd));
		}
	}
	return temp;
}