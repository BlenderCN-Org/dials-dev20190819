#include <scitbx/lbfgs_fem.hpp>

namespace scitbx {
namespace lbfgs_fem {

using namespace fem::major_types;

//C
//C     ***********************
//C     SIMPLE DRIVER FOR LBFGS
//C     ***********************
//C
//C     Example of driver for LBFGS routine, using a
//C     simple test problem. The solution point is at
//C     X=(1,...,1) and the optimal function value of 0.
//C
//C                          JORGE NOCEDAL
//C                        *** July 1990 ***
//C
void
one_pass(
  common& cmn,
  int const& iprint1)
{
  int n = fem::int0;
  int m = fem::int0;
  arr_1d<2, int> iprint(fem::fill0);
  bool diagco = fem::bool0;
  double eps = fem::double0;
  double xtol = fem::double0;
  int icall = fem::int0;
  int iflag = fem::int0;
  int j = fem::int0;
  const int ndim = 100;
  arr_1d<ndim, double> x(fem::fill0);
  double f = fem::double0;
  double t1 = fem::double0;
  double t2 = fem::double0;
  arr_1d<ndim, double> g(fem::fill0);
  arr_1d<ndim, double> diag(fem::fill0);
  const int msave = 7;
  const int nwork = ndim * (2 * msave + 1) + 2 * msave;
  arr<double> w(dimension(nwork), fem::fill0);
  //C
  //C     The driver for LBFGS must always declare LB2 as EXTERNAL
  //C
  n = 100;
  m = 5;
  iprint(1) = iprint1;
  iprint(2) = 0;
  //C
  //C     We do not wish to provide the diagonal matrices Hk0, and
  //C     therefore set DIAGCO to FALSE.
  //C
  diagco = false;
  eps = 1.0e-5;
  xtol = 1.0e-16;
  icall = 0;
  iflag = 0;
  FEM_DOSTEP(j, 1, n, 2) {
    x(j) = -1.2e0;
    x(j + 1) = 1.e0;
  }
  //C
  statement_20:
  f = 0.e0;
  FEM_DOSTEP(j, 1, n, 2) {
    t1 = 1.e0 - x(j);
    t2 = 1.e1 * (x(j + 1) - fem::pow2(x(j)));
    g(j + 1) = 2.e1 * t2;
    g(j) = -2.e0 * (x(j) * g(j + 1) + t1);
    f += fem::pow2(t1) + fem::pow2(t2);
  }
  lbfgs(cmn, n, m, x, f, g, diagco, diag, iprint, eps, xtol, w, iflag);
  if (iflag <= 0) {
    goto statement_50;
  }
  icall++;
  //C     We allow at most 2000 evaluations of F and G
  if (icall > 2000) {
    goto statement_50;
  }
  goto statement_20;
  statement_50:;
}

//C
//C     ** LAST LINE OF SIMPLE DRIVER (SDRIVE) **
//C
void
program_sdrive(
  int argc,
  char const* argv[])
{
  common cmn(argc, argv);
  blockdata_lb2(cmn);
  int passes = 1;
  //C     passes = 10000
  int iprint1 = -1;
  if (passes == 1) {
    iprint1 = 1;
  }
  int i = fem::int0;
  FEM_DO(i, 1, passes) {
    one_pass(cmn, iprint1);
  }
}

}} // namespace scitbx::lbfgs_fem

int
main(
  int argc,
  char const* argv[])
{
  return fem::main_with_catch(
    argc, argv,
    scitbx::lbfgs_fem::program_sdrive);
}