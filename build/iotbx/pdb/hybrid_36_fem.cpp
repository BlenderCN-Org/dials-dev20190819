#include <fem.hpp> // Fortran EMulation library of fable module

namespace placeholder_please_replace {

using namespace fem::major_types;

struct common :
  fem::common
{
  fem::cmn_sve decode_pure_sve;
  fem::cmn_sve hy36encode_sve;
  fem::cmn_sve hy36decode_sve;

  common(
    int argc,
    char const* argv[])
  :
    fem::common(argc, argv)
  {}
};

//C Fortran 77 port of the hy36encode() and hy36decode() functions in the
//C hybrid_36.py Python prototype/reference implementation.
//C See the Python script for more information.
//C
//C This file has no external dependencies.
//C To use in your project, comment out "program exercise" at the
//C bottom.
//C
//C This file is unrestricted Open Source (cctbx.sf.net).
//C Please send corrections and enhancements to cctbx@cci.lbl.gov .
//C
//C See also: http://cci.lbl.gov/hybrid_36/
//C
//C Ralf W. Grosse-Kunstleve, Feb 2007.
//C
//C This subroutine is an implementation detail.
void
encode_pure(
  str_cref digit_set,
  int const& width,
  int const& value,
  str_ref result)
{
  int digits_size = fem::int0;
  int val = fem::int0;
  int i = fem::int0;
  int j = fem::int0;
  int rest = fem::int0;
  int k = fem::int0;
  fem::str<16> buf = fem::char0;
  //C Input
  //C Output
  //C Local
  //C
  digits_size = fem::len(digit_set);
  val = value;
  i = 0;
  j = 0;
  if (val < 0) {
    j = 1;
    val = -val;
  }
  statement_1:
  rest = val / digits_size;
  k = val - rest * digits_size + 1;
  i++;
  buf(i, i) = digit_set(k, k);
  if (rest != 0) {
    val = rest;
    goto statement_1;
  }
  if (j != 0) {
    i++;
    buf(i, i) = "-";
  }
  if (i >= width) {
    k = 1;
  }
  else {
    k = width - i;
    result(1, k) = " ";
    k++;
  }
  statement_2:
  result(k, k) = buf(i, i);
  i = i - 1;
  k++;
  if (i > 0) {
    goto statement_2;
  }
}

struct decode_pure_save
{
  bool first_call;
  int iblank;
  int iminus;

  decode_pure_save() :
    first_call(fem::bool0),
    iblank(fem::int0),
    iminus(fem::int0)
  {}
};

//C
//C This subroutine is an implementation detail.
void
decode_pure(
  common& cmn,
  arr_cref<int> digits_values,
  int const& digits_size,
  str_cref s,
  int& result,
  str_ref errmsg,
  int& errmsg_len)
{
  FEM_CMN_SVE(decode_pure);
  digits_values(dim1(0, star));
  bool& first_call = sve.first_call;
  int& iblank = sve.iblank;
  int& iminus = sve.iminus;
  if (is_called_first_time) {
    first_call = true;
  }
  bool have_minus = fem::bool0;
  bool have_non_blank = fem::bool0;
  int value = fem::int0;
  int i = fem::int0;
  int si = fem::int0;
  int dv = fem::int0;
  //C Input
  //C Output
  //C Local
  //C
  if (first_call) {
    first_call = false;
    iblank = fem::ichar(" ");
    iminus = fem::ichar("-");
  }
  have_minus = false;
  have_non_blank = false;
  value = 0;
  {
    int fem_do_last = fem::len(s);
    FEM_DO(i, 1, fem_do_last) {
      si = fem::ichar(s(i, i));
      if (si < 0 || si > 127) {
        goto statement_2;
      }
      if (si == iblank) {
        if (!have_non_blank) {
          goto statement_1;
        }
        value = value * digits_size;
      }
      else if (si == iminus) {
        if (have_non_blank) {
          goto statement_2;
        }
        have_non_blank = true;
        have_minus = true;
        goto statement_1;
      }
      else {
        have_non_blank = true;
        dv = digits_values(si);
        if (dv < 0 || dv >= digits_size) {
          goto statement_2;
        }
        value = value * digits_size;
        value += dv;
      }
      statement_1:;
    }
  }
  if (have_minus) {
    value = -value;
  }
  result = value;
  errmsg_len = 0;
  return;
  statement_2:
  result = 0;
  errmsg = "invalid number literal.";
  errmsg_len = 23;
}

struct hy36encode_save
{
  fem::str<36> digits_lower;
  fem::str<36> digits_upper;

