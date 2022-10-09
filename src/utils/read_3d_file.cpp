#include "read_3d_file.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "utils/read_numbers.h"


int read_3d_file(const std::string filename, struct Data3D* data_3D) {
    // Check file extension
    size_t ext_pos = filename.find_last_of(".");
    if (ext_pos == std::string::npos) {
        std::cerr << "Error: file extension not found for filename "
            << filename << std::endl;
        return -1;
    }
    std::string ext_name = filename.substr(ext_pos + 1);
    std::ifstream file;
    file.open(filename);
    if (file.fail()) {
        return -1;
    }
    int status;
    if (ext_name == "off")
        status = read_off(file, data_3D);
    else if (ext_name == "obj")
        status = read_obj(file, data_3D);
    else {
        std::cerr << "Error: files with " << ext_name << " extension "
            << "are not handled." << std::endl;
        return -1;
    }
    file.close();
    return status;
}

int read_obj(std::istream& data, struct Data3D* obj_data) {
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
            if (data.eof())
                break;
            else {
                std::getline(data, line);
                continue;
            }
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
                    obj_data->vertices.push_back(vertex_array);
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
                            ||  face_vec[i] > obj_data->vertices.size()) {
                            read_status = -1;
                            break;
                        }
                    }
                    if (read_status == 0) {
                        for (size_t i = 0; i < face_vec.size(); ++i) {
                            face_vec[i] -=1;
                        }
                        std::copy_n(
                            face_vec.begin(), nb_params_read, face_array.begin()
                        );
                        obj_data->faces.push_back(face_array);
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
                            ||  polyline_vec[i] > obj_data->vertices.size()) {
                            read_status = -1;
                            break;
                        }
                    }
                    if (read_status == 0) {
                        obj_data->polylines.push_back(polyline_vec);
                    }
                }
                break;
            case '#':
                break;
            default:
                read_status = -1;
                break;
        }
        if (data.eof())
            break;
        std::getline(data, line);
    }
    return read_status;
}

int read_off(std::istream& data, struct Data3D* off_data) {
    std::string line;
    int nb_vertices, nb_faces;
    std::getline(data, line);
    std::istringstream iss(line);
    iss >> nb_vertices >> nb_faces;

    std::vector<float> vertex_vec;
    std::vector<int> face_vec;
    std::array<float, 4> vertex_array;
    int nb_params_read;
    int read_status = 0;
    // Vertices
    for (int i = 0; i < nb_vertices; ++i) {
        std::getline(data, line);
        vertex_vec.clear();
        read_status = get_numbers_from_line<float>(line, 3, vertex_vec, nb_params_read);
        if (nb_params_read != 3)
            read_status = -1;
        if (read_status == 0) {
            vertex_array.fill(0);
            std::copy_n(
                vertex_vec.begin(), nb_params_read, vertex_array.begin()
            );
            off_data->vertices.push_back(vertex_array);
        }
    }
    // Faces
    for (int i = 0; i < nb_faces; ++i) {
        std::getline(data, line);
        face_vec.clear();
        read_status = get_numbers_from_line<int>(line, INT16_MAX, face_vec, nb_params_read);
        if (nb_params_read != face_vec[0] + 1)
            read_status = -1;
        if (face_vec[0] != 3 || face_vec[0] != 4)
            read_status = -1;
        if (read_status == 0) {
            if (face_vec[0] == 3) {
                off_data->faces.push_back({ face_vec[1], face_vec[2], face_vec[3] });
            }
            else {
                off_data->faces.push_back({ face_vec[1], face_vec[2], face_vec[3] });
                off_data->faces.push_back({ face_vec[1], face_vec[3], face_vec[4] });
            }
        }
    }
    return read_status;
}