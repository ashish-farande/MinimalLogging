#ifndef C194F1CA_69EB_45E7_947E_6D8A582B3B82
#define C194F1CA_69EB_45E7_947E_6D8A582B3B82

#include <fstream>
#include <array>
#include <iostream>
#include <variant>

#include "meta_data/meta_data.h"
#include "meta_data/meta_data_node.h"
#include "meta_data/type_descriptors.h"
#include "utils.h"

template <typename T>
void writeToFile(std::ofstream &file, const T &arg)
{
    file << arg << " ";
}

template <typename T, typename... Args>
void writeToFile(std::ofstream &file, const T &arg, const Args &...args)
{
    writeToFile(file, arg);
    writeToFile(file, args...);
}

struct MetaDataSaver
{
    MetaDataSaver()
    {
        std::ofstream bin_file("metadata.dat", std::ios::binary);

        // TODO: Remove when project is complete
        // This is temporary for human readable format
        int log_lines = 0;
        std::ofstream file("metadata.txt");

        auto &temp = head_node();
        while (temp != nullptr)
        {
            bin_file.write(reinterpret_cast<const char *>(&temp->id), sizeof(decltype(temp->id)));
            bin_file.write(reinterpret_cast<const char *>(&temp->data), sizeof(decltype(temp->data)));
            for (const auto &desc : temp->data->desciprtors)
            {
                bin_file.write(reinterpret_cast<const char *>(&desc), sizeof(decltype(desc)));
            }

            // TODO: Remove when the implementation is done
            ///////////////////////////////////////////////////
            ++log_lines;
            file << temp->id << " | " << (int)temp->data->macroData.level << " | " << temp->data->macroData.file << " | " << temp->data->macroData.line << " | " << temp->data->macroData.function << " | " << temp->data->macroData.fmt_str << " | ";
            for (const auto &desc : temp->data->desciprtors)
            {
                // TODO: Update this to all the data types
                if (std::get_if<Int>(&desc))
                {
                    file << "int ";
                }
                else if (std::get_if<Float>(&desc))
                {
                    file << "float ";
                }
                else if (std::get_if<CStr>(&desc))
                {
                    file << "char* ";
                }
            }
            file << "\n";
            ///////////////////////////////////////////////////

            temp = temp->next;
        }

        std::cout << log_lines << " log lines were located in source code.\n";
    }
};


// TODO: Implement a reader
struct MetaDataReader
{
    MetaDataReader(std::string_view file_name)
    {
    }
};

#endif /* C194F1CA_69EB_45E7_947E_6D8A582B3B82 */
