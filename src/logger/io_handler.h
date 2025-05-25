#ifndef C194F1CA_69EB_45E7_947E_6D8A582B3B82
#define C194F1CA_69EB_45E7_947E_6D8A582B3B82

#include <fstream>
#include <cstring>
#include <chrono>

template <typename T>
void writeToFile(std::ofstream &file, const T &arg)
{
    file.write(reinterpret_cast<const char *>(&arg), sizeof(decltype(arg)));
}

void writeToFile(std::ofstream &file, const char *arg)
{
    // FIXME: Not the safest way to get the size of the string.
    size_t size = strlen(arg);
    file.write(reinterpret_cast<const char *>(&size), sizeof(decltype(size)));
    file.write(reinterpret_cast<const char *>(arg), size);
}

void writeToFile(std::ofstream &file, const std::string &arg)
{
    size_t size = arg.size();
    file.write(reinterpret_cast<const char *>(&size), sizeof(decltype(size)));
    file.write(reinterpret_cast<const char *>(arg.c_str()), size);
}

template <typename T, typename... Args>
void writeToFile(std::ofstream &file, const T &arg, const Args &...args)
{
    writeToFile(file, arg);
    writeToFile(file, args...);
}

template <class F, class... Args>
void log(Args const &...args)
{
    // TODO: Writing to the file should be handled using RingBuffers
    std::ofstream file_out(READ_LOG_FILE_PATH.c_str(), std::ios::app | std::ios::binary);

    std::time_t cur_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    file_out.write(reinterpret_cast<char const *>(&cur_time), sizeof(cur_time));
    file_out.write(reinterpret_cast<const char *>(&meta_data::meta_data_node<F, Args...>.id), sizeof(decltype(meta_data::meta_data_node<F, Args...>.id)));
    writeToFile(file_out, args...);
    file_out.close();
}

#endif /* C194F1CA_69EB_45E7_947E_6D8A582B3B82 */
