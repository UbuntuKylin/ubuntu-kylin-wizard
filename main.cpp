#include "blur.h"
#include "draw.h"
#include "style.h"

int main (int argc, char *argv[])
{
  gtk_init(&argc, &argv);

  Draw *draw = new Draw();
  draw->Run();

  gtk_main();

  return 0;
}
