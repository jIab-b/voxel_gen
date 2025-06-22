#include "meshing.h"
#include "world.h"
#include "raymath.h"
#include <vector>
#include <string.h>

// Helper function to get voxel type at a specific location
VoxelType get_voxel_type(World* world, int x, int y, int z) {
    Voxel v = get_voxel(world, x, y, z);
    return v.type;
}

Mesh generate_chunk_mesh(Chunk* chunk, World* world) {
    std::vector<Vector3> vertices;
    std::vector<Color> colors;

    int chunk_x_base = chunk->position.x;
    int chunk_y_base = chunk->position.y;
    int chunk_z_base = chunk->position.z;

    for (int face_axis = 0; face_axis < 3; ++face_axis) {
        int d = face_axis;
        int u = (d + 1) % 3;
        int v = (d + 2) % 3;

        int chunk_dims[] = {CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH};
        int dim_d = chunk_dims[d];
        int dim_u = chunk_dims[u];
        int dim_v = chunk_dims[v];

        int x[3] = {0};
        int q[3] = {0};
        q[d] = 1;

        std::vector<int> mask(dim_u * dim_v);

        for (x[d] = -1; x[d] < dim_d; ++x[d]) {
            int n = 0;
            for (x[v] = 0; x[v] < dim_v; ++x[v]) {
                for (x[u] = 0; x[u] < dim_u; ++x[u]) {
                    int world_pos1[3] = {chunk_x_base + x[0], chunk_y_base + x[1], chunk_z_base + x[2]};
                    int world_pos2[3] = {chunk_x_base + x[0] + q[0], chunk_y_base + x[1] + q[1], chunk_z_base + x[2] + q[2]};

                    VoxelType type1 = get_voxel_type(world, world_pos1[0], world_pos1[1], world_pos1[2]);
                    VoxelType type2 = get_voxel_type(world, world_pos2[0], world_pos2[1], world_pos2[2]);

                    bool is_face_needed = (type1 != VOXEL_TYPE_AIR && type2 == VOXEL_TYPE_AIR);
                    bool is_back_face_needed = (type1 == VOXEL_TYPE_AIR && type2 != VOXEL_TYPE_AIR);

                    mask[n++] = is_face_needed ? type1 : (is_back_face_needed ? -type2 : 0);
                }
            }

            n = 0;
            for (int j = 0; j < dim_v; ++j) {
                for (int i = 0; i < dim_u;) {
                    if (mask[n] != 0) {
                        int current_mask_val = mask[n];
                        int w, h;
                        for (w = 1; i + w < dim_u && mask[n + w] == current_mask_val; ++w);
                        
                        bool done = false;
                        for (h = 1; j + h < dim_v; ++h) {
                            for (int k = 0; k < w; ++k) {
                                if (mask[n + k + h * dim_u] != current_mask_val) {
                                    done = true;
                                    break;
                                }
                            }
                            if (done) break;
                        }

                        x[u] = i;
                        x[v] = j;

                        Vector3 du_vec = {0};
                        du_vec.x = (u == 0) ? (float)w : 0;
                        du_vec.y = (u == 1) ? (float)w : 0;
                        du_vec.z = (u == 2) ? (float)w : 0;

                        Vector3 dv_vec = {0};
                        dv_vec.x = (v == 0) ? (float)h : 0;
                        dv_vec.y = (v == 1) ? (float)h : 0;
                        dv_vec.z = (v == 2) ? (float)h : 0;

                        Vector3 p = {(float)(chunk_x_base + x[0]), (float)(chunk_y_base + x[1]), (float)(chunk_z_base + x[2])};
                        p.x += q[0];
                        p.y += q[1];
                        p.z += q[2];

                        if (current_mask_val > 0) { // Positive face
                            vertices.push_back(p);
                            vertices.push_back(Vector3Add(p, dv_vec));
                            vertices.push_back(Vector3Add(p, du_vec));
                            vertices.push_back(Vector3Add(p, du_vec));
                            vertices.push_back(Vector3Add(p, dv_vec));
                            vertices.push_back(Vector3Add(Vector3Add(p, du_vec), dv_vec));
                        } else { // Negative face
                            vertices.push_back(p);
                            vertices.push_back(Vector3Add(p, du_vec));
                            vertices.push_back(Vector3Add(p, dv_vec));
                            vertices.push_back(Vector3Add(p, dv_vec));
                            vertices.push_back(Vector3Add(p, du_vec));
                            vertices.push_back(Vector3Add(Vector3Add(p, du_vec), dv_vec));
                        }

                        VoxelType voxel_type = static_cast<VoxelType>(abs(current_mask_val));
                        Color c = WHITE;
                        if (voxel_type == VOXEL_TYPE_GRASS) c = GREEN;
                        else if (voxel_type == VOXEL_TYPE_DIRT) c = BROWN;
                        else if (voxel_type == VOXEL_TYPE_STONE) c = GRAY;
                        for(int k=0; k<6; ++k) colors.push_back(c);

                        for (int l = 0; l < h; ++l) {
                            for (int k = 0; k < w; ++k) {
                                mask[n + k + l * dim_u] = 0;
                            }
                        }
                        i += w;
                        n += w;
                    } else {
                        i++;
                        n++;
                    }
                }
            }
        }
    }

    Mesh mesh = {0};
    if (vertices.empty()) return mesh;

    mesh.vertexCount = vertices.size();
    mesh.triangleCount = vertices.size() / 3;
    mesh.vertices = (float*)malloc(vertices.size() * sizeof(Vector3));
    memcpy(mesh.vertices, vertices.data(), vertices.size() * sizeof(Vector3));
    mesh.colors = (unsigned char*)malloc(colors.size() * sizeof(Color));
    memcpy(mesh.colors, colors.data(), colors.size() * sizeof(Color));
    
    UploadMesh(&mesh, false);
    return mesh;
}