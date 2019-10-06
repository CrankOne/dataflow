# ifndef H_DATAFLOW_HANDLERS_INDEX_H
# define H_DATAFLOW_HANDLERS_INDEX_H

# include "handlers/handler.hpp"
# include "parameters/parameter.tcc"

namespace dataflow {

struct HandlerDescription;

/// Singleton class representing registry for all handlers defined handlers in
/// the application.
class HandlersIndex : public std::map<std::string, HandlerDescription *> {
private:
    static HandlersIndex * _self;
public:
    /// Returns instance of the index.
    static HandlersIndex & self();
};

}  // namespace ::dataflow

# endif  // H_DATAFLOW_HANDLERS_INDEX_H

