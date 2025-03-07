#ifndef _RC10_REGISTER_H
#define _RC10_REGISTER_H

#if !defined(MACOS)
# ifndef BYTE_ORDER
# define BYTE_ORDER !BIG_ENDIAN
# endif
#endif


#include <cstdlib>
#include <cstdio>
#include <cassert>


#define RCP_NUM_GENERAL_COMBINERS 8

#define RCP_RGB   0
#define RCP_ALPHA 1
#define RCP_BLUE  2
#define RCP_NONE  3

enum RegisterName {
    REG_ZERO,
    REG_CONSTANT_COLOR0,
    REG_CONSTANT_COLOR1,
    REG_FOG,
    REG_PRIMARY_COLOR,
    REG_SECONDARY_COLOR,
    REG_TEXTURE0,
    REG_TEXTURE1,
    REG_TEXTURE2,
    REG_TEXTURE3,
    REG_SPARE0,
    REG_SPARE1,
    REG_SPARE0_PLUS_SECONDARY_COLOR,
    REG_E_TIMES_F,
    REG_DISCARD,
    REG_ONE
};

#ifdef __GNUC__
__attribute__ ((unused))
#endif
static const char* GetRegisterNameString(unsigned int reg_name) {
    switch(reg_name) {
    case REG_ZERO:
        return "0x0";
    case REG_CONSTANT_COLOR0:
        return "0x1";
    case REG_CONSTANT_COLOR1:
        return "0x2";
    case REG_FOG:
        return "0x3";
    case REG_PRIMARY_COLOR:
        return "0x4";
    case REG_SECONDARY_COLOR:
        return "0x5";
    case REG_TEXTURE0:
        return "0x8";
    case REG_TEXTURE1:
        return "0x9";
    case REG_TEXTURE2:
        return "0xa";
    case REG_TEXTURE3:
        return "0xb";
    case REG_SPARE0:
        return "0xc";
    case REG_SPARE1:
        return "0xd";
    case REG_SPARE0_PLUS_SECONDARY_COLOR:
        return "0xe";
    case REG_E_TIMES_F:
        return "0xf";
    case REG_DISCARD:
        return "0x0";
    case REG_ONE:
        // REG_ONE is a pseudo-register that should have been 
        // mapped to REG_ZERO (with modifier) in MappedRegisterStruct::Init; 
        // therefore, this should not be reachable. 
        assert(false);
        break;
    default:
        fprintf(stderr, "unknown register name index %d\n", reg_name);
        assert(false);
        break;
    }
    return "UNKNOWN";
}

#define BIAS_NONE 0
#define BIAS_BY_NEGATIVE_ONE_HALF 1

#define SCALE_NONE 0
#define SCALE_BY_TWO 1
#define SCALE_BY_FOUR 2
#define SCALE_BY_ONE_HALF 3

#define MAP_UNSIGNED_IDENTITY 0
#define MAP_UNSIGNED_INVERT 1
#define MAP_EXPAND_NORMAL 2
#define MAP_EXPAND_NEGATE 3
#define MAP_HALF_BIAS_NORMAL 4
#define MAP_HALF_BIAS_NEGATE 5
#define MAP_SIGNED_IDENTITY 6
#define MAP_SIGNED_NEGATE 7

typedef struct _RegisterEnum {
  union {
    struct {
#if BYTE_ORDER != BIG_ENDIAN
      unsigned int name          :16; // RegisterName enum for register
      unsigned int channel       : 2; // RCP_RGB, RCP_ALPHA, etc
      unsigned int readOnly      : 1; // true or false
      unsigned int finalOnly     : 1; // true or false
      unsigned int unused        :12;
#else
      unsigned int unused        :12;
      unsigned int finalOnly     : 1; // true or false
      unsigned int readOnly      : 1; // true or false
      unsigned int channel       : 2; // RCP_RGB, RCP_ALPHA, RCP_BLUE, RCP_NONE
      unsigned int name          :16; // RegisterName enum for register
#endif
    } bits;
    unsigned int word;
  };
  int line_number;
} RegisterEnum;
// No need for writeOnly flag, since DISCARD is the only register in that category

// WARNING:  Don't monkey with the above structure or this macro
// unless you're absolutely sure of what you're doing!
// This constant allocation makes validation *much* cleaner.
#define RCP_SET_REGISTER_ENUM(name, channel, readonly, finalonly) \
   ((finalonly << 19) | (readonly << 18) | (channel << 16) | name)

