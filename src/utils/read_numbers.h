#ifndef READ_NUMBERS_H
#define READ_NUMBERS_H

#include <vector>
#include <string>
#include <cmath>

template <class T>
int string_to_number(const std::string& str, T& number) {
    int pos_comma;
    int floating_part;
    bool is_positive;
    pos_comma = str.find(".");
    if (pos_comma == std::string::npos)
        pos_comma = str.find(",");
    try {
        number = std::stoi(str.substr(0, pos_comma));
    }
    catch(std::invalid_argument e)  { return -1; }
    catch(std::out_of_range e)      { return -1; }
    is_positive = str[0] != '-';
    if (pos_comma != std::string::npos) {
        try {
            floating_part = std::stoi(str.substr(pos_comma + 1, std::string::npos));
        }
        catch(std::invalid_argument e)  { return -1; }
        catch(std::out_of_range e)      { return -1; }
        int size_floating_part = str.size() - (pos_comma + 1);
        double d_floating_part = floating_part * pow(0.1, size_floating_part);
        if (number >= 0 && is_positive)
            number += d_floating_part;
        else
            number -= d_floating_part;
    }
    return 0;
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
    pos_start = line.find(' ', 0);
    pos_start = line.find_first_not_of(' ', pos_start);
    pos_end = line.find(' ', pos_start);
    while (pos_end != std::string::npos) {
        param_str = line.substr(pos_start, pos_end - pos_start);
        res = string_to_number<T>(param_str, param);
        if (res != 0 || nb_params_read >= max_nb_params)
            return -1;
        params.push_back(param);
        nb_params_read++;
        pos_start = line.find_first_not_of(' ', pos_end);
        pos_end = line.find(' ', pos_start);
    }
    param_str = line.substr(pos_start);
    res = string_to_number<T>(param_str, param);
    if (res != 0 || nb_params_read >= max_nb_params)
        return -1;
    params.push_back(param);
    nb_params_read++;
    return 0;
}

#endif  // READ_NUMBERS_H