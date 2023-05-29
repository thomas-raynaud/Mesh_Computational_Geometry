#ifndef READ_3D_FILE_H
#define READ_3D_FILE_H

#include <vector>
#include <array>
#include <iostream>

struct Data3D {
    std::vector<std::array<float, 4>> vertices;
    std::vector<std::array<int, 3>> faces;
    std::vector<std::vector<int>> polylines;
};

void read_3d_file(const std::string &filename, struct Data3D* objData);
void read_obj(std::istream& data, struct Data3D* obj_data, const std::string &filename="");
void read_off(std::istream& data, struct Data3D* off_data, const std::string &filename="");

class Parse3DFileException : public std::exception {
public:
    Parse3DFileException(
        const std::string& filename,
        const std::string m_error,
        const int line_number = -1,
        const std::string& line_str = ""
    );
    virtual const char* what() const throw();
private:
    std::string m_error_str;
};


#endif  // READ_3D_FILE_H