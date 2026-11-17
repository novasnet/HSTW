#pragma once

#include <vector>

#ifdef _WIN32
#define HSTW_API __declspec(dllexport)
#else
#define HSTW_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // Funciones de interfaz exportadas (C-Compatible)
    HSTW_API void* HSTW_CreateEngine(int dimX, int dimY, int dimZ);
    HSTW_API void HSTW_DestroyEngine(void* engineInstance);
    HSTW_API void HSTW_InitializeTensor(void* engineInstance, const double* flatData, int size);
    HSTW_API double HSTW_SlideTensorX(void* engineInstance, const double* incomingSliceData, int sliceSize, int outgoingIndex);
    HSTW_API double HSTW_GetCurrentEnergy(void* engineInstance);

#ifdef __cplusplus
}
#endif

// Clase interna de C++ que encapsula la lógica del hipercubo tensorial
class HSTWCoreEngine {
private:
    int dimX, dimY, dimZ;
    std::vector<std::vector<std::vector<double>>> dataGrid;
    double currentTensorEnergy;

public:
    HSTWCoreEngine(int x, int y, int z);
    void initializeStream(const std::vector<double>& flatData);
    double slideTensorX(const std::vector<double>& incomingSlice, int outgoingLayerIndex);
    double getEnergy() const;
};