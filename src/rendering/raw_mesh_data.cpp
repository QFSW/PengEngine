#include "raw_mesh_data.h"

#include <utils/check.h>

using namespace rendering;

void RawMeshData::check_valid() const
{
    check(!corrupt);
}

RawMeshData RawMeshData::corrupt_data()
{
    return RawMeshData{
        .corrupt = true
    };
}
