# ifndef H_DATAFLOW_SYS_DICTIONARY_H
# define H_DATAFLOW_SYS_DICTIONARY_H

# include "parameters/parameter.tcc"

# include <string>
# include <map>

namespace dataflow {
namespace config {

class Dictionary : public AbstractParameter
                 , public std::map<std::string, AbstractParameter *> {
    // ...
};  // class Dictionary

}  // namespace ::dataflow::config
}  // namespace ::dataflow

# endif  // H_DATAFLOW_SYS_DICTIONARY_H

