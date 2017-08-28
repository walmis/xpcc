#include <stdlib.h>

extern "C" void* _sbrk(size_t size);

extern "C"
void* malloc(size_t size) {
  return _sbrk(size);
}

extern "C"
void free(void* ptr) {

}
