#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


// Return the maximum of two integers.
int
max(int a, int b) {
  if (a > b)
    return a;
  return b;
}

// fmtname: return last element of path (file name and trailing spaces)
char*
fmtname(char *path, int hasTrailingSpaces)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  if (hasTrailingSpaces)
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  else
    buf[strlen(p)] = '\0';
  return buf;
}

int
find(char *path, char *targetName) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return 1;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return 1;
  }

  int isFound = 0;

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    printf("%s %d %d %d\n", fmtname(path, 0), st.type, st.ino, (int) st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }

      char *fileName = fmtname(buf, 0);

      // If the file is found, print the path.
      if (st.type == T_FILE && strcmp(fileName, targetName) == 0) {
        isFound = 1;
        printf("%s\n", buf);
      }
      // If the file is a directory, recursively search the directory.
      else if (st.type == T_DIR && strcmp(fileName, ".") != 0 && strcmp(fileName, "..") != 0) {
        isFound = max(isFound, find(buf, targetName));
      }
    }
    break;
  }
  close(fd);
  return isFound;
}

int
main(int argc, char *argv[]) {
    
    if (argc != 3) {
        fprintf(2, "Usage: find <path> <name>\n");
        exit(1);
    }
    if (!find(argv[1], argv[2])) {
      fprintf(2, "find: No such file\n");
    }
    exit(0);
}