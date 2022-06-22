#include "pybind11/embed.h"
#include "pybind11/pybind11.h"
#include "ecspp.h"


namespace py = pybind11;





PYBIND11_MODULE(kivii, mod) {

	
	mod.doc() = "Scripting library wrapper for the KV engine";

	mod.def("add", [](int a, int b) {return a + b; });

}