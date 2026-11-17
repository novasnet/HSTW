import ctypes
import numpy as np
import os

# 1. Cargar la DLL compilada desde Visual Studio
# Asegúrate de colocar la ruta correcta hacia tu DLL (ej. x64/Release/HSTWEngineCpp.dll)
dll_path = "./HSTWEngineCpp.dll" 

if not os.path.exists(dll_path):
    raise FileNotFoundError(f"No se encontró la DLL en la ruta: {dll_path}. Copiala aquí o apunta a su ruta.")

hstw_lib = ctypes.CDLL(dll_path)

# 2. Definir los prototipos de las funciones exportadas de C++ (Signaturas)
hstw_lib.HSTW_CreateEngine.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
hstw_lib.HSTW_CreateEngine.restype = ctypes.c_void_p

hstw_lib.HSTW_DestroyEngine.argtypes = [ctypes.c_void_p]
hstw_lib.HSTW_DestroyEngine.restype = None

hstw_lib.HSTW_InitializeTensor.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_double), ctypes.c_int]
hstw_lib.HSTW_InitializeTensor.restype = None

hstw_lib.HSTW_SlideTensorX.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_double), ctypes.c_int, ctypes.c_int]
hstw_lib.HSTW_SlideTensorX.restype = ctypes.c_double

hstw_lib.HSTW_GetCurrentEnergy.argtypes = [ctypes.c_void_p]
hstw_lib.HSTW_GetCurrentEnergy.restype = ctypes.c_double

# 3. Inicializar el Motor Tensorial (Ej: Cubo de 3 x 3 x 3)
dim_x, dim_y, dim_z = 3, 3, 3
print(f"[*] Inicializando motor HSTW en C++ ({dim_x}x{dim_y}x{dim_z})...")
engine = hstw_lib.HSTW_CreateEngine(dim_x, dim_y, dim_z)

if not engine:
    raise RuntimeError("Error al crear la instancia del motor en C++.")

try:
    # Generar datos iniciales aleatorios con NumPy (27 elementos para 3x3x3)
    initial_data = np.random.uniform(1.0, 10.0, size=(dim_x * dim_y * dim_z)).astype(np.float64)
    
    # Enviar datos al motor de C++
    data_ptr = initial_data.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
    hstw_lib.HSTW_InitializeTensor(engine, data_ptr, len(initial_data))
    
    initial_energy = hstw_lib.HSTW_GetCurrentEnergy(engine)
    print(f"[+] Tensor inicializado con éxito. Energía inicial del hipercubo: {initial_energy:.4f}")

    # 4. Simular un flujo continuo deslizando capas en el tiempo
    print("\n[*] Iniciando simulación de flujo continuo de datos en O(1)...")
    for step in range(1, 6):
        # Crear una nueva capa entrante (tamaño dim_y * dim_z = 9)
        incoming_slice = np.random.uniform(5.0, 15.0, size=(dim_y * dim_z)).astype(np.float64)
        slice_ptr = incoming_slice.ctypes.data_as(ctypes.POINTER(ctypes.c_double))
        
        # Desplazar la ventana descartando la capa 0
        new_energy = hstw_lib.HSTW_SlideTensorX(engine, slice_ptr, len(incoming_slice), 0)
        print(f"    Paso temporal #{step} -> Energía marginal recalculada: {new_energy:.4f}")

finally:
    # 5. Limpieza de memoria nativa
    hstw_lib.HSTW_DestroyEngine(engine)
    print("\n[+] Motor destruido y memoria liberada correctamente.")