#define RCP_FOG_RGB         RCP_SET_REGISTER_ENUM(REG_FOG, RCP_RGB,   1, 0)
#define RCP_FOG_ALPHA       RCP_SET_REGISTER_ENUM(REG_FOG, RCP_ALPHA, 1, 1)
#define RCP_FOG_BLUE        RCP_SET_REGISTER_ENUM(REG_FOG, RCP_BLUE,  1, 0)
#define RCP_FOG           RCP_SET_REGISTER_ENUM(REG_FOG, RCP_NONE,  1, 0)
#define RCP_PRIMARY_COLOR_RGB   RCP_SET_REGISTER_ENUM(REG_PRIMARY_COLOR, RCP_RGB,   0, 0)
#define RCP_PRIMARY_COLOR_ALPHA   RCP_SET_REGISTER_ENUM(REG_PRIMARY_COLOR, RCP_ALPHA, 0, 0)
#define RCP_PRIMARY_COLOR_BLUE    RCP_SET_REGISTER_ENUM(REG_PRIMARY_COLOR, RCP_BLUE,  0, 0)
#define RCP_PRIMARY_COLOR     RCP_SET_REGISTER_ENUM(REG_PRIMARY_COLOR, RCP_NONE,  0, 0)
#define RCP_SECONDARY_COLOR_RGB   RCP_SET_REGISTER_ENUM(REG_SECONDARY_COLOR, RCP_RGB,   0, 0)
#define RCP_SECONDARY_COLOR_ALPHA RCP_SET_REGISTER_ENUM(REG_SECONDARY_COLOR, RCP_ALPHA, 0, 0)
#define RCP_SECONDARY_COLOR_BLUE  RCP_SET_REGISTER_ENUM(REG_SECONDARY_COLOR, RCP_BLUE,  0, 0)
#define RCP_SECONDARY_COLOR     RCP_SET_REGISTER_ENUM(REG_SECONDARY_COLOR, RCP_NONE,  0, 0)
#define RCP_SPARE0_RGB        RCP_SET_REGISTER_ENUM(REG_SPARE0, RCP_RGB,   0, 0)
#define RCP_SPARE0_ALPHA      RCP_SET_REGISTER_ENUM(REG_SPARE0, RCP_ALPHA, 0, 0)
#define RCP_SPARE0_BLUE       RCP_SET_REGISTER_ENUM(REG_SPARE0, RCP_BLUE,  0, 0)
#define RCP_SPARE0          RCP_SET_REGISTER_ENUM(REG_SPARE0, RCP_NONE,  0, 0)
#define RCP_SPARE1_RGB        RCP_SET_REGISTER_ENUM(REG_SPARE1, RCP_RGB,   0, 0)
#define RCP_SPARE1_ALPHA      RCP_SET_REGISTER_ENUM(REG_SPARE1, RCP_ALPHA, 0, 0)
#define RCP_SPARE1_BLUE       RCP_SET_REGISTER_ENUM(REG_SPARE1, RCP_BLUE,  0, 0)
#define RCP_SPARE1          RCP_SET_REGISTER_ENUM(REG_SPARE1, RCP_NONE,  0, 0)
#define RCP_TEXTURE0_RGB      RCP_SET_REGISTER_ENUM(REG_TEXTURE0, RCP_RGB,   0, 0)
#define RCP_TEXTURE0_ALPHA      RCP_SET_REGISTER_ENUM(REG_TEXTURE0, RCP_ALPHA, 0, 0)
#define RCP_TEXTURE0_BLUE     RCP_SET_REGISTER_ENUM(REG_TEXTURE0, RCP_BLUE,  0, 0)
#define RCP_TEXTURE0        RCP_SET_REGISTER_ENUM(REG_TEXTURE0, RCP_NONE,  0, 0)
#define RCP_TEXTURE1_RGB      RCP_SET_REGISTER_ENUM(REG_TEXTURE1, RCP_RGB,   0, 0)
#define RCP_TEXTURE1_ALPHA      RCP_SET_REGISTER_ENUM(REG_TEXTURE1, RCP_ALPHA, 0, 0)
#define RCP_TEXTURE1_BLUE     RCP_SET_REGISTER_ENUM(REG_TEXTURE1, RCP_BLUE,  0, 0)
#define RCP_TEXTURE1        RCP_SET_REGISTER_ENUM(REG_TEXTURE1, RCP_NONE,  0, 0)
#define RCP_TEXTURE2_RGB      RCP_SET_REGISTER_ENUM(REG_TEXTURE2, RCP_RGB,   0, 0)
#define RCP_TEXTURE2_ALPHA      RCP_SET_REGISTER_ENUM(REG_TEXTURE2, RCP_ALPHA, 0, 0)
#define RCP_TEXTURE2_BLUE     RCP_SET_REGISTER_ENUM(REG_TEXTURE2, RCP_BLUE,  0, 0)
#define RCP_TEXTURE2        RCP_SET_REGISTER_ENUM(REG_TEXTURE2, RCP_NONE,  0, 0)
#define RCP_TEXTURE3_RGB      RCP_SET_REGISTER_ENUM(REG_TEXTURE3, RCP_RGB,   0, 0)
#define RCP_TEXTURE3_ALPHA      RCP_SET_REGISTER_ENUM(REG_TEXTURE3, RCP_ALPHA, 0, 0)
#define RCP_TEXTURE3_BLUE     RCP_SET_REGISTER_ENUM(REG_TEXTURE3, RCP_BLUE,  0, 0)
#define RCP_TEXTURE3        RCP_SET_REGISTER_ENUM(REG_TEXTURE3, RCP_NONE,  0, 0)
#define RCP_CONST_COLOR0_RGB    RCP_SET_REGISTER_ENUM(REG_CONSTANT_COLOR0, RCP_RGB,   1, 0)
#define RCP_CONST_COLOR0_ALPHA    RCP_SET_REGISTER_ENUM(REG_CONSTANT_COLOR0, RCP_ALPHA, 1, 0)
#define RCP_CONST_COLOR0_BLUE   RCP_SET_REGISTER_ENUM(REG_CONSTANT_COLOR0, RCP_BLUE,  1, 0)
#define RCP_CONST_COLOR0      RCP_SET_REGISTER_ENUM(REG_CONSTANT_COLOR0, RCP_NONE,  1, 0)
#define RCP_CONST_COLOR1_RGB    RCP_SET_REGISTER_ENUM(REG_CONSTANT_COLOR1, RCP_RGB,   1, 0)
#define RCP_CONST_COLOR1_ALPHA    RCP_SET_REGISTER_ENUM(REG_CONSTANT_COLOR1, RCP_ALPHA, 1, 0)
#define RCP_CONST_COLOR1_BLUE   RCP_SET_REGISTER_ENUM(REG_CONSTANT_COLOR1, RCP_BLUE,  1, 0)
#define RCP_CONST_COLOR1      RCP_SET_REGISTER_ENUM(REG_CONSTANT_COLOR1, RCP_NONE,  1, 0)
#define RCP_ZERO_RGB        RCP_SET_REGISTER_ENUM(REG_ZERO, RCP_RGB,   1, 0)
#define RCP_ZERO_ALPHA        RCP_SET_REGISTER_ENUM(REG_ZERO, RCP_ALPHA, 1, 0)
#define RCP_ZERO_BLUE       RCP_SET_REGISTER_ENUM(REG_ZERO, RCP_BLUE,  1, 0)
#define RCP_ZERO          RCP_SET_REGISTER_ENUM(REG_ZERO, RCP_NONE,  1, 0)
#define RCP_ONE_RGB         RCP_SET_REGISTER_ENUM(REG_ONE, RCP_RGB,   1, 0)
#define RCP_ONE_ALPHA       RCP_SET_REGISTER_ENUM(REG_ONE, RCP_ALPHA, 1, 0)
#define RCP_ONE_BLUE        RCP_SET_REGISTER_ENUM(REG_ONE, RCP_BLUE,  1, 0)
#define RCP_ONE           RCP_SET_REGISTER_ENUM(REG_ONE, RCP_NONE,  1, 0)
#define RCP_DISCARD         RCP_SET_REGISTER_ENUM(REG_DISCARD, RCP_NONE, 0, 0)
#define RCP_FINAL_PRODUCT     RCP_SET_REGISTER_ENUM(REG_E_TIMES_F, RCP_NONE, 1, 1)
#define RCP_COLOR_SUM       RCP_SET_REGISTER_ENUM(REG_SPARE0_PLUS_SECONDARY_COLOR, RCP_NONE, 1, 1)

