// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <vtkDataSet.h>

#include <type_traits>

namespace dtk {

namespace detail {
template<typename T>
struct is_vtkdataset_impl : std::is_base_of<vtkDataSet, std::remove_pointer_t<std::decay_t<T>>>
{
};
} // namespace detail

template<typename T>
using is_vtkdataset = detail::is_vtkdataset_impl<T>;
} // namespace dtk

//
// dtkVisualizationTypeTraits.h ends here
