#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stat (const char *file, struct stat *buf)
{
  return __xstat(_STAT_VER,file,buf);
}