// #define MAP_CHANNEL(channel) ((RCP_RGB == (channel)) ? GL_RGB : (RCP_ALPHA == (channel) ? GL_ALPHA : GL_BLUE))

typedef union _BiasScaleEnum {
  struct {
#if BYTE_ORDER != BIG_ENDIAN
    unsigned int bias          :16; // OpenGL enum for bias
    unsigned int scale         :16; // OpenGL enum for scale
#else
    unsigned int scale         :16; // OpenGL enum for scale
    unsigned int bias          :16; // OpenGL enum for bias
#endif
  } bits;
  unsigned int word;
} BiasScaleEnum;

// WARNING:  Don't monkey with the above structure or this macro
// unless you're absolutely sure of what you're doing!
// This constant allocation makes validation *much* cleaner.
#define RCP_SET_BIAS_SCALE_ENUM(bias, scale) ((scale << 16) | bias)

#define RCP_BIAS_BY_NEGATIVE_ONE_HALF_SCALE_BY_TWO  RCP_SET_BIAS_SCALE_ENUM(BIAS_BY_NEGATIVE_ONE_HALF, SCALE_BY_TWO)
#define RCP_BIAS_BY_NEGATIVE_ONE_HALF       RCP_SET_BIAS_SCALE_ENUM(BIAS_BY_NEGATIVE_ONE_HALF, SCALE_NONE)
#define RCP_SCALE_BY_ONE_HALF           RCP_SET_BIAS_SCALE_ENUM(BIAS_NONE, SCALE_BY_ONE_HALF)
#define RCP_SCALE_BY_ONE              RCP_SET_BIAS_SCALE_ENUM(BIAS_NONE, SCALE_NONE)
#define RCP_SCALE_BY_TWO              RCP_SET_BIAS_SCALE_ENUM(BIAS_NONE, SCALE_BY_TWO)
#define RCP_SCALE_BY_FOUR             RCP_SET_BIAS_SCALE_ENUM(BIAS_NONE, SCALE_BY_FOUR)

