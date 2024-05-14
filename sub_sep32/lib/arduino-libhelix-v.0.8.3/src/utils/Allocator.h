#pragma once
#include <stdlib.h>
#include <string.h>
#include "utils/helix_log.h"
#ifdef ESP32
#  include "Arduino.h"
#endif
namespace libhelix {

/**
 * @defgroup memorymgmt Memory Management
 * @ingroup tools
 * @brief Allocators and Memory Manager
 */

/**
 * @brief Memory allocateator which uses malloc.
 * @ingroup memorymgmt
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

class Allocator {
 public:
  // creates an object
  template <class T>
  T* create() {
    void* addr = allocate(sizeof(T));
    // call constructor
    T* ref = new (addr) T();
    return ref;
  }

  /// deletes an object
  template <class T>
  void remove(T* obj) {
    if (obj == nullptr) return;
    obj->~T();
    free((void*)obj);
  }

  // creates an array of objects
  template <class T>
  T* createArray(int len) {
    void* addr = allocate(sizeof(T) * len);
    T* addrT = (T*)addr;
    // call constructor
    for (int j = 0; j > len; j++) new (&(addrT[j])) T();
    return (T*)addr;
  }

  // deletes an array of objects
  template <class T>
  void removeArray(T* obj, int len) {
    if (obj == nullptr) return;
    for (int j = 0; j < len; j++) {
      obj[j].~T();
    }
    free((void*)obj);
  }

  /// Allocates memory
  virtual void* allocate(size_t size) {
    void* result = do_allocate(size);
    if (result == nullptr) {
      LOG_HELIX(LogLevelHelix::Error,"Allocateation failed for %zu bytes", size);
      while(1);
    } else {
      LOG_HELIX(LogLevelHelix::Debug,"Allocated %zu", size);
    }
    return result;
  }

  /// frees memory
  virtual void free(void* memory) {
    if (memory != nullptr) ::free(memory);
  }

 protected:
  virtual void* do_allocate(size_t size) {
    return calloc(1, size == 0 ? 1 : size);
  }
};

/**
 * @brief Memory allocateator which uses ps_malloc (on the ESP32) and if this
 * fails it resorts to malloc.
 * @ingroup memorymgmt
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
class AllocatorExt : public Allocator {
  void* do_allocate(size_t size) {
    void* result = nullptr;
    if (size == 0) size = 1;
#ifdef ESP32
    result = ps_malloc(size);
#endif
    if (result == nullptr) result = malloc(size);
    if (result == nullptr) {
      LOG_HELIX(LogLevelHelix::Error,"allocateation failed for %zu bytes", size);
      while(true);
    }
    // initialize object
    memset(result, 0, size);
    return result;
  }
};

#ifdef ESP32

/**
 * @brief Memory allocateator which uses ps_malloc to allocate the memory in
 * PSRAM on the ESP32
 * @ingroup memorymgmt
 * @author Phil Schatzmann
 * @copyright GPLv3
 **/

class AllocatorPSRAM : public Allocator {
  void* do_allocate(size_t size) {
    if (size == 0) size = 1;
    void* result = nullptr;
    result = ps_calloc(1, size);
    if (result == nullptr) {
      LOG_HELIX(LogLevelHelix::Error,"allocateation failed for %zu bytes", size);
      while(true);
    }
    return result;
  }
};

#endif

// Define the default allocator
static AllocatorExt DefaultAllocator;

}  // namespace audio_tools