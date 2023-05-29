#include "read_3d_file.h"

#include <fstream>
#include <algorithm>
#include <exception>

#include "utils/read_numbers.h"


void read_3d_file(const std::string &filename, struct Data3D* data_3D) {
    // Check file extension
    size_t ext_pos = filename.find_last_of(".");
    if (ext_pos == std::string::npos) {
        throw Parse3DFileException(filename, std::string("File extension not found in the filename."));
    }
    std::string ext_name = filename.substr(ext_pos + 1);
    std::ifstream file;
    file.open(filename);
    if (file.fail()) {
        throw(filename, "Cannot open file.");
    }
    int status;
    if (ext_name == "off")
        read_off(file, data_3D, filename);
    else if (ext_name == "obj")
        read_obj(file, data_3D, filename);
    else {
        throw Parse3DFileException(
            filename, "files with " + ext_name + 
            " extension are not handled."
        );
    }
    file.close();
}

void read_obj(std::istream& data, struct Data3D* obj_data, const std::string &filename) {
    std::vector<float> vertex_vec;
    std::vector<int> face_vec, polyline_vec;
    std::array<float, 4> vertex_array;
    std::array<int, 3> face_array;
    std::string line, line_substr;
    int line_number = 1;
    int read_status;
    int nb_params_read;
    if (data.eof())
        throw Parse3DFileException(filename, "File is empty.");
    std::getline(data, line);
    while(true) {
        // If the line is empty, go to the next one
        if (line == "" || line.find_first_not_of(' ') == std::string::npos) {
            if (data.eof())
                break;
            else {
                std::getline(data, line);
                line_number++;
                continue;
            }
        }
        switch(line[0]) {
            case 'v':
                // ignore vertex normals for now
                if (line[1] == 'n') {
                    std::cout << "[read_obj] line ignored (" << line << ")" << std::endl;
                }
                // Read vertex data
                vertex_vec.clear();
                line_substr = line.substr(line.find(' '));
                read_status = get_numbers_from_line<float>(
                    line_substr, 4, vertex_vec, nb_params_read
                );
                if (nb_params_read < 3) {
                    throw Parse3DFileException(
                        filename, "Less than 3 numbers to specify vertex's position.",
                        line_number, line
                    );
                }
                if (read_status != 0) {
                    throw Parse3DFileException(
                        filename, "Line could not be parsed.", line_number, line
                    );
                }
                vertex_array.fill(0);
                std::copy_n(
                    vertex_vec.begin(), nb_params_read, vertex_array.begin()
                );
                obj_data->vertices.push_back(vertex_array);
                break;
            case 'f':
                // Read face data
                face_vec.clear();
                line_substr = line.substr(line.find(' '));
                read_status = get_numbers_from_line<int>(
                    line_substr, 3, face_vec, nb_params_read
                );
                if (nb_params_read != 3) {
                    throw Parse3DFileException(
                        filename, std::to_string(nb_params_read)
                        + " vertices in a face is invalid: there has to be 3.",
                        line_number, line
                    );
                }
                if (read_status != 0) {
                    throw Parse3DFileException(
                        filename, "Line could not be parsed.", line_number, line
                    );
                }
                // Check that all indexed vertices exist
                for (size_t i = 0; i < 3; ++i) {
                    if (    face_vec[i] < 1
                        ||  face_vec[i] > obj_data->vertices.size()) {
                        throw Parse3DFileException(
                            filename, "The indexed vertex " +
                            std::to_string(face_vec[i]) + " does not exist.",
                            line_number, line
                        );
                    }
                }
                for (size_t i = 0; i < face_vec.size(); ++i) {
                    face_vec[i] -= 1;
                }
                std::copy_n(
                    face_vec.begin(), nb_params_read, face_array.begin()
                );
                obj_data->faces.push_back(face_array);
                break;
            case 'l':
                // Read polyline data
                polyline_vec.clear();
                line_substr = line.substr(line.find(' '));
                read_status = get_numbers_from_line<int>(
                    line_substr, INT16_MAX, polyline_vec, nb_params_read
                );
                if (nb_params_read < 2) {
                    throw Parse3DFileException(
                        filename, std::to_string(nb_params_read)
                        + " vertices for a polyline is invalid: need at least 2.",
                        line_number, line
                    );
                }
                if (read_status != 0) {
                    throw Parse3DFileException(
                        filename, "Line could not be parsed.", line_number, line
                    );
                }
                // Check that all indexed vertices exist
                for (size_t i = 0; i < nb_params_read; ++i) {
                    if (    polyline_vec[i] < 1
                        ||  polyline_vec[i] > obj_data->vertices.size()) {
                        read_status = -1;
                        break;
                    }
                }
                obj_data->polylines.push_back(polyline_vec);
                break;
            case '#':
                break;
            case 'o':
            case 'm':
            case 'u':
            case 's':
                if (
                    line.substr(0, 2).find("o ") != std::string::npos
                    || line.substr(0, 2).find("s ") != std::string::npos
                    || line.substr(0, 6).find("mtllib") != std::string::npos
                    || line.substr(0, 6).find("usemtl") != std::string::npos) {
                    std::cout << "[read_obj] line ignored (" << line << ")" << std::endl;
                    break;
                }
            default:
                throw Parse3DFileException(
                    filename, "Invalid syntax.", line_number, line
                );
                break;
        }
        if (data.eof())
            break;
        std::getline(data, line);
        line_number++;
    }
}