  hy36encode_save() :
    digits_lower(fem::char0),
    digits_upper(fem::char0)
  {}
};

//C
//C hybrid-36 encoder: converts integer value to string result
//C
//C   width: must be 4 (e.g. for residue sequence numbers)
//C               or 5 (e.g. for atom serial numbers)
//C
//C   value: integer value to be converted
//C
//C   result: string holding the result (len(result) must be >= width)
//C           result(width+1:len(result)) is NOT modified in order
//C           to maximize runtime performance!
//C
//C   errmsg: string holding error message, if any
//C           len(errmsg) should be >= 80
//C           errmsg is not modified if errmsg_len below is 0
//C           DO NOT use if (errmsg .eq. ' ') to check for errors!
//C           It is not supported because it would be too slow.
//C
//C   errmsg_len: length of error message, or 0 on success
//C               use if (errmsg_len .ne. 0) to check for errors
//C
void
hy36encode(
  common& cmn,
  int const& width,
  int const& value,
  str_ref result,
  str_ref errmsg,
  int& errmsg_len)
{
  FEM_CMN_SVE(hy36encode);
  fem::str<36>& digits_lower = sve.digits_lower;
  fem::str<36>& digits_upper = sve.digits_upper;
  if (is_called_first_time) {
    digits_upper = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    digits_lower = "0123456789abcdefghijklmnopqrstuvwxyz";
  }
  int i = fem::int0;
  //C Input
  //C Output
  //C Local
  //C
  i = value;
  if (width == 4) {
    if (i >=  - 999) {
      if (i < 10000) {
        encode_pure(digits_upper(1, 10), 4, i, result);
        errmsg_len = 0;
        return;
      }
      i = i - 10000;
      if (i < 1213056) {
        i += 466560;
        encode_pure(digits_upper, 0, i, result);
        errmsg_len = 0;
        return;
      }
      i = i - 1213056;
      if (i < 1213056) {
        i += 466560;
        encode_pure(digits_lower, 0, i, result);
        errmsg_len = 0;
        return;
      }
    }
  }
  else if (width == 5) {
    if (i >=  - 9999) {
      if (i < 100000) {
        encode_pure(digits_upper(1, 10), 5, i, result);
        errmsg_len = 0;
        return;
      }
      i = i - 100000;
      if (i < 43670016) {
        i += 16796160;
        encode_pure(digits_upper, 0, i, result);
        errmsg_len = 0;
        return;
      }
      i = i - 43670016;
      if (i < 43670016) {
        i += 16796160;
        encode_pure(digits_lower, 0, i, result);
        errmsg_len = 0;
        return;
      }
    }
  }
  else {
    errmsg = "unsupported width.";
    errmsg_len = 18;
    goto statement_1;
  }
  errmsg = "value out of range.";
  errmsg_len = 19;
  statement_1:
  {
    int fem_do_last = fem::min(fem::max(1, width), fem::len(result));
    FEM_DO(i, 1, fem_do_last) {
      result(i, i) = "*";
    }
  }
}

struct hy36decode_save
{
  fem::str<36> digits_lower;
  fem::str<36> digits_upper;
  arr<int> digits_values_lower;
  arr<int> digits_values_upper;
  bool first_call;
  fem::str<57> ie_range;

