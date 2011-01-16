#include <D3DX10math.h>
#include <vector>
#include <string>
#include <cassert>
#include <tchar.h>

using namespace std;

struct Face {
	Face(int v0, int v1, int v2) : v0(v0), v1(v1), v2(v2), v3(-1), n0(-1), n1(-1), n2(-1), n3(-1), t0(-1), t1(-1), t2(-1), t3(-1) {}
	Face(int v0, int v1, int v2, int v3) : v0(v0), v1(v1), v2(v2), v3(v3), n0(-1), n1(-1), n2(-1), n3(-1), t0(-1), t1(-1), t2(-1), t3(-1) {}

	Face(int v0, int v1, int v2, int n0, int n1, int n2) : v0(v0), v1(v1), v2(v2), n0(n0), n1(n1), n2(n2), t0(-1), t1(-1), t2(-1) {}
	Face(int v0, int v1, int v2, int v3, int n0, int n1, int n2, int n3) : v0(v0), v1(v1), v2(v2), v3(v3), n0(n0), n1(n1), n2(n2), n3(n3), t0(-1), t1(-1), t2(-1), t3(-1) {}

	Face(int v0, int v1, int v2, int n0, int n1, int n2, int t0, int t1, int t2) : v0(v0), v1(v1), v2(v2), v3(-1), n0(n0), n1(n1), n2(n2), n3(-1), t0(t0), t1(t1), t2(t2), t3(-1) {}
	Face(int v0, int v1, int v2, int v3, int n0, int n1, int n2, int n3, int t0, int t1, int t2, int t3) : v0(v0), v1(v1), v2(v2), v3(v3), n0(n0), n1(n1), n2(n2), n3(n3), t0(t0), t1(t1), t2(t2), t3(t3) {}

	int v0, v1, v2, v3;
	int n0, n1, n2, n3;
	int t0, t1, t2, t3;
};

struct Geometry {

	vector<D3DXVECTOR3> verts;
	vector<D3DXVECTOR3> normals;
	vector<D3DXVECTOR2> tex;

	vector<Face> faces;

	string name;
};

void save(FILE *f, const Geometry &geometry)
{
	if (!geometry.name.empty())
		fprintf(f, "o %s\n", geometry.name.c_str());

	for (int i = 0; i < (int)geometry.verts.size(); ++i)
		fprintf(f, "v %f %f %f\n", geometry.verts[i].x, geometry.verts[i].y, geometry.verts[i].z);

	for (int i = 0; i < (int)geometry.normals.size(); ++i)
		fprintf(f, "vn %f %f %f\n", geometry.normals[i].x, geometry.normals[i].y, geometry.normals[i].z);

	for (int i = 0; i < (int)geometry.tex.size(); ++i)
		fprintf(f, "vt %f %f\n", geometry.tex[i].x, geometry.tex[i].y);

	const bool n = !geometry.normals.empty();
	const bool t = !geometry.tex.empty();
	const bool quad = geometry.faces[0].v3 != -1;
	int mask = t * 2 + n;
	// verts are 1 based
	for (int i = 0; i < (int)geometry.faces.size(); ++i) {
		if (quad) {
			switch (mask) {
			case 0: fprintf(f, "f %d %d %d %d\n", 
								geometry.faces[i].v0 + 1, geometry.faces[i].v1 + 1, geometry.faces[i].v2 + 1, geometry.faces[i].v3 + 1); 
				break;
			case 1: fprintf(f, "f %d//%d %d//%d %d//%d %d//%d\n", 
								geometry.faces[i].v0 + 1, geometry.faces[i].n0 + 1, 
								geometry.faces[i].v1 + 1, geometry.faces[i].n1 + 1, 
								geometry.faces[i].v2 + 1, geometry.faces[i].n2 + 1,
								geometry.faces[i].v3 + 1, geometry.faces[i].n3 + 1);
				break;
			case 3: fprintf(f, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", 
								geometry.faces[i].v0 + 1, geometry.faces[i].t0 + 1, geometry.faces[i].n0 + 1,
								geometry.faces[i].v1 + 1, geometry.faces[i].t1 + 1, geometry.faces[i].n1 + 1,
								geometry.faces[i].v2 + 1, geometry.faces[i].t2 + 1, geometry.faces[i].n2 + 1,
								geometry.faces[i].v3 + 1, geometry.faces[i].t3 + 1, geometry.faces[i].n3 + 1);
				break;
			default: assert(0); break;
			}
		} else {
			switch (mask) {
			case 0: fprintf(f, "f %d %d %d\n", 
								geometry.faces[i].v0 + 1, geometry.faces[i].v1 + 1, geometry.faces[i].v2 + 1); 
				break;
			case 1: fprintf(f, "f %d//%d %d//%d %d//%d\n", 
								geometry.faces[i].v0 + 1, geometry.faces[i].n0 + 1, 
								geometry.faces[i].v1 + 1, geometry.faces[i].n1 + 1, 
								geometry.faces[i].v2 + 1, geometry.faces[i].n2 + 1);
				break;
			case 3: fprintf(f, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", 
								geometry.faces[i].v0 + 1, geometry.faces[i].t0 + 1, geometry.faces[i].n0 + 1,
								geometry.faces[i].v1 + 1, geometry.faces[i].t1 + 1, geometry.faces[i].n1 + 1,
								geometry.faces[i].v2 + 1, geometry.faces[i].t2 + 1, geometry.faces[i].n2 + 1); 
				break;
			default: assert(0); break;
			}
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{

	FILE *f = fopen("c:\\temp\\obj1.obj", "wt");

	Geometry g;
	//g.name = "test";

	float s = 1;
	g.verts.push_back(D3DXVECTOR3(-s,-s,-s));
	g.verts.push_back(D3DXVECTOR3(+s,-s,-s));
	g.verts.push_back(D3DXVECTOR3(-s,+s,-s));
	g.verts.push_back(D3DXVECTOR3(+s,+s,-s));

	g.verts.push_back(D3DXVECTOR3(-s,-s,+s));
	g.verts.push_back(D3DXVECTOR3(+s,-s,+s));
	g.verts.push_back(D3DXVECTOR3(-s,+s,+s));
	g.verts.push_back(D3DXVECTOR3(+s,+s,+s));

	g.faces.push_back(Face(0, 2, 3, 1));
	g.faces.push_back(Face(0, 1, 5, 4));
	g.faces.push_back(Face(4, 6, 7, 5));
	g.faces.push_back(Face(5, 7, 3, 1));
	g.faces.push_back(Face(2, 3, 7, 6));
	g.faces.push_back(Face(2, 6, 4, 0));

	save(f, g);

	fclose(f);

	return 0;
}

