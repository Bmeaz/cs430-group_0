#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//tracks current line
int line = 1;

int nextC(FILE* json) {
	int c = fgetc(json);
#ifdef DEBUG
	printf("nextC: '%c'\n", c);
#endif
	if (c == '\n') {
		line += 1;
	}
	if (c == EOF) {
		fprintf(stderr, "Error: Unexpected end of file on line number %d. \n", line);
		fclose(json);
		exit(1);
	}
	return c;
}

void expectC(FILE* json, int d) {
	int c = nextC(json);
	if (c == d) return;
	fprintf(stderr, "Error: Expected '%c' on line %d.\n", d, line);
	fclose(json);
	exit(1);
}

void skipWS(FILE *json) {
	int c = nextC(json);
	while (isspace(c)) {
		c = nextC(json);
	}
	ungetc(c, json);
}

char* nextString(FILE* json) {
	char buffer[129];
	int c = nextC(json);
	if (c != '"') {
		fprintf(stderr, "Error: Expected string on line %d.\n", line);
		fclose(json);
		exit(1);
	}
	c = nextC(json);
	int i = 0;
	while (c != '"') {
		if (i >= 128) {
			fprintf(stderr, "Error: Strings this long are not supported.\n");
			fclose(json);
			exit(1);
		}
		if (c < 32 || c > 126) {
			fprintf(stderr, "Error: Strings may contain only ascii characters.\n");
			fclose(json);
			exit(1);
		}
		buffer[i] = c;
		i += 1;
		c = nextC(json);
	}
	buffer[i] = 0;
	return strdup(buffer);
}

double nextNumber(FILE* json) {
	double value;

	int f = fscanf(json, "%lf", &value);
	if (f != 1) {
		fprintf(stderr, "Error: floating point is expected in line number %d.\n", line);
	}
	return value;
}

double* nextVector(FILE* json) {
	double* v = malloc(3 * sizeof(double));
	expectC(json, '[');
	skipWS(json);
	v[0] = nextNumber(json);
	skipWS(json);
	expectC(json, ',');
	skipWS(json);
	v[1] = nextNumber(json);
	skipWS(json);
	expectC(json, ',');
	skipWS(json);
	v[2] = nextNumber(json);
	skipWS(json);
	expectC(json, ']');
	return v;
}

void readScene(char* filename, Object** objects) {
	int c;
	FILE* json = fopen(filename, "r");
	if (json == NULL) {
		fprintf(stderr, "file could not be opened %s.\n", filename);
		exit(1);
	}
	skipWS(json);

	// beginning of list
	expectC(json, '[');
	skipWS(json);

	// Find objects
	int i = 0;
	while (1) {
		Object* object = malloc(sizeof(Object));
		objects[i] = object;
		c = fgetc(json);
		if (c == '{') {
			skipWS(json);

			// Parse object
			char* key = nextString(json);
			if (strcmp(key, "type") != 0) {
				fprintf(stderr, "Error: Expected \"type\" key on line number %d.\n", line);
				fclose(json);
				exit(1);
			}
			skipWS(json);

			expectC(json, ':');

			skipWS(json);

			char* value = nextString(json);

			// checks if its a sphere or a plane, and its color
			char* tempKey = value;
			// save the type value for the object
			if (strcmp(value, "camera") == 0) {
				objects[i]->type = 0;
			}
			else if (strcmp(value, "sphere") == 0) {
				objects[i]->type = 1;
			}
			else if (strcmp(value, "plane") == 0) {
				objects[i]->type = 2;
			}
			else {
				fprintf(stderr, "Error: Unknown type, \"%s\", on line number %d.\n", value, line);
				fclose(json);
				exit(1);
			}

			skipWS(json);

			while (1) {
				c = nextC(json);
				if (c == '}') {
					break;
				}
				else if (c == ',') {
					skipWS(json);
					char* key = nextString(json);
					skipWS(json);
					expectC(json, ':');
					skipWS(json);
					if ((strcmp(key, "width") == 0) || (strcmp(key, "height") == 0) ||
						(strcmp(key, "radius") == 0)) {
						double value = nextNumber(json);
						if (strcmp(key, "width") == 0) {
							objects[i]->camera.width = value;
						}
						else if (strcmp(key, "height") == 0) {
							objects[i]->camera.height = value;
						}
						else {
							objects[i]->sphere.radius = value;
						}
					}
					// if key is color, position or normal, then it would be a 3d vector in the json file
					else if ((strcmp(key, "color") == 0) || (strcmp(key, "position") == 0) ||
						strcmp(key, "normal") == 0) {
						double* value = nextVector(json);
						if (strcmp(key, "color") == 0) {
							objects[i]->color[0] = value[0];
							objects[i]->color[1] = value[1];
							objects[i]->color[2] = value[2];
						}
						// if the key is position sphere and plane are both considered
						else if (strcmp(key, "position") == 0) {
							if (strcmp(tempKey, "sphere") == 0) {
								objects[i]->sphere.position[0] = value[0];
								objects[i]->sphere.position[1] = value[1];
								objects[i]->sphere.position[2] = value[2];
							}
							else if (strcmp(tempKey, "plane") == 0) {
								objects[i]->plane.position[0] = value[0];
								objects[i]->plane.position[1] = value[1];
								objects[i]->plane.position[2] = value[2];
							}
							else {
								fprintf(stderr, "Error: Unknown type!.\n");
								exit(1);
							}
						}
						else {
							objects[i]->plane.normal[0] = value[0];
							objects[i]->plane.normal[1] = value[1];
							objects[i]->plane.normal[2] = value[2];
						}
					}
					else {
						fprintf(stderr, "Error: Unkonwn property, %s, on line %d.\n", key, line);
						fclose(json);
						exit(1);
					}
					skipWS(json);
				}
				else {
					fprintf(stderr, "Error: Unexpected value on line %d\n", line);
					fclose(json);
					exit(1);
				}
			}
			skipWS(json);
			c = nextC(json);
			if (c == ',') {
				skipWS(json);
			}
			else if (c == ']') {
				fclose(json);
				//no more objects
				objects[i + 1] = NULL;
				return;
			}
			else {
				fprintf(stderr, "Error: Expecting ',' or ']' on line %d.\n", line);
				fclose(json);
				exit(1);
			}
		}
		// move parser to next object in json
		i = i + 1;
	}
}
