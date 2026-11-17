using System;
using System.Runtime.InteropServices;

namespace HSTW.WebDashboard.Services
{
    public class TensorEngineService : IDisposable
    {
        private const string DllName = "HSTWEngineCpp.dll";
        private IntPtr _nativeEngine = IntPtr.Zero;

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr HSTW_CreateEngine(int dimX, int dimY, int dimZ);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void HSTW_DestroyEngine(IntPtr engineInstance);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void HSTW_InitializeTensor(IntPtr engineInstance, double[] flatData, int size);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern double HSTW_SlideTensorX(IntPtr engineInstance, double[] incomingSliceData, int sliceSize, int outgoingIndex);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern double HSTW_GetCurrentEnergy(IntPtr engineInstance);

        public TensorEngineService()
        {
            // Inicializar hipercubo 3x3x3 por defecto en C++
            _nativeEngine = HSTW_CreateEngine(3, 3, 3);
        }

        public void InitializeTensor(double[] data)
        {
            if (_nativeEngine != IntPtr.Zero)
            {
                HSTW_InitializeTensor(_nativeEngine, data, data.Length);
            }
        }

        public double SlideTensor(double[] slice, int axis)
        {
            if (_nativeEngine != IntPtr.Zero)
            {
                return HSTW_SlideTensorX(_nativeEngine, slice, slice.Length, axis);
            }
            return 0.0;
        }

        public double GetCurrentEnergy()
        {
            if (_nativeEngine != IntPtr.Zero)
            {
                return HSTW_GetCurrentEnergy(_nativeEngine);
            }
            return 0.0;
        }

        public void Dispose()
        {
            if (_nativeEngine != IntPtr.Zero)
            {
                HSTW_DestroyEngine(_nativeEngine);
                _nativeEngine = IntPtr.Zero;
            }
        }
    }
}