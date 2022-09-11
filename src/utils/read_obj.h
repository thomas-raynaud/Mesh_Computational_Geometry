#ifndef READ_OBJ_H
#define READ_OBJ_H

#include <string>
#include <vector>
#include <array>

struct ObjData {
    std::vector<std::array<float, 4>> vertices;
    std::vector<std::array<int, 3>> faces;
    std::vector<std::vector<int>> polylines;
};

int read_obj(const std::string filename, struct ObjData* objData);
int read_obj(std::istream& data, struct ObjData* objData);


template <class T>
int number_to_string(const std::string &str, T& number) {
    int res = 0;
    try {
        number = (T)std::stof(str);
    }
    catch(std::invalid_argument e)  { res = -1; }
    catch(std::out_of_range e)      { res = -1; }
    return res;
}

template <class T>
int get_numbers_from_line(
    const std::string &line,
    const int max_nb_params,
    std::vector<T> &params,
    int &nb_params_read
) {
    std::string param_str;
    T param;
    int pos_start, pos_end;
    int res;
    nb_params_read = 0;
    pos_start = line.find(" ", 0);
    pos_start += 1;
    pos_end = line.find(" ", pos_start + 1);
    while (pos_end != std::string::npos) {
        param_str = line.substr(pos_start, pos_end - pos_start);
        res = number_to_string<T>(param_str, param);
        if (res != 0 || nb_params_read >= max_nb_params)
            return -1;
        params.push_back(param);
        nb_params_read++;
        pos_start = pos_end + 1;
        pos_end = line.find(" ", pos_start);
    }
    param_str = line.substr(pos_start);
    res = number_to_string<T>(param_str, param);
    if (res != 0 || nb_params_read >= max_nb_params)
        return -1;
    params.push_back(param);
    nb_params_read++;
    return 0;
}

#endif  // READ_OBJ_H