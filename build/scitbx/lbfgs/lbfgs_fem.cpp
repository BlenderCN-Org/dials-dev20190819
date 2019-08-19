#include <scitbx/lbfgs_fem.hpp>

namespace scitbx {
namespace lbfgs_fem {

using namespace fem::major_types;

//C
//C     LAST LINE OF SUBROUTINE LBFGS
//C
void
lb1(
  common& cmn,
  arr_cref<int> iprint,
  int const& iter,
  int const& nfun,
  double const& gnorm,
  int const& n,
  int const& m,
  arr_cref<double> x,
  double const& f,
  arr_cref<double> g,
  double const& stp,
  bool const& finish)
{
  iprint(dimension(2));
  x(dimension(n));
  g(dimension(n));
  common_write write(cmn);
  // COMMON lb3
  int& mp = cmn.mp;
  //
  static const char* format_10 =
    "('*************************************************')";
  static const char* format_40 = "(' VECTOR X= ')";
  static const char* format_50 = "(6(2x,1p,d10.3))";
  static const char* format_60 = "(' GRADIENT VECTOR G= ')";
  static const char* format_70 =
    "(/,'   I   NFN',4x,'FUNC',8x,'GNORM',7x,'STEPLENGTH',/)";
  static const char* format_80 = "(2(i4,1x),3x,3(1p,d10.3,2x))";
  //C
  //C     -------------------------------------------------------------
  //C     THIS ROUTINE PRINTS MONITORING INFORMATION. THE FREQUENCY AND
  //C     AMOUNT OF OUTPUT ARE CONTROLLED BY IPRINT.
  //C     -------------------------------------------------------------
  //C
  int i = fem::int0;
  if (iter == 0) {
    write(mp, format_10);
    write(mp,
      "('  N=',i5,'   NUMBER OF CORRECTIONS=',i2,/,'       INITIAL VALUES')"),
      n, m;
    write(mp, "(' F= ',1p,d10.3,'   GNORM= ',1p,d10.3)"), f, gnorm;
    if (iprint(2) >= 1) {
      write(mp, format_40);
      {
        write_loop wloop(cmn, mp, format_50);
        FEM_DO(i, 1, n) {
          wloop, x(i);
        }
      }
      write(mp, format_60);
      {
        write_loop wloop(cmn, mp, format_50);
        FEM_DO(i, 1, n) {
          wloop, g(i);
        }
      }
    }
    write(mp, format_10);
    write(mp, format_70);
  }
  else {
    if ((iprint(1) == 0) && (iter != 1 && !finish)) {
      return;
    }
    if (iprint(1) != 0) {
      if (fem::mod(iter - 1, iprint(1)) == 0 || finish) {
        if (iprint(2) > 1 && iter > 1) {
          write(mp, format_70);
        }
        write(mp, format_80), iter, nfun, f, gnorm, stp;
      }
      else {
        return;
      }
    }
    else {
      if (iprint(2) > 1 && finish) {
        write(mp, format_70);
      }
      write(mp, format_80), iter, nfun, f, gnorm, stp;
    }
    if (iprint(2) == 2 || iprint(2) == 3) {
      if (finish) {
        write(mp, "(' FINAL POINT X= ')");
      }
      else {
        write(mp, format_40);
      }
      {
        write_loop wloop(cmn, mp, format_50);
        FEM_DO(i, 1, n) {
          wloop, x(i);
        }
      }
      if (iprint(2) == 3) {
        write(mp, format_60);
        {
          write_loop wloop(cmn, mp, format_50);
          FEM_DO(i, 1, n) {
            wloop, g(i);
          }
        }
      }
    }
    if (finish) {
      write(mp,
        "(/,' THE MINIMIZATION TERMINATED WITHOUT DETECTING ERRORS.',/,"
        "' IFLAG = 0')");
    }
  }
  //C
}

//C
//C   ----------------------------------------------------------
//C
void
daxpy(
  int const& n,
  double const& da,
  arr_cref<double> dx,
  int const& incx,
  arr_ref<double> dy,
  int const& incy)
{
  dx(dimension(1));
  dy(dimension(1));
  int ix = fem::int0;
  int iy = fem::int0;
  int i = fem::int0;
  int m = fem::int0;
  int mp1 = fem::int0;
  //C
  //C     constant times a vector plus a vector.
  //C     uses unrolled loops for increments equal to one.
  //C     jack dongarra, linpack, 3/11/78.
  //C
  if (n <= 0) {
    return;
  }
  if (da == 0.0e0) {
    return;
  }
  if (incx == 1 && incy == 1) {
    goto statement_20;
  }
  //C
  //C        code for unequal increments or equal increments
  //C          not equal to 1
  //C
  ix = 1;
  iy = 1;
  if (incx < 0) {
    ix = (-n + 1) * incx + 1;
  }
  if (incy < 0) {
    iy = (-n + 1) * incy + 1;
  }
  FEM_DO(i, 1, n) {
    dy(iy) += da * dx(ix);
    ix += incx;
    iy += incy;
  }
  return;
  //C
  //C        code for both increments equal to 1
  //C
  //C        clean-up loop
  //C
  statement_20:
  m = fem::mod(n, 4);
  if (m == 0) {
    goto statement_40;
  }
  FEM_DO(i, 1, m) {
    dy(i) += da * dx(i);
  }
  if (n < 4) {
    return;
  }
  statement_40:
  mp1 = m + 1;
  FEM_DOSTEP(i, mp1, n, 4) {
    dy(i) += da * dx(i);
    dy(i + 1) += da * dx(i + 1);
    dy(i + 2) += da * dx(i + 2);
    dy(i + 3) += da * dx(i + 3);
  }
}

//C
//C   ----------------------------------------------------------
//C
double
ddot(
  int const& n,
  arr_cref<double> dx,
  int const& incx,
  arr_cref<double> dy,
  int const& incy)
{
  double return_value = fem::double0;
  dx(dimension(1));
  dy(dimension(1));
  double dtemp = fem::double0;
  int ix = fem::int0;
  int iy = fem::int0;
  int i = fem::int0;
  int m = fem::int0;
  int mp1 = fem::int0;
  //C
  //C     forms the dot product of two vectors.
  //C     uses unrolled loops for increments equal to one.
  //C     jack dongarra, linpack, 3/11/78.
  //C
  return_value = 0.0e0;
  dtemp = 0.0e0;
  if (n <= 0) {
    return return_value;
  }
  if (incx == 1 && incy == 1) {
    goto statement_20;
  }
  //C
  //C        code for unequal increments or equal increments
  //C          not equal to 1
  //C
  ix = 1;
  iy = 1;
  if (incx < 0) {
    ix = (-n + 1) * incx + 1;
  }
  if (incy < 0) {
    iy = (-n + 1) * incy + 1;
  }
  FEM_DO(i, 1, n) {
    dtemp += dx(ix) * dy(iy);
    ix += incx;
    iy += incy;
  }
  return_value = dtemp;
  return return_value;
  //C
  //C        code for both increments equal to 1
  //C
  //C        clean-up loop
  //C
  statement_20:
  m = fem::mod(n, 5);
  if (m == 0) {
    goto statement_40;
  }
  FEM_DO(i, 1, m) {
    dtemp += dx(i) * dy(i);
  }
  if (n < 5) {
    goto statement_60;
  }
  statement_40:
  mp1 = m + 1;
  FEM_DOSTEP(i, mp1, n, 5) {
    dtemp += dx(i) * dy(i) + dx(i + 1) * dy(i + 1) + dx(i + 2) * dy(
      i + 2) + dx(i + 3) * dy(i + 3) + dx(i + 4) * dy(i + 4);
  }
  statement_60:
  return_value = dtemp;
  return return_value;
}

void
mcstep(
  double& stx,
  double& fx,
  double& dx,
  double& sty,
  double& fy,
  double& dy,
  double& stp,
  double const& fp,
  double const& dp,
  bool& brackt,
  double const& stpmin,
  double const& stpmax,
  int& info)
{
  //C
  //C     SUBROUTINE MCSTEP
  //C
  //C     THE PURPOSE OF MCSTEP IS TO COMPUTE A SAFEGUARDED STEP FOR
  //C     A LINESEARCH AND TO UPDATE AN INTERVAL OF UNCERTAINTY FOR
  //C     A MINIMIZER OF THE FUNCTION.
  //C
  //C     THE PARAMETER STX CONTAINS THE STEP WITH THE LEAST FUNCTION
  //C     VALUE. THE PARAMETER STP CONTAINS THE CURRENT STEP. IT IS
  //C     ASSUMED THAT THE DERIVATIVE AT STX IS NEGATIVE IN THE
  //C     DIRECTION OF THE STEP. IF BRACKT IS SET TRUE THEN A
  //C     MINIMIZER HAS BEEN BRACKETED IN AN INTERVAL OF UNCERTAINTY
  //C     WITH ENDPOINTS STX AND STY.
  //C
  //C     THE SUBROUTINE STATEMENT IS
  //C
  //C       SUBROUTINE MCSTEP(STX,FX,DX,STY,FY,DY,STP,FP,DP,BRACKT,
  //C                        STPMIN,STPMAX,INFO)
  //C
  //C     WHERE
  //C
  //C       STX, FX, AND DX ARE VARIABLES WHICH SPECIFY THE STEP,
  //C         THE FUNCTION, AND THE DERIVATIVE AT THE BEST STEP OBTAINED
  //C         SO FAR. THE DERIVATIVE MUST BE NEGATIVE IN THE DIRECTION
  //C         OF THE STEP, THAT IS, DX AND STP-STX MUST HAVE OPPOSITE
  //C         SIGNS. ON OUTPUT THESE PARAMETERS ARE UPDATED APPROPRIATELY.
  //C
  //C       STY, FY, AND DY ARE VARIABLES WHICH SPECIFY THE STEP,
  //C         THE FUNCTION, AND THE DERIVATIVE AT THE OTHER ENDPOINT OF
  //C         THE INTERVAL OF UNCERTAINTY. ON OUTPUT THESE PARAMETERS ARE
  //C         UPDATED APPROPRIATELY.
  //C
  //C       STP, FP, AND DP ARE VARIABLES WHICH SPECIFY THE STEP,
  //C         THE FUNCTION, AND THE DERIVATIVE AT THE CURRENT STEP.
  //C         IF BRACKT IS SET TRUE THEN ON INPUT STP MUST BE
  //C         BETWEEN STX AND STY. ON OUTPUT STP IS SET TO THE NEW STEP.
  //C
  //C       BRACKT IS A LOGICAL VARIABLE WHICH SPECIFIES IF A MINIMIZER
  //C         HAS BEEN BRACKETED. IF THE MINIMIZER HAS NOT BEEN BRACKETED
  //C         THEN ON INPUT BRACKT MUST BE SET FALSE. IF THE MINIMIZER
  //C         IS BRACKETED THEN ON OUTPUT BRACKT IS SET TRUE.
  //C
  //C       STPMIN AND STPMAX ARE INPUT VARIABLES WHICH SPECIFY LOWER
  //C         AND UPPER BOUNDS FOR THE STEP.
  //C
  //C       INFO IS AN INTEGER OUTPUT VARIABLE SET AS FOLLOWS:
  //C         IF INFO = 1,2,3,4,5, THEN THE STEP HAS BEEN COMPUTED
  //C         ACCORDING TO ONE OF THE FIVE CASES BELOW. OTHERWISE
  //C         INFO = 0, AND THIS INDICATES IMPROPER INPUT PARAMETERS.
  //C
  //C     SUBPROGRAMS CALLED
  //C
  //C       FORTRAN-SUPPLIED ... ABS,MAX,MIN,SQRT
  //C
  //C     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1983
  //C     JORGE J. MORE', DAVID J. THUENTE
  //C
  info = 0;
  //C
  //C     CHECK THE INPUT PARAMETERS FOR ERRORS.
  //C
  if ((brackt && (stp <= fem::min(stx, sty) || stp >= fem::max(stx,
      sty))) || dx * (stp - stx) >= 0.0f || stpmax < stpmin) {
    return;
  }
  //C
  //C     DETERMINE IF THE DERIVATIVES HAVE OPPOSITE SIGN.
  //C
  double sgnd = dp * (dx / fem::abs(dx));
  //C
  //C     FIRST CASE. A HIGHER FUNCTION VALUE.
  //C     THE MINIMUM IS BRACKETED. IF THE CUBIC STEP IS CLOSER
  //C     TO STX THAN THE QUADRATIC STEP, THE CUBIC STEP IS TAKEN,
  //C     ELSE THE AVERAGE OF THE CUBIC AND QUADRATIC STEPS IS TAKEN.
  //C
  bool bound = fem::bool0;
  double theta = fem::double0;
  double s = fem::double0;
  double gamma = fem::double0;
  double p = fem::double0;
  double q = fem::double0;
  double r = fem::double0;
  double stpc = fem::double0;
  double stpq = fem::double0;
  double stpf = fem::double0;
  if (fp > fx) {
    info = 1;
    bound = true;
    theta = 3 * (fx - fp) / (stp - stx) + dx + dp;
    s = fem::max(fem::abs(theta), fem::abs(dx), fem::abs(dp));
    gamma = s * fem::sqrt(fem::pow2((theta / s)) - (dx / s) * (dp / s));
    if (stp < stx) {
      gamma = -gamma;
    }
    p = (gamma - dx) + theta;
    q = ((gamma - dx) + gamma) + dp;
    r = p / q;
    stpc = stx + r * (stp - stx);
    stpq = stx + ((dx / ((fx - fp) / (stp - stx) + dx)) / 2) * (stp - stx);
    if (fem::abs(stpc - stx) < fem::abs(stpq - stx)) {
      stpf = stpc;
    }
    else {
      stpf = stpc + (stpq - stpc) / 2;
    }
    brackt = true;
    //C
    //C     SECOND CASE. A LOWER FUNCTION VALUE AND DERIVATIVES OF
    //C     OPPOSITE SIGN. THE MINIMUM IS BRACKETED. IF THE CUBIC
    //C     STEP IS CLOSER TO STX THAN THE QUADRATIC (SECANT) STEP,
    //C     THE CUBIC STEP IS TAKEN, ELSE THE QUADRATIC STEP IS TAKEN.
    //C
  }
  else if (sgnd < 0.0f) {
    info = 2;
    bound = false;
    theta = 3 * (fx - fp) / (stp - stx) + dx + dp;
    s = fem::max(fem::abs(theta), fem::abs(dx), fem::abs(dp));
    gamma = s * fem::sqrt(fem::pow2((theta / s)) - (dx / s) * (dp / s));
    if (stp > stx) {
      gamma = -gamma;
    }
    p = (gamma - dp) + theta;
    q = ((gamma - dp) + gamma) + dx;
    r = p / q;
    stpc = stp + r * (stx - stp);
    stpq = stp + (dp / (dp - dx)) * (stx - stp);
    if (fem::abs(stpc - stp) > fem::abs(stpq - stp)) {
      stpf = stpc;
    }
    else {
      stpf = stpq;
    }
    brackt = true;
    //C
    //C     THIRD CASE. A LOWER FUNCTION VALUE, DERIVATIVES OF THE
    //C     SAME SIGN, AND THE MAGNITUDE OF THE DERIVATIVE DECREASES.
    //C     THE CUBIC STEP IS ONLY USED IF THE CUBIC TENDS TO INFINITY
    //C     IN THE DIRECTION OF THE STEP OR IF THE MINIMUM OF THE CUBIC
    //C     IS BEYOND STP. OTHERWISE THE CUBIC STEP IS DEFINED TO BE
    //C     EITHER STPMIN OR STPMAX. THE QUADRATIC (SECANT) STEP IS ALSO
    //C     COMPUTED AND IF THE MINIMUM IS BRACKETED THEN THE THE STEP
    //C     CLOSEST TO STX IS TAKEN, ELSE THE STEP FARTHEST AWAY IS TAKEN.
    //C
  }
  else if (fem::abs(dp) < fem::abs(dx)) {
    info = 3;
    bound = true;
    theta = 3 * (fx - fp) / (stp - stx) + dx + dp;
    s = fem::max(fem::abs(theta), fem::abs(dx), fem::abs(dp));
    //C
    //C        THE CASE GAMMA = 0 ONLY ARISES IF THE CUBIC DOES NOT TEND
    //C        TO INFINITY IN THE DIRECTION OF THE STEP.
    //C
    gamma = s * fem::sqrt(fem::max(0.0e0, fem::pow2((theta / s)) - (
      dx / s) * (dp / s)));
    if (stp > stx) {
      gamma = -gamma;
    }
    p = (gamma - dp) + theta;
    q = (gamma + (dx - dp)) + gamma;
    r = p / q;
    if (r < 0.0f && gamma != 0.0f) {
      stpc = stp + r * (stx - stp);
    }
    else if (stp > stx) {
      stpc = stpmax;
    }
    else {
      stpc = stpmin;
    }
    stpq = stp + (dp / (dp - dx)) * (stx - stp);
    if (brackt) {
      if (fem::abs(stp - stpc) < fem::abs(stp - stpq)) {
        stpf = stpc;
      }
      else {
        stpf = stpq;
      }
    }
    else {
      if (fem::abs(stp - stpc) > fem::abs(stp - stpq)) {
        stpf = stpc;
      }
      else {
        stpf = stpq;
      }
    }
    //C
    //C     FOURTH CASE. A LOWER FUNCTION VALUE, DERIVATIVES OF THE
    //C     SAME SIGN, AND THE MAGNITUDE OF THE DERIVATIVE DOES
    //C     NOT DECREASE. IF THE MINIMUM IS NOT BRACKETED, THE STEP
    //C     IS EITHER STPMIN OR STPMAX, ELSE THE CUBIC STEP IS TAKEN.
    //C
  }
  else {
    info = 4;
    bound = false;
    if (brackt) {
      theta = 3 * (fp - fy) / (sty - stp) + dy + dp;
      s = fem::max(fem::abs(theta), fem::abs(dy), fem::abs(dp));
      gamma = s * fem::sqrt(fem::pow2((theta / s)) - (dy / s) * (dp / s));
      if (stp > sty) {
        gamma = -gamma;
      }
      p = (gamma - dp) + theta;
      q = ((gamma - dp) + gamma) + dy;
      r = p / q;
      stpc = stp + r * (sty - stp);
      stpf = stpc;
    }
    else if (stp > stx) {
      stpf = stpmax;
    }
    else {
      stpf = stpmin;
    }
  }
  //C
  //C     UPDATE THE INTERVAL OF UNCERTAINTY. THIS UPDATE DOES NOT
  //C     DEPEND ON THE NEW STEP OR THE CASE ANALYSIS ABOVE.
  //C
  if (fp > fx) {
    sty = stp;
    fy = fp;
    dy = dp;
  }
  else {
    if (sgnd < 0.0f) {
      sty = stx;
      fy = fx;
      dy = dx;
    }
    stx = stp;
    fx = fp;
    dx = dp;
  }
  //C
  //C     COMPUTE THE NEW STEP AND SAFEGUARD IT.
  //C
  stpf = fem::min(stpmax, stpf);
  stpf = fem::max(stpmin, stpf);
  stp = stpf;
  if (brackt && bound) {
    if (sty > stx) {
      stp = fem::min(stx + 0.66f * (sty - stx), stp);
    }
    else {
      stp = fem::max(stx + 0.66f * (sty - stx), stp);
    }
  }
  //C
  //C     LAST LINE OF SUBROUTINE MCSTEP.
  //C
}

struct mcsrch_save
{
  bool brackt;
  double dg;
  double dginit;
  double dgm;
  double dgtest;
  double dgx;
  double dgxm;
  double dgy;
  double dgym;
  double finit;
  double fm;
  double ftest1;
  double fx;
  double fxm;
  double fy;
  double fym;
  int infoc;
  int j;
  double p5;
  double p66;
  bool stage1;
  double stmax;
  double stmin;
  double stx;
  double sty;
  double width;
  double width1;
  double xtrapf;
  double zero;

