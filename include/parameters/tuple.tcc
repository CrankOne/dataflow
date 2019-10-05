# ifndef H_DATAFLOW_SYS_TUPLE_H
# define H_DATAFLOW_SYS_TUPLE_H

# include "parameters/parameter.tcc"

# include <vector>

namespace dataflow {
namespace config {

class Tuple : public AbstractParameter
            , public std::vector<AbstractParameter *> {
    // ...
};  // class Dictionary

}  // namespace ::dataflow::config
}  // namespace ::dataflow

# endif  // H_DATAFLOW_SYS_TUPLE_H


