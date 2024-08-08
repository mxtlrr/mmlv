// Yay, usermode dev time

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>
#include <dirent.h>

typedef struct {
  char name[64];           // File name
  char buf[256];           // File contents
  unsigned short ending;   // 0xFA 0xCE, signifies the ending of
                           // the files contents
} file_t;


typedef struct {
  unsigned char magic; // 0xbf
  file_t files[256];   // Every single file available on the initrd
} initrd_t;

int main(int argc, char** argv){
  if(argc == 1){
    fprintf(stderr, "usage: gen <folder>/\n");
    return 0xf;
  }
  char* dir = argv[1];
  printf("adding files in directory %s to initrd...\n", dir);

  file_t files[256];
  int counter = 0;

  struct dirent* de;
  DIR* dr = opendir(dir);

  if(!dr){
    printf("[!] Cannot open directory %s.\n", dir);
    return 0;
  }

  while((de = readdir(dr)) != NULL && (counter != 256 || counter <= 256)){
    if(strcmp(de->d_name, ".") == 0);
    else {
      if(strcmp(de->d_name, "..") == 0);
      else {
        // Read file
        char buffer[50] = { 0};
        strcat(buffer, dir);
        strcat(buffer, de->d_name);
        FILE* f = fopen(buffer, "r");
        if(!f){
          printf("Can't open %s for reading. Bye!\n", de->d_name);
          return 1;
        }

        char* buff = malloc(257);
        if(!buff){
          printf("No memory to allocate to buffer\n");
          return 2;
        }

        int z = fread(buff, 256, 1, f);
        // if(z == 0) return 4; // No bytes read!
        fclose(f);

        // Add buffer to file
        strncpy(files[counter].buf, buff, 256);
        files[counter].buf[255] = '\0'; // Null terminator

        // Add to file
        strncpy(files[counter].name, de->d_name, strlen(de->d_name));
        files[counter].ending = 0xFACE;

        printf("Added file %s to initrd!\n", de->d_name);
      }
    }
    counter++;
  }

  // Now the fun part. Let's write the initrd to disk
  FILE* fp = fopen("initrd.img", "w");
  fprintf(fp, "%c", 0xbf); // Magic data
  // Then, we write every single files data
  for(int i = 0; i < 256; i++){
    if(files[i].buf[0] == 0){
      // Nothing else to do!
      break;
    }

    // Otherwise, write the data.
    fprintf(fp, "%s%s%c%c",
          files[i].name, files[i].buf,
          (files[i].ending >> 8) & 0xff,
          (files[i].ending & 0xff));
  }
  fclose(fp);
}