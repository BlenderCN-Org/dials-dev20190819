#include <scitbx/array_family/boost_python/flex_fwd.h>
#include <annlib_adaptbx/ann_adaptor.h>

struct dummy {}; // work around gcc-3.3-darwin bug

#include <boost/python.hpp>
#include <scitbx/boost_python/utils.h>
using namespace boost::python;
using namespace annlib_adaptbx;

BOOST_PYTHON_MODULE(annlib_ext)
{
#if defined(__APPLE__) && defined(__MACH__) \
 && defined(__GNUC__) && __GNUC__ == 3 && __GNUC_MINOR__ == 3
   class_<dummy>("_dummy", no_init);
#endif
   class_<AnnAdaptor >("AnnAdaptor",
    init<af::shared<ANNcoord>,int >())
     .def(init<af::shared<ANNcoord>,int,int>((arg_("data"),arg_("dim"),arg_("k"))))
     .def("query",&AnnAdaptor::query)
     .def_readonly("nn",&AnnAdaptor::nn)
     .def_readonly("distances",&AnnAdaptor::distances)
  ;
   class_<AnnAdaptorSelfInclude >("AnnAdaptorSelfInclude",
    init<af::shared<annself_include::ANNcoord>,int >())
     .def(init<af::shared<annself_include::ANNcoord>,int,int>((arg_("data"),arg_("dim"),arg_("k"))))
     .def("query",&AnnAdaptorSelfInclude::query)
     .def_readonly("nn",&AnnAdaptorSelfInclude::nn)
     .def_readonly("distances",&AnnAdaptorSelfInclude::distances)
  ;
}
