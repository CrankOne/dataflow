# include "handlers/handler.hpp"

void one(float, int) {
}

DATAFLOW_ADD_FUNCTION( one, one )


class One : public dataflow::AbstractHandler {
public:
    //One = delete;
    bool call( float );
};

//DATAFLOW_ADD_CLASS( one, One )
