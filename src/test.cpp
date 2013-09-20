NSTANDARD_TEMPLATES
#define NONSTANDARD_TEMPLATES 1

#include <Geometry/Matrix.h>
#include <GLColorOps.h>
include <Geometry/ComponentArray.h>


int main(void) {
  printf("hello world");
  Geometry::ComponentArray<GLfloat,4> viewPos(0.0f,0.0f,1.0f,0.0f);
  return 0;
}