void read_off(std::istream& data, struct Data3D* off_data, const std::string &filename) {
    std::string line;
    int nb_vertices, nb_faces;
    std::getline(data, line);
    std::vector<int> off_params;
    int nb_params_read;
    int line_number = 1;
    int read_status = 0;
    read_status = get_numbers_from_line<int>(line, INT16_MAX, off_params, nb_params_read);
    if (read_status != 0) {
        throw Parse3DFileException(
            filename, "Line could not be parsed.", line_number, line
        );
    }
    if (nb_params_read < 2) {
        throw Parse3DFileException(
            filename, "Invalid number of arguments: there should be 2: <nb_vertices> <nb_faces>", line_number, line
        );
    }
    nb_vertices = off_params[0];
    nb_faces = off_params[1];

    std::vector<float> vertex_vec;
    std::vector<int> face_vec;
    std::array<float, 4> vertex_array;
    
    // Vertices
    off_data->vertices.reserve(nb_vertices);
    for (int i = 0; i < nb_vertices; ++i) {
        std::getline(data, line);
        vertex_vec.clear();
        read_status = get_numbers_from_line<float>(line, 3, vertex_vec, nb_params_read);
        if (nb_params_read != 3) {
            throw Parse3DFileException(
                filename, "Vertex should be defined with 3 coordinates ("
                + std::to_string(nb_params_read) + " coordinates specified here).",
                line_number, line
            );
        }
        if (read_status != 0) {
            throw Parse3DFileException(
                filename, "Line could not be parsed.", line_number, line
            );
        }
        vertex_array.fill(0);
        std::copy_n(
            vertex_vec.begin(), nb_params_read, vertex_array.begin()
        );
        off_data->vertices.push_back(vertex_array);
    }
    // Faces
    off_data->faces.reserve(nb_faces);
    for (int i = 0; i < nb_faces; ++i) {
        std::getline(data, line);
        face_vec.clear();
        read_status = get_numbers_from_line<int>(line, INT16_MAX, face_vec, nb_params_read);
        if (nb_params_read != face_vec[0] + 1)
        {
            throw Parse3DFileException(
                filename, "There should be " + std::to_string(face_vec[0])
                + " vertices in this face, but " + std::to_string(nb_params_read - 1)
                + " are specified.",
                line_number, line
            );
        }
        if (face_vec[0] != 3 && face_vec[0] != 4)
        {
            throw Parse3DFileException(
                filename, "Number of vertices in a face should be 3 or 4. Here we have "
                + std::to_string(face_vec[0]) + " vertices",
                line_number, line
            );
        }
        if (face_vec[0] == 3) {
            off_data->faces.push_back({ face_vec[1], face_vec[2], face_vec[3] });
        }
        else {
            off_data->faces.push_back({ face_vec[1], face_vec[2], face_vec[3] });
            off_data->faces.push_back({ face_vec[1], face_vec[3], face_vec[4] });
        }
    }
}

Parse3DFileException::Parse3DFileException(
        const std::string& filename,
        const std::string error_str,
        const int line_number,
        const std::string& line_str
    ) {
    m_error_str = "Error while reading 3D file " + filename + ": " + error_str;
    if (line_number != -1) {
        m_error_str += "\nLine: " + std::to_string(line_number) + "\n\"" + line_str + "\"";
    }
}

const char* Parse3DFileException::what() const throw() {
    return m_error_str.c_str();
}