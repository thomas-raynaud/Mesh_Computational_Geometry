#include "read_obj.h"

#include <fstream>
#include <algorithm>


int read_obj(const std::string filename, struct ObjData* objData) {
    std::ifstream file;
    file.open(filename);
    int status = read_obj(file, objData);
    file.close();
    return status;
}

#include <iostream>
int read_obj(std::istream& data, struct ObjData* objData) {
    std::vector<float> vertex_vec;
    std::vector<int> face_vec, polyline_vec;
    std::array<float, 4> vertex_array;
    std::array<int, 3> face_array;
    std::string line;
    int read_status = 0;
    int nb_params_read;
    if (data.eof())
        return 1;
    std::getline(data, line);
    while(read_status == 0) {
        // If the line is empty, go to the next one
        if (line == "" || line.find_first_not_of(' ') == std::string::npos) {
            std::getline(data, line);
            continue;
        }
        switch(line[0]) {
            case 'v':
                // Read vertex data
                vertex_vec.clear();
                read_status = get_numbers_from_line<float>(
                    line, 4, vertex_vec, nb_params_read
                );
                if (nb_params_read < 3)
                    read_status = -1;
                if (read_status == 0) {
                    vertex_array.fill(0);
                    std::copy_n(
                        vertex_vec.begin(), nb_params_read, vertex_array.begin()
                    );
                    objData->vertices.push_back(vertex_array);
                }
                break;
            case 'f':
                // Read face data
                face_vec.clear();
                read_status = get_numbers_from_line<int>(
                    line, 3, face_vec, nb_params_read
                );
                if (nb_params_read != 3)
                    read_status = -1;
                if (read_status == 0) {
                    // Check that all indexed vertices exist
                    for (size_t i = 0; i < 3; ++i) {
                        if (    face_vec[i] < 1
                            ||  face_vec[i] > objData->vertices.size()) {
                            read_status = -1;
                            break;
                        }
                    }
                    if (read_status == 0) {
                        std::copy_n(
                            face_vec.begin(), nb_params_read, face_array.begin()
                        );
                        objData->faces.push_back(face_array);
                    }
                }
                break;
            case 'l':
                // Read polyline data
                polyline_vec.clear();
                read_status = get_numbers_from_line<int>(
                    line, INT16_MAX, polyline_vec, nb_params_read
                );
                if (nb_params_read < 2)
                    read_status = -1;
                if (read_status == 0) {
                    // Check that all indexed vertices exist
                    for (size_t i = 0; i < nb_params_read; ++i) {
                        if (    polyline_vec[i] < 1
                            ||  polyline_vec[i] > objData->vertices.size()) {
                            read_status = -1;
                            break;
                        }
                    }
                    if (read_status == 0) {
                        objData->polylines.push_back(polyline_vec);
                    }
                }
                break;
            case '#':
                break;
            default:
                read_status = -1;
                break;
        }
        std::getline(data, line);
        if (data.eof())
            break;
    }
    return read_status;
}