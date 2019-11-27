// Version: $Id: 5da4c69fc83df94948505bc1255b5441f0bfd88f $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "dtkArrayData.h"

// /////////////////////////////////////////////////////////////////

void dtkRoundUpNextPowerOfTwo(quint32& nalloc)
{
    nalloc |= nalloc >> 1;
    nalloc |= nalloc >> 2;
    nalloc |= nalloc >> 4;
    nalloc |= nalloc >> 8;
    nalloc |= nalloc >> 16;
    ++nalloc;
};

void dtkRoundUpNextPowerOfTwo(quint64& nalloc)
{
    nalloc |= nalloc >> 1;
    nalloc |= nalloc >> 2;
    nalloc |= nalloc >> 4;
    nalloc |= nalloc >> 8;
    nalloc |= nalloc >> 16;
    nalloc |= nalloc >> 32;
    ++nalloc;
};

qintptr dtkAllocMore(qintptr alloc, qintptr extra)
{
    Q_ASSERT(alloc >= 0 && extra >= 0);
    Q_ASSERT_X(alloc < (quint64(1) << 63) - (extra + 1), "dtkAllocMore", "Requested size is too large!");

    quint64 nalloc = alloc + extra;

    // Round up to next power of 2

    // Assuming container is growing, always overshoot
    //--nalloc;

    dtkRoundUpNextPowerOfTwo(nalloc);

    Q_ASSERT(nalloc > quint64(alloc + extra));

    return nalloc - extra;
}

// /////////////////////////////////////////////////////////////////

const dtkArrayData dtkArrayData::shared_null[2] = {
    { -1, 0, 0, 0, sizeof(dtkArrayData) }, // shared null
    /* zero initialized terminator */
};

static const dtkArrayData dtk_array[3] = {
    { -1, 0, 0, 0, sizeof(dtkArrayData) }, // shared empty
    {  0, 0, 0, 0, sizeof(dtkArrayData) }, // unsharable empty
    /* zero initialized terminator */
};

static const dtkArrayData& dtk_array_empty = dtk_array[0];
static const dtkArrayData& dtk_array_unsharable_empty = dtk_array[1];

// /////////////////////////////////////////////////////////////////

dtkArrayData *dtkArrayData::allocate(size_t objectSize, size_t alignment, size_t capacity, AllocationOptions options)
{
    // Alignment is a power of two
    Q_ASSERT(alignment >= Q_ALIGNOF(dtkArrayData) && !(alignment & (alignment - 1)));

    // Don't allocate empty headers
    if (!(options & RawData) && !capacity) {
#if QT_SUPPORTS(UNSHARABLE_CONTAINERS)

        if (options & Unsharable)
            return const_cast<dtkArrayData *>(&dtk_array_unsharable_empty);

#endif
        return const_cast<dtkArrayData *>(&dtk_array_empty);
    }

    size_t headerSize = sizeof(dtkArrayData);

    // Allocate extra (alignment - Q_ALIGNOF(dtkArrayData)) padding bytes so we
    // can properly align the data array. This assumes malloc is able to
    // provide appropriate alignment for the header -- as it should!
    // Padding is skipped when allocating a header for RawData.
    if (!(options & RawData))
        headerSize += (alignment - Q_ALIGNOF(dtkArrayData));

    // Allocate additional space if array is growing
    if (options & Grow)
        capacity = dtkAllocMore(qintptr(objectSize * capacity), qintptr(headerSize)) / objectSize;

    size_t allocSize = headerSize + objectSize * capacity;

    dtkArrayData *header = static_cast<dtkArrayData *>(::malloc(allocSize));

    if (header) {
        quintptr data = (quintptr(header) + sizeof(dtkArrayData) + alignment - 1) & ~(alignment - 1);

        header->ref.atomic.store(bool(!(options & Unsharable)));
        header->size = 0;
        header->alloc = capacity;
        header->capacityReserved = bool(options & CapacityReserved);
        header->offset = data - quintptr(header);
    }

    return header;
}

void dtkArrayData::deallocate(dtkArrayData *data, size_t objectSize, size_t alignment)
{
    // Alignment is a power of two
    Q_ASSERT(alignment >= Q_ALIGNOF(dtkArrayData) && !(alignment & (alignment - 1)));
    Q_UNUSED(objectSize) Q_UNUSED(alignment)

#if QT_SUPPORTS(UNSHARABLE_CONTAINERS)

    if (data == &dtk_array_unsharable_empty)
        return;

#endif
    Q_ASSERT_X(!data->ref.isStatic(), "dtkArrayData::deallocate", "Static data can not be deleted");

    ::free(data);
}

//
// dtkArrayData.cpp ends here
