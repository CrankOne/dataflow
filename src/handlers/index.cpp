# include "handlers/index.hpp"

namespace dataflow {

HandlersIndex * HandlersIndex::_self = nullptr;

HandlersIndex &
HandlersIndex::self() {
    if( ! HandlersIndex::_self ) _self = new HandlersIndex();
    return *_self;
}

}  // namespace ::dataflow

