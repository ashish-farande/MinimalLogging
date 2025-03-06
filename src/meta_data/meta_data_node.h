#ifndef F1F261F2_B30E_4B19_83C7_A33467729E5F
#define F1F261F2_B30E_4B19_83C7_A33467729E5F

#include <utility>

#include "meta_data/meta_data.h"

struct MetaDataNode;

MetaDataNode *&head_node()
{
    static MetaDataNode *head{nullptr};
    return head;
}

struct MetaDataNode
{
    constexpr MetaDataNode(const MetaDataWithDescriptors *d) : id(gen_id()), data(d)
    {
        auto &head = head_node();
        next = std::exchange(head, this);
    }

    int64_t id;
    const MetaDataWithDescriptors *data{nullptr};
    MetaDataNode *next{nullptr};
};

template <class MetaData_, class... Args>
constexpr auto get_meta_data_ptr()
{
    static constexpr std::array type_descriptors{GetTypeDescriptor<Args>::value...};
    static constexpr MetaDataWithDescriptors meta_data{MetaData_{}(), Span{type_descriptors}};
    return &meta_data;
}

template <class F, class... Args>
inline MetaDataNode meta_data_node{get_meta_data_ptr<F, Args...>()};

#endif /* F1F261F2_B30E_4B19_83C7_A33467729E5F */
