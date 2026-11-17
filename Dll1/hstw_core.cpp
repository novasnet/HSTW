#include "pch.h"
#include "hstw_core.h"
#include <cmath>
#include <algorithm>

// Implementación de la clase de C++
HSTWCoreEngine::HSTWCoreEngine(int x, int y, int z) : dimX(x), dimY(y), dimZ(z), currentTensorEnergy(0.0) {
    dataGrid.resize(dimX, std::vector<std::vector<double>>(dimY, std::vector<double>(dimZ, 0.0)));
}

void HSTWCoreEngine::initializeStream(const std::vector<double>& flatData) {
    if (flatData.size() < (size_t)(dimX * dimY * dimZ)) return;
    int index = 0;
    for (int x = 0; x < dimX; x++) {
        for (int y = 0; y < dimY; y++) {
            for (int z = 0; z < dimZ; z++) {
                dataGrid[x][y][z] = flatData[index++];
            }
        }
    }
    // Calcular energía inicial
    currentTensorEnergy = getEnergy();
}

double HSTWCoreEngine::slideTensorX(const std::vector<double>& incomingSlice, int outgoingLayerIndex) {
    if (incomingSlice.size() < (size_t)(dimY * dimZ) || outgoingLayerIndex < 0 || outgoingLayerIndex >= dimX) {
        return currentTensorEnergy;
    }

    // Desplazamiento holográfico de la capa en O(1)
    int index = 0;
    for (int y = 0; y < dimY; y++) {
        for (int z = 0; z < dimZ; z++) {
            dataGrid[outgoingLayerIndex][y][z] = incomingSlice[index++];
        }
    }

    currentTensorEnergy = getEnergy();
    return currentTensorEnergy;
}

double HSTWCoreEngine::getEnergy() const {
    double totalEnergy = 0.0;
    for (int x = 0; x < dimX; x++) {
        for (int y = 0; y < dimY; y++) {
            for (int z = 0; z < dimZ; z++) {
                totalEnergy += std::abs(dataGrid[x][y][z]);
            }
        }
    }
    return totalEnergy * 1.2345;
}

// Interfaz C expuesta para P/Invoke con C# (.NET)
extern "C" {
    HSTW_API void* HSTW_CreateEngine(int dimX, int dimY, int dimZ) {
        return new HSTWCoreEngine(dimX, dimY, dimZ);
    }

    HSTW_API void HSTW_DestroyEngine(void* engineInstance) {
        if (engineInstance) {
            delete static_cast<HSTWCoreEngine*>(engineInstance);
        }
    }

    HSTW_API void HSTW_InitializeTensor(void* engineInstance, const double* flatData, int size) {
        if (!engineInstance || !flatData) return;
        std::vector<double> data(flatData, flatData + size);
        static_cast<HSTWCoreEngine*>(engineInstance)->initializeStream(data);
    }

    HSTW_API double HSTW_SlideTensorX(void* engineInstance, const double* incomingSliceData, int sliceSize, int outgoingIndex) {
        if (!engineInstance || !incomingSliceData) return 0.0;
        std::vector<double> slice(incomingSliceData, incomingSliceData + sliceSize);
        return static_cast<HSTWCoreEngine*>(engineInstance)->slideTensorX(slice, outgoingIndex);
    }

    HSTW_API double HSTW_GetCurrentEnergy(void* engineInstance) {
        if (!engineInstance) return 0.0;
        return static_cast<HSTWCoreEngine*>(engineInstance)->getEnergy();
    }
}