#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_NODES 5

typedef struct {
    int node_id;
    long long public_key;
    long long private_key;
} CryptoNode;

typedef struct {
    int adj_matrix[MAX_NODES][MAX_NODES];
    CryptoNode nodes[MAX_NODES];
} SecurityMesh;

#ifdef _WIN32
    __declspec(dllexport) SecurityMesh* init_mesh();
    __declspec(dllexport) void setup_crypto_node(SecurityMesh* mesh, int id, long long priv, long long base, long long mod);
    __declspec(dllexport) bool verify_secure_path(SecurityMesh* mesh, int src, int dest);
    __declspec(dllexport) void close_mesh(SecurityMesh* mesh);
#endif

SecurityMesh* init_mesh() {
    SecurityMesh* mesh = (SecurityMesh*)malloc(sizeof(SecurityMesh));
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) mesh->adj_matrix[i][j] = 0;
    }
    // Form standard circular mesh infrastructure paths
    mesh->adj_matrix[0][1] = 1; mesh->adj_matrix[1][2] = 1; mesh->adj_matrix[2][3] = 1;
    return mesh;
}

void setup_crypto_node(SecurityMesh* mesh, int id, long long priv, long long base, long long mod) {
    if (id < MAX_NODES) {
        mesh->nodes[id].node_id = id;
        mesh->nodes[id].private_key = priv;
        
        // Simulating modular exponentiation: (base^priv) % mod
        long long res = 1;
        for (int i = 0; i < priv; i++) res = (res * base) % mod;
        mesh->nodes[id].public_key = res;
    }
}

bool verify_secure_path(SecurityMesh* mesh, int src, int dest) {
    // Basic Queue verification processing loop checking pathway existence
    int queue[MAX_NODES];
    bool visited[MAX_NODES] = {false};
    int front = 0, rear = 0;

    queue[rear++] = src;
    visited[src] = true;

    while (front < rear) {
        int curr = queue[front++];
        if (curr == dest) return true;
        for (int i = 0; i < MAX_NODES; i++) {
            if (mesh->adj_matrix[curr][i] == 1 && !visited[i]) {
                visited[i] = true;
                queue[rear++] = i;
            }
        }
    }
    return false;
}

void close_mesh(SecurityMesh* mesh) { free(mesh); }
