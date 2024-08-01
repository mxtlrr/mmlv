typedef unsigned long uint64_t;
typedef unsigned int  uint32_t;
typedef struct {
  uint64_t addr;
  uint64_t bufsize;
  uint32_t ppsl;

  uint32_t height;
  uint32_t width;
} framebuffer_t;

int _start(framebuffer_t* fb){
  while(1);

  return 0;
}