  hy36decode_save() :
    digits_lower(fem::char0),
    digits_upper(fem::char0),
    digits_values_lower(dim1(0, 127), fem::fill0),
    digits_values_upper(dim1(0, 127), fem::fill0),
    first_call(fem::bool0),
    ie_range(fem::char0)
  {}
};

//C
//C hybrid-36 decoder: converts string s to integer result
//C
//C   width: must be 4 (e.g. for residue sequence numbers)
//C               or 5 (e.g. for atom serial numbers)
//C
//C   s: string to be converted
//C        len(s) must be equal to width, or an error message is
//C        returned otherwise
//C
//C   result: integer holding the conversion result
//C
//C   errmsg, errmsg_len: see hy36encode documentation above
//C
void
hy36decode(
  common& cmn,
  int const& width,
  str_cref s,
  int& result,
  str_ref errmsg,
  int& errmsg_len)
{
  FEM_CMN_SVE(hy36decode);
  fem::str<36>& digits_lower = sve.digits_lower;
  fem::str<36>& digits_upper = sve.digits_upper;
  arr_ref<int> digits_values_lower(sve.digits_values_lower, dim1(0, 127));
  arr_ref<int> digits_values_upper(sve.digits_values_upper, dim1(0, 127));
  bool& first_call = sve.first_call;
  fem::str<57>& ie_range = sve.ie_range;
  if (is_called_first_time) {
    digits_upper = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    digits_lower = "0123456789abcdefghijklmnopqrstuvwxyz";
    first_call = true;
    fem::data((values, 128*datum(-1))), digits_values_upper;
    fem::data((values, 128*datum(-1))), digits_values_lower;
    ie_range = "internal error hy36decode: integer value out of range.";
  }
  int i = fem::int0;
  int di = fem::int0;
  //C Input
  //C Output
  //C Local
  //C
  if (first_call) {
    first_call = false;
    FEM_DO(i, 1, 36) {
      di = fem::ichar(digits_upper(i, i));
      if (di < 0 || di > 127) {
        result = 0;
        errmsg = ie_range;
        errmsg_len = fem::len(ie_range);
        return;
      }
      digits_values_upper(di) = i - 1;
    }
    FEM_DO(i, 1, 36) {
      di = fem::ichar(digits_lower(i, i));
      if (di < 0 || di > 127) {
        result = 0;
        errmsg = ie_range;
        errmsg_len = fem::len(ie_range);
        return;
      }
      digits_values_lower(di) = i - 1;
    }
  }
  if (fem::len(s) == width) {
    di = fem::ichar(s(1, 1));
    if (di >= 0 && di <= 127) {
      if (digits_values_upper(di) >= 10) {
        decode_pure(cmn, digits_values_upper, 36, s, result, errmsg,
          errmsg_len);
        if (errmsg_len != 0) {
          return;
        }
        if (width == 4) {
          //C                             - 10*36**(width-1) + 10**width
          result = result - 456560;
          errmsg_len = 0;
        }
        else if (width == 5) {
          result = result - 16696160;
          errmsg_len = 0;
        }
        else {
          goto statement_3;
        }
        return;
      }
      else if (digits_values_lower(di) >= 10) {
        decode_pure(cmn, digits_values_lower, 36, s, result, errmsg,
          errmsg_len);
        if (errmsg_len != 0) {
          return;
        }
        if (width == 4) {
          //C                             + 16*36**(width-1) + 10**width
          result += 756496;
          errmsg_len = 0;
        }
        else if (width == 5) {
          result += 26973856;
          errmsg_len = 0;
        }
        else {
          goto statement_3;
        }
        return;
      }
      else {
        decode_pure(cmn, digits_values_upper, 10, s, result, errmsg,
          errmsg_len);
        if (!(width == 4 || width == 5)) {
          goto statement_3;
        }
        return;
      }
    }
  }
  statement_3:
  result = 0;
  errmsg = "unsupported width.";
  errmsg_len = 18;
}

//C
//C Unit tests for hy36encode and hy36decode
//C
//C   Can be safely ignored, but to guard yourself against
//C   compiler bugs add
//C
//C     call tst_hybrid_36_f(.true.)
//C
//C   to your main program. The runtime in quick mode is in the
//C   range of 0.01 seconds or less.
//C
void
tst_hybrid_36_f(
  common& cmn,
  bool const& quick)
{
  //C Input
  //C Local
  //C
  int decoded = fem::int0;
  fem::str<80> errmsg = fem::char0;
  int errmsg_len = fem::int0;
  hy36decode(cmn, 4, "    ", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode empty");
  }
  if (decoded != 0) {
    FEM_STOP("error decoded value empty");
  }
  hy36decode(cmn, 4, "  -0", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode empty");
  }
  if (decoded != 0) {
    FEM_STOP("error decoded value -0");
  }
  hy36decode(cmn, 4, "-999", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode -999");
  }
  if (decoded !=  - 999) {
    FEM_STOP("error decoded value -999");
  }
  hy36decode(cmn, 4, "9999", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode 9999");
  }
  if (decoded != 9999) {
    FEM_STOP("error decoded value 9999");
  }
  hy36decode(cmn, 4, "A000", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode A000");
  }
  if (decoded != 10000) {
    FEM_STOP("error decoded value A000");
  }
  hy36decode(cmn, 4, "ZZZZ", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode ZZZZ");
  }
  if (decoded != 1223055) {
    FEM_STOP("error decoded value ZZZZ");
  }
  hy36decode(cmn, 4, "a000", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode a000");
  }
  if (decoded != 1223056) {
    FEM_STOP("error decoded value a000");
  }
  hy36decode(cmn, 4, "zzzz", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode zzzz");
  }
  if (decoded != 2436111) {
    FEM_STOP("error decoded value zzzz");
  }
  //C
  hy36decode(cmn, 5, "     ", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode empty");
  }
  if (decoded != 0) {
    FEM_STOP("error decoded value empty");
  }
  hy36decode(cmn, 5, "   -0", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode empty");
  }
  if (decoded != 0) {
    FEM_STOP("error decoded value -0");
  }
  hy36decode(cmn, 5, "-9999", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode -9999");
  }
  if (decoded !=  - 9999) {
    FEM_STOP("error decoded value -9999");
  }
  hy36decode(cmn, 5, "99999", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode 99999");
  }
  if (decoded != 99999) {
    FEM_STOP("error decoded value 99999");
  }
  hy36decode(cmn, 5, "A0000", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode A0000");
  }
  if (decoded != 100000) {
    FEM_STOP("error decoded value A0000");
  }
  hy36decode(cmn, 5, "ZZZZZ", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode ZZZZZ");
  }
  if (decoded != 43770015) {
    FEM_STOP("error decoded value ZZZZZ");
  }
  hy36decode(cmn, 5, "a0000", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode a0000");
  }
  if (decoded != 43770016) {
    FEM_STOP("error decoded value a0000");
  }
  hy36decode(cmn, 5, "zzzzz", decoded, errmsg, errmsg_len);
  if (errmsg_len != 0) {
    FEM_STOP("error hy36decode zzzzz");
  }
  if (decoded != 87440031) {
    FEM_STOP("error decoded value zzzzz");
  }
  //C
  int value = fem::int0;
  fem::str<4> s4 = fem::char0;
  fem::str<5> s5 = fem::char0;
  if (!quick) {
    FEM_DO(value, -999, 2436111) {
      errmsg_len = -1;
      hy36encode(cmn, 4, value, s4, errmsg, errmsg_len);
      if (errmsg_len != 0) {
        FEM_STOP("error hy36encode");
      }
      decoded = -1000;
      errmsg_len = -1;
      hy36decode(cmn, 4, s4, decoded, errmsg, errmsg_len);
      if (errmsg_len != 0) {
        FEM_STOP("error hy36decode");
      }
      if (decoded != value) {
        FEM_STOP("error decoded value");
      }
    }
    //C
    FEM_DO(value, -9999, 110000) {
      errmsg_len = -1;
      hy36encode(cmn, 5, value, s5, errmsg, errmsg_len);
      if (errmsg_len != 0) {
        FEM_STOP("error hy36encode");
      }
      decoded = -10000;
      errmsg_len = -1;
      hy36decode(cmn, 5, s5, decoded, errmsg, errmsg_len);
      if (errmsg_len != 0) {
        FEM_STOP("error hy36decode");
      }
      if (decoded != value) {
        FEM_STOP("error decoded value");
      }
    }
  }
  //C
  s4 = " ";
  hy36encode(cmn, 4, -1000, s4, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36encode range");
  }
  if (errmsg != "value out of range.") {
    FEM_STOP("error errmsg range");
  }
  if (s4 != "****") {
    FEM_STOP("no stars range");
  }
  s4 = " ";
  hy36encode(cmn, 4, 2436112, s4, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36encode range");
  }
  if (errmsg != "value out of range.") {
    FEM_STOP("error errmsg range");
  }
  if (s4 != "****") {
    FEM_STOP("no stars range");
  }
  decoded = -1;
  hy36decode(cmn, 4, " abc", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode invalid");
  }
  if (errmsg != "invalid number literal.") {
    FEM_STOP("error errmsg invalid");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  decoded = -1;
  hy36decode(cmn, 4, "abc-", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode invalid");
  }
  if (errmsg != "invalid number literal.") {
    FEM_STOP("error errmsg invalid");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  decoded = -1;
  hy36decode(cmn, 4, "A=BC", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode invalid");
  }
  if (errmsg != "invalid number literal.") {
    FEM_STOP("error errmsg invalid");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  decoded = -1;
  hy36decode(cmn, 4, "40a0", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode invalid");
  }
  if (errmsg != "invalid number literal.") {
    FEM_STOP("error errmsg invalid");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  decoded = -1;
  hy36decode(cmn, 4, "40A0", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode invalid");
  }
  if (errmsg != "invalid number literal.") {
    FEM_STOP("error errmsg invalid");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  //C
  s5 = " ";
  hy36encode(cmn, 5, -10000, s5, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36encode range");
  }
  if (errmsg != "value out of range.") {
    FEM_STOP("error errmsg range");
  }
  if (s5 != "*****") {
    FEM_STOP("no stars range");
  }
  s5 = " ";
  hy36encode(cmn, 5, 87440032, s5, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36encode range");
  }
  if (errmsg != "value out of range.") {
    FEM_STOP("error errmsg range");
  }
  if (s5 != "*****") {
    FEM_STOP("no stars range");
  }
  decoded = -1;
  hy36decode(cmn, 5, " abcd", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode invalid");
  }
  if (errmsg != "invalid number literal.") {
    FEM_STOP("error errmsg invalid");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  decoded = -1;
  hy36decode(cmn, 5, "ABCD-", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode invalid");
  }
  if (errmsg != "invalid number literal.") {
    FEM_STOP("error errmsg invalid");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  decoded = -1;
  hy36decode(cmn, 5, "a=bcd", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode invalid");
  }
  if (errmsg != "invalid number literal.") {
    FEM_STOP("error errmsg invalid");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  decoded = -1;
  hy36decode(cmn, 5, "410b0", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode invalid");
  }
  if (errmsg != "invalid number literal.") {
    FEM_STOP("error errmsg invalid");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  decoded = -1;
  hy36decode(cmn, 5, "410B0", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode invalid");
  }
  if (errmsg != "invalid number literal.") {
    FEM_STOP("error errmsg invalid");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  //C
  s5 = "=====";
  hy36encode(cmn, 3, 0, s5(1, 3), errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36encode width");
  }
  if (errmsg != "unsupported width.") {
    FEM_STOP("error errmsg width");
  }
  if (s5 != "***==") {
    FEM_STOP("no stars range");
  }
  decoded = -1;
  hy36decode(cmn, 3, "  0", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode width");
  }
  if (errmsg != "unsupported width.") {
    FEM_STOP("error errmsg width");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  decoded = -1;
  hy36decode(cmn, 3, "A00", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode width");
  }
  if (errmsg != "unsupported width.") {
    FEM_STOP("error errmsg width");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  decoded = -1;
  hy36decode(cmn, 3, "a00", decoded, errmsg, errmsg_len);
  if (errmsg_len == 0) {
    FEM_STOP("error hy36decode width");
  }
  if (errmsg != "unsupported width.") {
    FEM_STOP("error errmsg width");
  }
  if (decoded != 0) {
    FEM_STOP("decoded != 0 invalid");
  }
  //C
}

//C
//C Calls unit tests above. To use this file in your project,
//C comment out all lines below.
//C
//C To compile and run the unit tests use, e.g.:
//C   f77 -o hy36 hybrid_36_f.f
//C   ./hy36
//C
void
program_exercise(
  int argc,
  char const* argv[])
{
  common cmn(argc, argv);
  tst_hybrid_36_f(cmn, false);
}

} // namespace placeholder_please_replace

int
main(
  int argc,
  char const* argv[])
{
  return fem::main_with_catch(
    argc, argv,
    placeholder_please_replace::program_exercise);
}