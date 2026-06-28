#ifndef MESH_CRYPTO_H
#define MESH_CRYPTO_H
#include <stdbool.h>

typedef struct CryptoNode CryptoNode;
typedef struct SecurityMesh SecurityMesh;
SecurityMesh* init_mesh();
void setup_crypto_node(SecurityMesh* mesh, int id, long long priv, long long base, long long mod);
bool verify_secure_path(SecurityMesh* mesh, int src, int dest);
void close_mesh(SecurityMesh* mesh);

#endif
