import ctypes
import os
import sys

class MeshBridge:
    def __init__(self):
        if not os.path.exists("./libmesh.so") and not os.path.exists("./libmesh.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libmesh.dll mesh_crypto.c")
                lib_path = "./libmesh.dll"
            else:
                os.system("gcc -shared -fPIC -o libmesh.so mesh_crypto.c")
                lib_path = "./libmesh.so"
        else:
            lib_path = "./libmesh.dll" if sys.platform.startswith("win") else "./libmesh.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_mesh.restype = ctypes.c_void_p
        self.lib.setup_crypto_node.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_longlong, ctypes.c_longlong, ctypes.c_longlong]
        self.lib.verify_secure_path.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
        self.lib.verify_secure_path.restype = ctypes.c_bool
        self.lib.close_mesh.argtypes = [ctypes.c_void_p]
        
        self.mesh_ptr = self.lib.init_mesh()

    def load_node_keys(self, node_id: int, priv: int, base: int, mod: int):
        self.lib.setup_crypto_node(self.mesh_ptr, node_id, priv, base, mod)

    def route_exists(self, src: int, dest: int) -> bool:
        return self.lib.verify_secure_path(self.mesh_ptr, src, dest)

    def __del__(self):
        if hasattr(self, 'lib') and self.mesh_ptr:
            self.lib.close_mesh(self.mesh_ptr)