class MappedRegisterStruct {
public:
  void Init(RegisterEnum _reg, int _map = MAP_UNSIGNED_IDENTITY)
  {
    if (REG_ONE == _reg.bits.name) {
      _reg.bits.name = REG_ZERO;
      switch (_map) {
      case MAP_UNSIGNED_IDENTITY:
        _map = MAP_UNSIGNED_INVERT;
        break;
      case MAP_UNSIGNED_INVERT:
        _map = MAP_UNSIGNED_IDENTITY;
        break;
      case MAP_EXPAND_NORMAL:
        _map = MAP_UNSIGNED_INVERT;
        break;
      case MAP_EXPAND_NEGATE:
        _map = MAP_EXPAND_NORMAL;
        break;
      case MAP_HALF_BIAS_NORMAL:
        _map = MAP_HALF_BIAS_NEGATE;
        break;
      case MAP_HALF_BIAS_NEGATE:
        _map = MAP_HALF_BIAS_NORMAL;
        break;
      case MAP_SIGNED_IDENTITY:
        _map = MAP_UNSIGNED_INVERT;
        break;
      case MAP_SIGNED_NEGATE:
        _map = MAP_EXPAND_NORMAL;
        break;
      }
    }
    map = _map;
    reg = _reg;
  }
  int map;
  RegisterEnum reg;
};

#ifdef TEST_BIT_FIELDS

class RegisterEnumTest {
  public:
    RegisterEnumTest()
    {
      RegisterEnum reg;
      bool error = false;

      if (sizeof(reg.bits) != sizeof(reg.word))
        error = true;

      reg.word = 0; reg.bits.name = 0xFFFF;
      if (RCP_SET_REGISTER_ENUM(0xFFFF, 0, 0, 0) != reg.word)
        error = true;

      reg.word = 0; reg.bits.channel = 3;
      if (RCP_SET_REGISTER_ENUM(0, 3, 0, 0) != reg.word) 
        error = true;

      reg.word = 0; reg.bits.readOnly = true;
      if (RCP_SET_REGISTER_ENUM(0, 0, 1, 0) != reg.word) 
        error = true;

      reg.word = 0; reg.bits.finalOnly = true;
      if (RCP_SET_REGISTER_ENUM(0, 0, 0, 1) != reg.word) 
        error = true;

      if (error) {
  fprintf(stderr, "ERROR: Bit Fields were not compiled correctly in " __FILE__ "!\n");
  exit(1);
      }
    }
};

static RegisterEnumTest registerEnumTest;
#endif

#endif