  mcsrch_save() :
    brackt(fem::bool0),
    dg(fem::double0),
    dginit(fem::double0),
    dgm(fem::double0),
    dgtest(fem::double0),
    dgx(fem::double0),
    dgxm(fem::double0),
    dgy(fem::double0),
    dgym(fem::double0),
    finit(fem::double0),
    fm(fem::double0),
    ftest1(fem::double0),
    fx(fem::double0),
    fxm(fem::double0),
    fy(fem::double0),
    fym(fem::double0),
    infoc(fem::int0),
    j(fem::int0),
    p5(fem::double0),
    p66(fem::double0),
    stage1(fem::bool0),
    stmax(fem::double0),
    stmin(fem::double0),
    stx(fem::double0),
    sty(fem::double0),
    width(fem::double0),
    width1(fem::double0),
    xtrapf(fem::double0),
    zero(fem::double0)
  {}
};

//C    ------------------------------------------------------------------
//C
//C     **************************
//C     LINE SEARCH ROUTINE MCSRCH
//C     **************************
//C
void
mcsrch(
  common& cmn,
  int const& n,
  arr_ref<double> x,
  double const& f,
  arr_cref<double> g,
  arr_cref<double> s,
  double& stp,
  double const& ftol,
  double const& xtol,
  int const& maxfev,
  int& info,
  int& nfev,
  arr_ref<double> wa)
{
  FEM_CMN_SVE(mcsrch);
  x(dimension(n));
  g(dimension(n));
  s(dimension(n));
  wa(dimension(n));
  common_write write(cmn);
  double& gtol = cmn.gtol;
  double& stpmin = cmn.stpmin;
  double& stpmax = cmn.stpmax;
  //
  bool& brackt = sve.brackt;
  double& dg = sve.dg;
  double& dginit = sve.dginit;
  double& dgm = sve.dgm;
  double& dgtest = sve.dgtest;
  double& dgx = sve.dgx;
  double& dgxm = sve.dgxm;
  double& dgy = sve.dgy;
  double& dgym = sve.dgym;
  double& finit = sve.finit;
  double& fm = sve.fm;
  double& ftest1 = sve.ftest1;
  double& fx = sve.fx;
  double& fxm = sve.fxm;
  double& fy = sve.fy;
  double& fym = sve.fym;
  int& infoc = sve.infoc;
  int& j = sve.j;
  double& p5 = sve.p5;
  double& p66 = sve.p66;
  bool& stage1 = sve.stage1;
  double& stmax = sve.stmax;
  double& stmin = sve.stmin;
  double& stx = sve.stx;
  double& sty = sve.sty;
  double& width = sve.width;
  double& width1 = sve.width1;
  double& xtrapf = sve.xtrapf;
  double& zero = sve.zero;
  if (is_called_first_time) {
    p5 = 0.5e0;
    p66 = 0.66e0;
    xtrapf = 4.0e0;
    zero = 0.0e0;
  }
  //C
  //C                     SUBROUTINE MCSRCH
  //C
  //C     A slight modification of the subroutine CSRCH of More' and Thuente.
  //C     The changes are to allow reverse communication, and do not affect
  //C     the performance of the routine.
  //C
  //C     THE PURPOSE OF MCSRCH IS TO FIND A STEP WHICH SATISFIES
  //C     A SUFFICIENT DECREASE CONDITION AND A CURVATURE CONDITION.
  //C
  //C     AT EACH STAGE THE SUBROUTINE UPDATES AN INTERVAL OF
  //C     UNCERTAINTY WITH ENDPOINTS STX AND STY. THE INTERVAL OF
  //C     UNCERTAINTY IS INITIALLY CHOSEN SO THAT IT CONTAINS A
  //C     MINIMIZER OF THE MODIFIED FUNCTION
  //C
  //C          F(X+STP*S) - F(X) - FTOL*STP*(GRADF(X)'S).
  //C
  //C     IF A STEP IS OBTAINED FOR WHICH THE MODIFIED FUNCTION
  //C     HAS A NONPOSITIVE FUNCTION VALUE AND NONNEGATIVE DERIVATIVE,
  //C     THEN THE INTERVAL OF UNCERTAINTY IS CHOSEN SO THAT IT
  //C     CONTAINS A MINIMIZER OF F(X+STP*S).
  //C
  //C     THE ALGORITHM IS DESIGNED TO FIND A STEP WHICH SATISFIES
  //C     THE SUFFICIENT DECREASE CONDITION
  //C
  //C           F(X+STP*S) .LE. F(X) + FTOL*STP*(GRADF(X)'S),
  //C
  //C     AND THE CURVATURE CONDITION
  //C
  //C           ABS(GRADF(X+STP*S)'S)) .LE. GTOL*ABS(GRADF(X)'S).
  //C
  //C     IF FTOL IS LESS THAN GTOL AND IF, FOR EXAMPLE, THE FUNCTION
  //C     IS BOUNDED BELOW, THEN THERE IS ALWAYS A STEP WHICH SATISFIES
  //C     BOTH CONDITIONS. IF NO STEP CAN BE FOUND WHICH SATISFIES BOTH
  //C     CONDITIONS, THEN THE ALGORITHM USUALLY STOPS WHEN ROUNDING
  //C     ERRORS PREVENT FURTHER PROGRESS. IN THIS CASE STP ONLY
  //C     SATISFIES THE SUFFICIENT DECREASE CONDITION.
  //C
  //C     THE SUBROUTINE STATEMENT IS
  //C
  //C        SUBROUTINE MCSRCH(N,X,F,G,S,STP,FTOL,XTOL, MAXFEV,INFO,NFEV,WA)
  //C     WHERE
  //C
  //C       N IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER
  //C         OF VARIABLES.
  //C
  //C       X IS AN ARRAY OF LENGTH N. ON INPUT IT MUST CONTAIN THE
  //C         BASE POINT FOR THE LINE SEARCH. ON OUTPUT IT CONTAINS
  //C         X + STP*S.
  //C
  //C       F IS A VARIABLE. ON INPUT IT MUST CONTAIN THE VALUE OF F
  //C         AT X. ON OUTPUT IT CONTAINS THE VALUE OF F AT X + STP*S.
  //C
  //C       G IS AN ARRAY OF LENGTH N. ON INPUT IT MUST CONTAIN THE
  //C         GRADIENT OF F AT X. ON OUTPUT IT CONTAINS THE GRADIENT
  //C         OF F AT X + STP*S.
  //C
  //C       S IS AN INPUT ARRAY OF LENGTH N WHICH SPECIFIES THE
  //C         SEARCH DIRECTION.
  //C
  //C       STP IS A NONNEGATIVE VARIABLE. ON INPUT STP CONTAINS AN
  //C         INITIAL ESTIMATE OF A SATISFACTORY STEP. ON OUTPUT
  //C         STP CONTAINS THE FINAL ESTIMATE.
  //C
  //C       FTOL AND GTOL ARE NONNEGATIVE INPUT VARIABLES. (In this reverse
  //C         communication implementation GTOL is defined in a COMMON
  //C         statement.) TERMINATION OCCURS WHEN THE SUFFICIENT DECREASE
  //C         CONDITION AND THE DIRECTIONAL DERIVATIVE CONDITION ARE
  //C         SATISFIED.
  //C
  //C       XTOL IS A NONNEGATIVE INPUT VARIABLE. TERMINATION OCCURS
  //C         WHEN THE RELATIVE WIDTH OF THE INTERVAL OF UNCERTAINTY
  //C         IS AT MOST XTOL.
  //C
  //C       STPMIN AND STPMAX ARE NONNEGATIVE INPUT VARIABLES WHICH
  //C         SPECIFY LOWER AND UPPER BOUNDS FOR THE STEP. (In this reverse
  //C         communication implementatin they are defined in a COMMON
  //C         statement).
  //C
  //C       MAXFEV IS A POSITIVE INTEGER INPUT VARIABLE. TERMINATION
  //C         OCCURS WHEN THE NUMBER OF CALLS TO FCN IS AT LEAST
  //C         MAXFEV BY THE END OF AN ITERATION.
  //C
  //C       INFO IS AN INTEGER OUTPUT VARIABLE SET AS FOLLOWS:
  //C
  //C         INFO = 0  IMPROPER INPUT PARAMETERS.
  //C
  //C         INFO =-1  A RETURN IS MADE TO COMPUTE THE FUNCTION AND GRADIENT.
  //C
  //C         INFO = 1  THE SUFFICIENT DECREASE CONDITION AND THE
  //C                   DIRECTIONAL DERIVATIVE CONDITION HOLD.
  //C
  //C         INFO = 2  RELATIVE WIDTH OF THE INTERVAL OF UNCERTAINTY
  //C                   IS AT MOST XTOL.
  //C
  //C         INFO = 3  NUMBER OF CALLS TO FCN HAS REACHED MAXFEV.
  //C
  //C         INFO = 4  THE STEP IS AT THE LOWER BOUND STPMIN.
  //C
  //C         INFO = 5  THE STEP IS AT THE UPPER BOUND STPMAX.
  //C
  //C         INFO = 6  ROUNDING ERRORS PREVENT FURTHER PROGRESS.
  //C                   THERE MAY NOT BE A STEP WHICH SATISFIES THE
  //C                   SUFFICIENT DECREASE AND CURVATURE CONDITIONS.
  //C                   TOLERANCES MAY BE TOO SMALL.
  //C
  //C       NFEV IS AN INTEGER OUTPUT VARIABLE SET TO THE NUMBER OF
  //C         CALLS TO FCN.
  //C
  //C       WA IS A WORK ARRAY OF LENGTH N.
  //C
  //C     SUBPROGRAMS CALLED
  //C
  //C       MCSTEP
  //C
  //C       FORTRAN-SUPPLIED...ABS,MAX,MIN
  //C
  //C     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1983
  //C     JORGE J. MORE', DAVID J. THUENTE
  //C
  //C     **********
  if (info ==  - 1) {
    goto statement_45;
  }
  infoc = 1;
  //C
  //C     CHECK THE INPUT PARAMETERS FOR ERRORS.
  //C
  if (n <= 0 || stp <= zero || ftol < zero || gtol < zero ||
      xtol < zero || stpmin < zero || stpmax < stpmin ||
      maxfev <= 0) {
    return;
  }
  //C
  //C     COMPUTE THE INITIAL GRADIENT IN THE SEARCH DIRECTION
  //C     AND CHECK THAT S IS A DESCENT DIRECTION.
  //C
  dginit = zero;
  FEM_DO(j, 1, n) {
    dginit += g(j) * s(j);
  }
  if (dginit >= zero) {
    write(cmn.lp, "(/,'  THE SEARCH DIRECTION IS NOT A DESCENT DIRECTION')");
    return;
  }
  //C
  //C     INITIALIZE LOCAL VARIABLES.
  //C
  brackt = false;
  stage1 = true;
  nfev = 0;
  finit = f;
  dgtest = ftol * dginit;
  width = stpmax - stpmin;
  width1 = width / p5;
  FEM_DO(j, 1, n) {
    wa(j) = x(j);
  }
  //C
  //C     THE VARIABLES STX, FX, DGX CONTAIN THE VALUES OF THE STEP,
  //C     FUNCTION, AND DIRECTIONAL DERIVATIVE AT THE BEST STEP.
  //C     THE VARIABLES STY, FY, DGY CONTAIN THE VALUE OF THE STEP,
  //C     FUNCTION, AND DERIVATIVE AT THE OTHER ENDPOINT OF
  //C     THE INTERVAL OF UNCERTAINTY.
  //C     THE VARIABLES STP, F, DG CONTAIN THE VALUES OF THE STEP,
  //C     FUNCTION, AND DERIVATIVE AT THE CURRENT STEP.
  //C
  stx = zero;
  fx = finit;
  dgx = dginit;
  sty = zero;
  fy = finit;
  dgy = dginit;
  //C
  //C     START OF ITERATION.
  //C
  statement_30:
  //C
  //C        SET THE MINIMUM AND MAXIMUM STEPS TO CORRESPOND
  //C        TO THE PRESENT INTERVAL OF UNCERTAINTY.
  //C
  if (brackt) {
    stmin = fem::min(stx, sty);
    stmax = fem::max(stx, sty);
  }
  else {
    stmin = stx;
    stmax = stp + xtrapf * (stp - stx);
  }
  //C
  //C        FORCE THE STEP TO BE WITHIN THE BOUNDS STPMAX AND STPMIN.
  //C
  stp = fem::max(stp, stpmin);
  stp = fem::min(stp, stpmax);
  //C
  //C        IF AN UNUSUAL TERMINATION IS TO OCCUR THEN LET
  //C        STP BE THE LOWEST POINT OBTAINED SO FAR.
  //C
  if ((brackt && (stp <= stmin || stp >= stmax)) || nfev >= maxfev -
      1 || infoc == 0 || (brackt && stmax - stmin <= xtol * stmax)) {
    stp = stx;
  }
  //C
  //C        EVALUATE THE FUNCTION AND GRADIENT AT STP
  //C        AND COMPUTE THE DIRECTIONAL DERIVATIVE.
  //C        We return to main program to obtain F and G.
  //C
  FEM_DO(j, 1, n) {
    x(j) = wa(j) + stp * s(j);
  }
  info = -1;
  return;
  //C
  statement_45:
  info = 0;
  nfev++;
  dg = zero;
  FEM_DO(j, 1, n) {
    dg += g(j) * s(j);
  }
  ftest1 = finit + stp * dgtest;
  //C
  //C        TEST FOR CONVERGENCE.
  //C
  if ((brackt && (stp <= stmin || stp >= stmax)) || infoc == 0) {
    info = 6;
  }
  if (stp == stpmax && f <= ftest1 && dg <= dgtest) {
    info = 5;
  }
  if (stp == stpmin && (f > ftest1 || dg >= dgtest)) {
    info = 4;
  }
  if (nfev >= maxfev) {
    info = 3;
  }
  if (brackt && stmax - stmin <= xtol * stmax) {
    info = 2;
  }
  if (f <= ftest1 && fem::abs(dg) <= gtol * (-dginit)) {
    info = 1;
  }
  //C
  //C        CHECK FOR TERMINATION.
  //C
  if (info != 0) {
    return;
  }
  //C
  //C        IN THE FIRST STAGE WE SEEK A STEP FOR WHICH THE MODIFIED
  //C        FUNCTION HAS A NONPOSITIVE VALUE AND NONNEGATIVE DERIVATIVE.
  //C
  if (stage1 && f <= ftest1 && dg >= fem::min(ftol, gtol) * dginit) {
    stage1 = false;
  }
  //C
  //C        A MODIFIED FUNCTION IS USED TO PREDICT THE STEP ONLY IF
  //C        WE HAVE NOT OBTAINED A STEP FOR WHICH THE MODIFIED
  //C        FUNCTION HAS A NONPOSITIVE FUNCTION VALUE AND NONNEGATIVE
  //C        DERIVATIVE, AND IF A LOWER FUNCTION VALUE HAS BEEN
  //C        OBTAINED BUT THE DECREASE IS NOT SUFFICIENT.
  //C
  if (stage1 && f <= fx && f > ftest1) {
    //C
    //C           DEFINE THE MODIFIED FUNCTION AND DERIVATIVE VALUES.
    //C
    fm = f - stp * dgtest;
    fxm = fx - stx * dgtest;
    fym = fy - sty * dgtest;
    dgm = dg - dgtest;
    dgxm = dgx - dgtest;
    dgym = dgy - dgtest;
    //C
    //C           CALL CSTEP TO UPDATE THE INTERVAL OF UNCERTAINTY
    //C           AND TO COMPUTE THE NEW STEP.
    //C
    mcstep(stx, fxm, dgxm, sty, fym, dgym, stp, fm, dgm, brackt,
      stmin, stmax, infoc);
    //C
    //C           RESET THE FUNCTION AND GRADIENT VALUES FOR F.
    //C
    fx = fxm + stx * dgtest;
    fy = fym + sty * dgtest;
    dgx = dgxm + dgtest;
    dgy = dgym + dgtest;
  }
  else {
    //C
    //C           CALL MCSTEP TO UPDATE THE INTERVAL OF UNCERTAINTY
    //C           AND TO COMPUTE THE NEW STEP.
    //C
    mcstep(stx, fx, dgx, sty, fy, dgy, stp, f, dg, brackt, stmin, stmax, infoc);
  }
  //C
  //C        FORCE A SUFFICIENT DECREASE IN THE SIZE OF THE
  //C        INTERVAL OF UNCERTAINTY.
  //C
  if (brackt) {
    if (fem::abs(sty - stx) >= p66 * width1) {
      stp = stx + p5 * (sty - stx);
    }
    width1 = width;
    width = fem::abs(sty - stx);
  }
  //C
  //C        END OF ITERATION.
  //C
  goto statement_30;
  //C
  //C     LAST LINE OF SUBROUTINE MCSRCH.
  //C
}

struct lbfgs_save
{
  double beta;
  int bound;
  int cp;
  bool finish;
  double ftol;
  double gnorm;
  int i;
  int info;
  int inmc;
  int iscn;
  int ispt;
  int iter;
  int iycn;
  int iypt;
  int maxfev;
  int nfev;
  int nfun;
  int npt;
  double one;
  int point;
  double sq;
  double stp;
  double stp1;
  double xnorm;
  double yr;
  double ys;
  double yy;
  double zero;

