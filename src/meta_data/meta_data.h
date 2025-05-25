#ifndef C73D5097_30ED_44E3_AEB0_4426E6452DFC
#define C73D5097_30ED_44E3_AEB0_4426E6452DFC

#include <string_view>
#include <variant>
#include <cstdint>
#include <span>
#include <array>

#include "utils/utils.h"
#include "meta_data/type_descriptors.h"

namespace meta_data
{
    enum class Level
    {
        kError,
        kWarn,
        kInfo,
        kDebug,
    };

    struct MetaData
    {
        std::string_view file{};
        std::string_view function{};
        std::string_view fmt_str{};
        int64_t line{};
        Level level;
    };

    struct MetaDataWithDescriptors
    {
        MetaData macroData;
        TypeDescriptors desciprtors;
    };
}
#endif /* C73D5097_30ED_44E3_AEB0_4426E6452DFC */
