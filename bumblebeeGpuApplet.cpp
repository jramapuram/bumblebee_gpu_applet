#include <iostream>
#include <fstream>
#include <stdio.h>
#include <nvml.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <stdint.h>

struct GPUStats{
  uint32_t gpu_usage;
  uint32_t mem_usage;
  uint32_t temp;
};

inline bool fileExists(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

// ~ cat /proc/acpi/bbswitch
// 0000:01:00.0 OFF
bool isGPUActive() {
  const char* bbfile = "/proc/acpi/bbswitch";
  if (fileExists(bbfile)) {
    std::ifstream bb(bbfile);
    std::string bbtext((std::istreambuf_iterator<char>(bb)),
                       std::istreambuf_iterator<char>());
    return bbtext.find("ON") != std::string::npos;
  }
  return false;
}

int getStats(GPUStats* stats)
{
  nvmlReturn_t result;
  uint32_t temp;

  // First initialize NVML library
  result = nvmlInit();
  if (NVML_SUCCESS != result){
    return -1;
  }

  nvmlDevice_t device;

  result = nvmlDeviceGetHandleByIndex(0, &device);
  if (NVML_SUCCESS != result){
    nvmlShutdown();
    return -1;
  }

  result = nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temp);
  if (NVML_SUCCESS != result) {
    nvmlShutdown();
    return -1;
  }

  nvmlUtilization_t utilization;
  result = nvmlDeviceGetUtilizationRates(device, &utilization);
  if (NVML_SUCCESS != result) {
    nvmlShutdown();
    return -1;
  }

  result = nvmlShutdown();
  if (NVML_SUCCESS != result){
    return -1;
  }

  stats->gpu_usage = utilization.gpu;
  stats->mem_usage = utilization.memory;
  stats->temp = temp;
  return 0;
}

int main()
{
  if (isGPUActive()){
    GPUStats stats;
    int retcode = getStats(&stats);
    printf("GPU: %d%% GPU_MEM: %d%% GPU_TEMP: %dC", stats.gpu_usage,
           stats.mem_usage, stats.temp);
  }else{
    printf("GPU: OFF");
  }
}
