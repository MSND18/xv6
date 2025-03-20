// kernel/sysinfo.h
#include "types.h"
#ifndef KERNEL_SYSINFO_H
#define KERNEL_SYSINFO_H

struct sysinfo {
  uint64 freemem;  // 可用内存大小
  uint64 nproc;    // 活跃进程数
};

#endif // KERNEL_SYSINFO_H