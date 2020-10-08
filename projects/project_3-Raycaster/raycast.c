#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jsonparser.c"

typedef struct {
  int type; // 0 = camera, 1 = sphere, 2 = plane
  double color[3];
  union {
    struct {
      double width;
      double height;
    } camera;

    struct {
      double position[3];
      double radius;
    } sphere;

    struct {
      double position[3];
      double normal[3];
    } plane;
  };
} Object;

double sphereIntersection(double* Ro, double* Rd, double* Center, double r) {
  	double a = sqr(Rd[0]) + sqr(Rd[1]) + sqr(Rd[2]);
  	double b = 2*(Ro[0]*Rd[0] + Ro[1]*Rd[1] + Ro[2]*Rd[2] - Rd[0]*Center[0] - Rd[1]*Center[1] - Rd[2]*Center[2]);
  	double c = sqr(Ro[0]) + sqr(Ro[1]) + sqr(Ro[2]) + sqr(Center[0]) +
             sqr(Center[1]) + sqr(Center[2]) - 2*(Ro[0]*Center[0]
             + Ro[1]*Center[1] + Ro[2]*Center[2]) - sqr(r);
  	double det = sqr(b) - 4*a*c;
  	if (det < 0) return -1;
  	det = sqrt(det);
  	double t0 = (-b - det) / (2*a);
 	double t1 = (-b + det) / (2*a);
	if (t0 > 0) return t0;
 	if (t1 > 0) return t1;

  	return -1;
}

double planeIntersection(double* Ro, double* Rd, double* position, double* normal) {
  	double t = - (normal[0]*Ro[0] + normal[1]*Ro[1] + normal[2]*Ro[2] - normal[0]*position[0]
                - normal[1]*position[1] - normal[2]*position[2]) / (normal[0]*Rd[0]
                + normal[1]*Rd[1] + normal[2]*Rd[2]);

  	if (t > 0) return t;
	return -1;
}

int intersect(double* Rd, int objectNum, Object** objects) {
	int closestObjectNum = -1;
	double bestT = INFINITY;
	int i;
	double t;
	double Ro[3] = { 0, 0, 0 };
	for (i = 0; i<objectNum; i++){
		if (objects[i]->type == 1) {
			t = sphereIntersection(Ro, Rd, objects[i]->sphere.position, objects[i]->sphere.radius);
			if (t) {
				if (t > 0 && t <= bestT) {
					bestT = t;
					closestObjectNum = i;
				}
			}
			else {
				fprintf(stderr, "Error: finding the distance unsuccessfully.\n");
				return (1);
			}
		}
		else if (objects[i]->type == 2) {
			t = planeIntersection(Ro, Rd, objects[i]->plane.position, objects[i]->plane.normal);
			if (t) {
				if (t > 0 && t <= bestT) {
					bestT = t;
					closestObjectNum = i;
				}
			}
			else {
				fprintf(stderr, "Error: finding the distance unsuccessfully.\n");
				return (1);
			}
		}
	}
	return closestObjectNum;
}

PPMimage* rayCasting(char* filename, int w, int h, Object** objects) {
	PPMimage* buffer = (PPMimage*)malloc(sizeof(PPMimage));
	if (objects[0] == NULL) {
		fprintf(stderr, "Error: no object found");
		exit(1);
	}
	int cameraFound = 0;
	double width;
	double height;
	int i;
	for (i = 0; objects[i] != 0; i += 1) {
		if (objects[i]->type == 0) {
			cameraFound = 1;
			width = objects[i]->camera.width;
			height = objects[i]->camera.height;
			if (width <= 0 || height <= 0) {
				fprintf(stderr, "Error: invalid camera size");
				exit(1);
			}
		}
	}
	if (cameraFound == 0) {
		fprintf(stderr, "Error: Camera not found");
		exit(1);
	}

	buffer->pixData = (unsigned char*)malloc(w*h * sizeof(PPMRGBpixel));
	PPMRGBpixel *pixel = (PPMRGBpixel*)malloc(sizeof(PPMRGBpixel));
	if (buffer->pixData == NULL || buffer == NULL) {
		fprintf(stderr, "Error: memory allocation failed. \n");
		exit(1);
	}

	double pixwidth = width / w;
	double pixheight = height / h;
	double pointx, pointy, pointz;
	int j, k;
	for (k = 0; k<h; k++) {
		int count = (h-k-1)*w*3;
		double vy = -height / 2 + pixheight * (k + 0.5);
		for (j = 0; j<w; j++) {
			double vx = -width / 2 + pixwidth * (j + 0.5);
			double Rd[3] = { vx, vy, 1 };

			normalize(Rd);
			int intersection = intersect(Rd, i, objects);
			if (intersection>=0) {
				pixel->r = (int)((objects[intersection]->color[0]) * 255);
				pixel->g = (int)((objects[intersection]->color[1]) * 255);
				pixel->b = (int)((objects[intersection]->color[2]) * 255);
			}
			else {
				pixel->r = 0;
				pixel->g = 0;
				pixel->b = 0;
			}
			buffer->pixData[count++] = pixel->r;
			buffer->pixData[count++] = pixel->g;
			buffer->pixData[count++] = pixel->b;
			}
		}
	return buffer;
}


int main (int argc, char *argv[]) {

    // number of arguments is incorrect
    if (argc != 5) {
       fail("Invaild number of arguments");
    }
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    char *input = argv[3];
    char *output = argv[4];

    // First argument given is invalid
    if (width <= 0) {
        fail( "Invalid Width argument");
    }
    // Second argument given is invalid
    else if (height <= 0) {
        fail( "Invalid Height argument");
    }
    // Third Argument: file does not exist
    else if (!fileExists(input)) {
        fail("input file does not exist");
    }
    // Third Argument: incorrect type
    else if(!isFileType(input, ".json")) {
        fail("input file is not of type .json");
    }
    // Fourth Argument: incorrect type
    else if(!isFileType(output, ".ppm")) {
        fail("output file is not of type .ppm");
    }

    readLine(input);

    return(0);
}
