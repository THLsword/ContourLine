#include"SaddlePoint.h"

vector<Point_3> saddlepoint(Triangle_mesh& tm_, Vertex_distance_map VD_map_) {
	vector<Point_3> temp;
	//�M��mesh���Ҧ�vertices
	for (vertex_descriptor vd : vertices(tm_)) {
		double D = get(VD_map_, vd);
		//�M���C��vertex���@�����(1-ring)
		CGAL::Vertex_around_target_circulator<Triangle_mesh> vbegin(tm_.halfedge(vd), tm_), done(vbegin);
		double pre = D - get(VD_map_, *vbegin);
		int count = 0;
		do {
			*vbegin++;
			double distance = get(VD_map_, *vbegin);
			//�p�G pre-distance ���Ÿ��ܤƨ⦸�H�W�A�h�{���Osaddlepoint
			if (pre * (D-distance) <= 0) count++;
			pre = D - distance;
		} while (vbegin != done);
		if (count > 2) {
			temp.push_back(tm_.point(vd));
		}
	}
	return temp;
}