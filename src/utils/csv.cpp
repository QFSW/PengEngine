#include "csv.h"

namespace csv
{
    std::string CsvParser<std::string>::parse_item(const std::string& item) const
    {
        return item;
    }
}