  lbfgs_save() :
    beta(fem::double0),
    bound(fem::int0),
    cp(fem::int0),
    finish(fem::bool0),
    ftol(fem::double0),
    gnorm(fem::double0),
    i(fem::int0),
    info(fem::int0),
    inmc(fem::int0),
    iscn(fem::int0),
    ispt(fem::int0),
    iter(fem::int0),
    iycn(fem::int0),
    iypt(fem::int0),
    maxfev(fem::int0),
    nfev(fem::int0),
    nfun(fem::int0),
    npt(fem::int0),
    one(fem::double0),
    point(fem::int0),
    sq(fem::double0),
    stp(fem::double0),
    stp1(fem::double0),
    xnorm(fem::double0),
    yr(fem::double0),
    ys(fem::double0),
    yy(fem::double0),
    zero(fem::double0)
  {}
};

//C     ----------------------------------------------------------------------
//C     This file contains the LBFGS algorithm and supporting routines
//C
//C     ****************
//C     LBFGS SUBROUTINE
//C     ****************
//C
void
lbfgs(
  common& cmn,
  int const& n,
  int const& m,
  arr_ref<double> x,
  double const& f,
  arr_cref<double> g,
  int const& diagco,
  arr_ref<double> diag,
  arr_cref<int> iprint,
  double const& eps,
  double const& xtol,
  arr_ref<double> w,
  int& iflag)
{
  FEM_CMN_SVE(lbfgs);
  x(dimension(n));
  g(dimension(n));
  diag(dimension(n));
  iprint(dimension(2));
  w(dimension(n * (2 * m + 1) + 2 * m));
  common_write write(cmn);
  int& lp = cmn.lp;
  double& gtol = cmn.gtol;
  //
  double& beta = sve.beta;
  int& bound = sve.bound;
  int& cp = sve.cp;
  bool& finish = sve.finish;
  double& ftol = sve.ftol;
  double& gnorm = sve.gnorm;
  int& i = sve.i;
  int& info = sve.info;
  int& inmc = sve.inmc;
  int& iscn = sve.iscn;
  int& ispt = sve.ispt;
  int& iter = sve.iter;
  int& iycn = sve.iycn;
  int& iypt = sve.iypt;
  int& maxfev = sve.maxfev;
  int& nfev = sve.nfev;
  int& nfun = sve.nfun;
  int& npt = sve.npt;
  double& one = sve.one;
  int& point = sve.point;
  double& sq = sve.sq;
  double& stp = sve.stp;
  double& stp1 = sve.stp1;
  double& xnorm = sve.xnorm;
  double& yr = sve.yr;
  double& ys = sve.ys;
  double& yy = sve.yy;
  double& zero = sve.zero;
  if (is_called_first_time) {
    one = 1.0e+0;
    zero = 0.0e+0;
  }
  //C
  //C        LIMITED MEMORY BFGS METHOD FOR LARGE SCALE OPTIMIZATION
  //C                          JORGE NOCEDAL
  //C                        *** July 1990 ***
  //C
  //C     This subroutine solves the unconstrained minimization problem
  //C
  //C                      min F(x),    x= (x1,x2,...,xN),
  //C
  //C      using the limited memory BFGS method. The routine is especially
  //C      effective on problems involving a large number of variables. In
  //C      a typical iteration of this method an approximation Hk to the
  //C      inverse of the Hessian is obtained by applying M BFGS updates to
  //C      a diagonal matrix Hk0, using information from the previous M steps.
  //C      The user specifies the number M, which determines the amount of
  //C      storage required by the routine. The user may also provide the
  //C      diagonal matrices Hk0 if not satisfied with the default choice.
  //C      The algorithm is described in "On the limited memory BFGS method
  //C      for large scale optimization", by D. Liu and J. Nocedal,
  //C      Mathematical Programming B 45 (1989) 503-528.
  //C
  //C      The user is required to calculate the function value F and its
  //C      gradient G. In order to allow the user complete control over
  //C      these computations, reverse  communication is used. The routine
  //C      must be called repeatedly under the control of the parameter
  //C      IFLAG.
  //C
  //C      The steplength is determined at each iteration by means of the
  //C      line search routine MCVSRCH, which is a slight modification of
  //C      the routine CSRCH written by More' and Thuente.
  //C
  //C      The calling statement is
  //C
  //C          CALL LBFGS(N,M,X,F,G,DIAGCO,DIAG,IPRINT,EPS,XTOL,W,IFLAG)
  //C
  //C      where
  //C
  //C     N       is an INTEGER variable that must be set by the user to the
  //C             number of variables. It is not altered by the routine.
  //C             Restriction: N>0.
  //C
  //C     M       is an INTEGER variable that must be set by the user to
  //C             the number of corrections used in the BFGS update. It
  //C             is not altered by the routine. Values of M less than 3 are
  //C             not recommended; large values of M will result in excessive
  //C             computing time. 3<= M <=7 is recommended. Restriction: M>0.
  //C
  //C     X       is a DOUBLE PRECISION array of length N. On initial entry
  //C             it must be set by the user to the values of the initial
  //C             estimate of the solution vector. On exit with IFLAG=0, it
  //C             contains the values of the variables at the best point
  //C             found (usually a solution).
  //C
  //C     F       is a DOUBLE PRECISION variable. Before initial entry and on
  //C             a re-entry with IFLAG=1, it must be set by the user to
  //C             contain the value of the function F at the point X.
  //C
  //C     G       is a DOUBLE PRECISION array of length N. Before initial
  //C             entry and on a re-entry with IFLAG=1, it must be set by
  //C             the user to contain the components of the gradient G at
  //C             the point X.
  //C
  //C     DIAGCO  is a LOGICAL variable that must be set to .TRUE. if the
  //C             user  wishes to provide the diagonal matrix Hk0 at each
  //C             iteration. Otherwise it should be set to .FALSE., in which
  //C             case  LBFGS will use a default value described below. If
  //C             DIAGCO is set to .TRUE. the routine will return at each
  //C             iteration of the algorithm with IFLAG=2, and the diagonal
  //C              matrix Hk0  must be provided in the array DIAG.
  //C
  //C     DIAG    is a DOUBLE PRECISION array of length N. If DIAGCO=.TRUE.,
  //C             then on initial entry or on re-entry with IFLAG=2, DIAG
  //C             it must be set by the user to contain the values of the
  //C             diagonal matrix Hk0.  Restriction: all elements of DIAG
  //C             must be positive.
  //C
  //C     IPRINT  is an INTEGER array of length two which must be set by the
  //C             user.
  //C
  //C             IPRINT(1) specifies the frequency of the output:
  //C                IPRINT(1) < 0 : no output is generated,
  //C                IPRINT(1) = 0 : output only at first and last iteration,
  //C                IPRINT(1) > 0 : output every IPRINT(1) iterations.
  //C
  //C             IPRINT(2) specifies the type of output generated:
  //C                IPRINT(2) = 0 : iteration count, number of function
  //C                                evaluations, function value, norm of the
  //C                                gradient, and steplength,
  //C                IPRINT(2) = 1 : same as IPRINT(2)=0, plus vector of
  //C                                variables and  gradient vector at the
  //C                                initial point,
  //C                IPRINT(2) = 2 : same as IPRINT(2)=1, plus vector of
  //C                                variables,
  //C                IPRINT(2) = 3 : same as IPRINT(2)=2, plus gradient vector.
  //C
  //C     EPS     is a positive DOUBLE PRECISION variable that must be set by
  //C             the user, and determines the accuracy with which the solution
  //C             is to be found. The subroutine terminates when
  //C
  //C                         ||G|| < EPS max(1,||X||),
  //C
  //C             where ||.|| denotes the Euclidean norm.
  //C
  //C     XTOL    is a  positive DOUBLE PRECISION variable that must be set by
  //C             the user to an estimate of the machine precision (e.g.
  //C             10**(-16) on a SUN station 3/60). The line search routine will
  //C             terminate if the relative width of the interval of uncertainty
  //C             is less than XTOL.
  //C
  //C     W       is a DOUBLE PRECISION array of length N(2M+1)+2M used as
  //C             workspace for LBFGS. This array must not be altered by the
  //C             user.
  //C
  //C     IFLAG   is an INTEGER variable that must be set to 0 on initial entry
  //C             to the subroutine. A return with IFLAG<0 indicates an error,
  //C             and IFLAG=0 indicates that the routine has terminated without
  //C             detecting errors. On a return with IFLAG=1, the user must
  //C             evaluate the function F and gradient G. On a return with
  //C             IFLAG=2, the user must provide the diagonal matrix Hk0.
  //C
  //C             The following negative values of IFLAG, detecting an error,
  //C             are possible:
  //C
  //C              IFLAG=-1  The line search routine MCSRCH failed. The
  //C                        parameter INFO provides more detailed information
  //C                        (see also the documentation of MCSRCH):
  //C
  //C                       INFO = 0  IMPROPER INPUT PARAMETERS.
  //C
  //C                       INFO = 2  RELATIVE WIDTH OF THE INTERVAL OF
  //C                                 UNCERTAINTY IS AT MOST XTOL.
  //C
  //C                       INFO = 3  MORE THAN 20 FUNCTION EVALUATIONS WERE
  //C                                 REQUIRED AT THE PRESENT ITERATION.
  //C
  //C                       INFO = 4  THE STEP IS TOO SMALL.
  //C
  //C                       INFO = 5  THE STEP IS TOO LARGE.
  //C
  //C                       INFO = 6  ROUNDING ERRORS PREVENT FURTHER PROGRESS.
  //C                                 THERE MAY NOT BE A STEP WHICH SATISFIES
  //C                                 THE SUFFICIENT DECREASE AND CURVATURE
  //C                                 CONDITIONS. TOLERANCES MAY BE TOO SMALL.
  //C
  //C              IFLAG=-2  The i-th diagonal element of the diagonal inverse
  //C                        Hessian approximation, given in DIAG, is not
  //C                        positive.
  //C
  //C              IFLAG=-3  Improper input parameters for LBFGS (N or M are
  //C                        not positive).
  //C
  //C    ON THE DRIVER:
  //C
  //C    The program that calls LBFGS must contain the declaration:
  //C
  //C                       EXTERNAL LB2
  //C
  //C    LB2 is a BLOCK DATA that defines the default values of several
  //C    parameters described in the COMMON section.
  //C
  //C    COMMON:
  //C
  //C     The subroutine contains one common area, which the user may wish to
  //C    reference:
  //C
  //C    MP  is an INTEGER variable with default value 6. It is used as the
  //C        unit number for the printing of the monitoring information
  //C        controlled by IPRINT.
  //C
  //C    LP  is an INTEGER variable with default value 6. It is used as the
  //C        unit number for the printing of error messages. This printing
  //C        may be suppressed by setting LP to a non-positive value.
  //C
  //C    GTOL is a DOUBLE PRECISION variable with default value 0.9, which
  //C        controls the accuracy of the line search routine MCSRCH. If the
  //C        function and gradient evaluations are inexpensive with respect
  //C        to the cost of the iteration (which is sometimes the case when
  //C        solving very large problems) it may be advantageous to set GTOL
  //C        to a small value. A typical small value is 0.1.  Restriction:
  //C        GTOL should be greater than 1.D-04.
  //C
  //C    STPMIN and STPMAX are non-negative DOUBLE PRECISION variables which
  //C        specify lower and uper bounds for the step in the line search.
  //C        Their default values are 1.D-20 and 1.D+20, respectively. These
  //C        values need not be modified unless the exponents are too large
  //C        for the machine being used, or unless the problem is extremely
  //C        badly scaled (in which case the exponents should be increased).
  //C
  //C  MACHINE DEPENDENCIES
  //C
  //C        The only variables that are machine-dependent are XTOL,
  //C        STPMIN and STPMAX.
  //C
  //C  GENERAL INFORMATION
  //C
  //C    Other routines called directly:  DAXPY, DDOT, LB1, MCSRCH
  //C
  //C    Input/Output  :  No input; diagnostic messages on unit MP and
  //C                     error messages on unit LP.
  //C
  //C     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //C
  //C     INITIALIZE
  //C     ----------
  //C
  if (iflag == 0) {
    goto statement_10;
  }
  switch (iflag) {
    case 1: goto statement_172;
    case 2: goto statement_100;
    default: break;
  }
  statement_10:
  iter = 0;
  if (n <= 0 || m <= 0) {
    goto statement_196;
  }
  if (gtol <= 1.e-04) {
    if (lp > 0) {
      write(lp,
        "(/,'  GTOL IS LESS THAN OR EQUAL TO 1.D-04',/,"
        "' IT HAS BEEN RESET TO 9.D-01')");
    }
    gtol = 9.e-01;
  }
  nfun = 1;
  point = 0;
  finish = false;
  if (diagco != 0) {
    FEM_DO(i, 1, n) {
      if (diag(i) <= zero) {
        goto statement_195;
      }
    }
  }
  else {
    FEM_DO(i, 1, n) {
      diag(i) = 1.0e0;
    }
  }
  //C
  //C     THE WORK VECTOR W IS DIVIDED AS FOLLOWS:
  //C     ---------------------------------------
  //C     THE FIRST N LOCATIONS ARE USED TO STORE THE GRADIENT AND
  //C         OTHER TEMPORARY INFORMATION.
  //C     LOCATIONS (N+1)...(N+M) STORE THE SCALARS RHO.
  //C     LOCATIONS (N+M+1)...(N+2M) STORE THE NUMBERS ALPHA USED
  //C         IN THE FORMULA THAT COMPUTES H*G.
  //C     LOCATIONS (N+2M+1)...(N+2M+NM) STORE THE LAST M SEARCH
  //C         STEPS.
  //C     LOCATIONS (N+2M+NM+1)...(N+2M+2NM) STORE THE LAST M
  //C         GRADIENT DIFFERENCES.
  //C
  //C     THE SEARCH STEPS AND GRADIENT DIFFERENCES ARE STORED IN A
  //C     CIRCULAR ORDER CONTROLLED BY THE PARAMETER POINT.
  //C
  ispt = n + 2 * m;
  iypt = ispt + n * m;
  FEM_DO(i, 1, n) {
    w(ispt + i) = -g(i) * diag(i);
  }
  gnorm = fem::dsqrt(ddot(n, g, 1, g, 1));
  stp1 = one / gnorm;
  //C
  //C     PARAMETERS FOR LINE SEARCH ROUTINE
  //C
  ftol = 1.0e-4;
  maxfev = 20;
  //C
  if (iprint(1) >= 0) {
    lb1(cmn, iprint, iter, nfun, gnorm, n, m, x, f, g, stp, finish);
  }
  //C
  //C    --------------------
  //C     MAIN ITERATION LOOP
  //C    --------------------
  //C
  statement_80:
  iter++;
  info = 0;
  bound = iter - 1;
  if (iter == 1) {
    goto statement_165;
  }
  if (iter > m) {
    bound = m;
  }
  //C
  ys = ddot(n, w(iypt + npt + 1), 1, w(ispt + npt + 1), 1);
  if (diagco == 0) {
    yy = ddot(n, w(iypt + npt + 1), 1, w(iypt + npt + 1), 1);
    FEM_DO(i, 1, n) {
      diag(i) = ys / yy;
    }
  }
  else {
    iflag = 2;
    return;
  }
  statement_100:
  if (diagco != 0) {
    FEM_DO(i, 1, n) {
      if (diag(i) <= zero) {
        goto statement_195;
      }
    }
  }
  //C
  //C     COMPUTE -H*G USING THE FORMULA GIVEN IN: Nocedal, J. 1980,
  //C     "Updating quasi-Newton matrices with limited storage",
  //C     Mathematics of Computation, Vol.24, No.151, pp. 773-782.
  //C     ---------------------------------------------------------
  //C
  cp = point;
  if (point == 0) {
    cp = m;
  }
  w(n + cp) = one / ys;
  FEM_DO(i, 1, n) {
    w(i) = -g(i);
  }
  cp = point;
  FEM_DO(i, 1, bound) {
    cp = cp - 1;
    if (cp ==  - 1) {
      cp = m - 1;
    }
    sq = ddot(n, w(ispt + cp * n + 1), 1, w, 1);
    inmc = n + m + cp + 1;
    iycn = iypt + cp * n;
    w(inmc) = w(n + cp + 1) * sq;
    daxpy(n, -w(inmc), w(iycn + 1), 1, w, 1);
  }
  //C
  FEM_DO(i, 1, n) {
    w(i) = diag(i) * w(i);
  }
  //C
  FEM_DO(i, 1, bound) {
    yr = ddot(n, w(iypt + cp * n + 1), 1, w, 1);
    beta = w(n + cp + 1) * yr;
    inmc = n + m + cp + 1;
    beta = w(inmc) - beta;
    iscn = ispt + cp * n;
    daxpy(n, beta, w(iscn + 1), 1, w, 1);
    cp++;
    if (cp == m) {
      cp = 0;
    }
  }
  //C
  //C     STORE THE NEW SEARCH DIRECTION
  //C     ------------------------------
  //C
  FEM_DO(i, 1, n) {
    w(ispt + point * n + i) = w(i);
  }
  //C
  //C     OBTAIN THE ONE-DIMENSIONAL MINIMIZER OF THE FUNCTION
  //C     BY USING THE LINE SEARCH ROUTINE MCSRCH
  //C     ----------------------------------------------------
  statement_165:
  nfev = 0;
  stp = one;
  if (iter == 1) {
    stp = stp1;
  }
  FEM_DO(i, 1, n) {
    w(i) = g(i);
  }
  statement_172:
  mcsrch(cmn, n, x, f, g, w(ispt + point * n + 1), stp, ftol, xtol,
    maxfev, info, nfev, diag);
  if (info ==  - 1) {
    iflag = 1;
    return;
  }
  if (info != 1) {
    goto statement_190;
  }
  nfun += nfev;
  //C
  //C     COMPUTE THE NEW STEP AND GRADIENT CHANGE
  //C     -----------------------------------------
  //C
  npt = point * n;
  FEM_DO(i, 1, n) {
    w(ispt + npt + i) = stp * w(ispt + npt + i);
    w(iypt + npt + i) = g(i) - w(i);
  }
  point++;
  if (point == m) {
    point = 0;
  }
  //C
  //C     TERMINATION TEST
  //C     ----------------
  //C
  gnorm = fem::dsqrt(ddot(n, g, 1, g, 1));
  xnorm = fem::dsqrt(ddot(n, x, 1, x, 1));
  xnorm = fem::dmax1(1.0e0, xnorm);
  if (gnorm / xnorm <= eps) {
    finish = true;
  }
  //C
  if (iprint(1) >= 0) {
    lb1(cmn, iprint, iter, nfun, gnorm, n, m, x, f, g, stp, finish);
  }
  if (finish) {
    iflag = 0;
    return;
  }
  goto statement_80;
  //C
  //C     ------------------------------------------------------------
  //C     END OF MAIN ITERATION LOOP. ERROR EXITS.
  //C     ------------------------------------------------------------
  //C
  statement_190:
  iflag = -1;
  if (lp > 0) {
    write(lp,
      "(/,' IFLAG= -1 ',/,' LINE SEARCH FAILED. SEE',"
      "' DOCUMENTATION OF ROUTINE MCSRCH',/,' ERROR RETURN',"
      "' OF LINE SEARCH: INFO= ',i2,/,"
      "' POSSIBLE CAUSES: FUNCTION OR GRADIENT ARE INCORRECT',/,"
      "' OR INCORRECT TOLERANCES')"),
      info;
  }
  return;
  statement_195:
  iflag = -2;
  if (lp > 0) {
    write(lp,
      "(/,' IFLAG= -2',/,' THE',i5,'-TH DIAGONAL ELEMENT OF THE',/,"
      "' INVERSE HESSIAN APPROXIMATION IS NOT POSITIVE')"),
      i;
  }
  return;
  statement_196:
  iflag = -3;
  if (lp > 0) {
    write(lp,
      "(/,' IFLAG= -3',/,' IMPROPER INPUT PARAMETERS (N OR M',"
      "' ARE NOT POSITIVE)')");
  }
  //C
  //C     FORMATS
  //C     -------
  //C
}

struct blockdata_lb2_save
{
};

//C     ******
//C
//C   ----------------------------------------------------------
//C     DATA
//C   ----------------------------------------------------------
//C
void
blockdata_lb2(
  common& cmn)
{
  FEM_CMN_SVE(blockdata_lb2);
  if (is_called_first_time) {
    cmn.mp = 6;
    cmn.lp = 6;
    cmn.gtol = 9.0e-01;
    cmn.stpmin = 1.0e-20;
    cmn.stpmax = 1.0e+20;
  }
}

}} // namespace scitbx::lbfgs_fem