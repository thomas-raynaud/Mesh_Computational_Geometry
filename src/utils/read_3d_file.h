#ifndef READ_3D_FILE_H
#define READ_3D_FILE_H

#include <vector>
#include <array>

struct Data3D {
    std::vector<std::array<float, 4>> vertices;
    std::vector<std::array<int, 3>> faces;
    std::vector<std::vector<int>> polylines;
};

int read_3d_file(const std::string filename, struct Data3D* objData);
int read_obj(std::istream& data, struct Data3D* obj_data);
int read_off(std::istream& data, struct Data3D* off_data);


#endif  // READ_3D_FILE_H