/* disarm -- a simple disassembler for ARM instructions
 * (c) 2000 Gareth McCaughan
 *
 * This file may be distributed and used freely provided:
 * 1. You do not distribute any version that lacks this
 *    copyright notice (exactly as it appears here, extending
 *    from the start to the end of the C-language comment
 *    containing these words)); and,
 * 2. If you distribute any modified version, its source
 *    contains a clear description of the ways in which
 *    it differs from the original version, and a clear
 *    indication that the changes are not mine.
 * There is no restriction on your permission to use and
 * distribute object code or executable code derived from
 * this.
 *
 * The original version of this file (or perhaps a later
 * version by the original author) may or may not be
 * available at http://web.ukonline.co.uk/g.mccaughan/g/software.html .
 *
 * Share and enjoy!    -- g
 */

/* changes for riscose:
 *
 * 0. add this changelog
 * 1. extract this header from disarm.c
 * 2. declare instr_disassemble as extern
 */

#ifndef DISARM_H_
#define DISARM_H_

typedef unsigned long word;
typedef unsigned long address;
typedef unsigned long addrdiff;
#define W(x) ((word*)(x))

#define declstruct(name) typedef struct name s##name, * p##name
#define defstruct(name) struct name
#define defequiv(new,old) typedef struct old s##new, * p##new

declstruct(DisOptions);
declstruct(Instruction);

typedef enum {
  target_None,		/* instruction doesn't refer to an address */
  target_Data,		/* instruction refers to address of data */
  target_FloatS,	/* instruction refers to address of single-float */
  target_FloatD,	/* instruction refers to address of double-float */
  target_FloatE,	/* blah blah extended-float */
  target_FloatP,	/* blah blah packed decimal float */
  target_Code,		/* instruction refers to address of code */
  target_Unknown	/* instruction refers to address of *something* */
} eTargetType;

defstruct(Instruction) {
  char text[128];	/* the disassembled instruction */
  int undefined;	/* non-0 iff it's an undefined instr */
  int badbits;		/* non-0 iff something reserved has the wrong value */
  int oddbits;		/* non-0 iff something unspecified isn't 0 */
  int is_SWI;		/* non-0 iff it's a SWI */
  word swinum;		/* only set for SWIs */
  address target;	/* address instr refers to */
  eTargetType target_type;	/* and what we expect to be there */
  long offset;		/* offset from register in LDR or STR or similar */
  char * addrstart;	/* start of address part of instruction, or 0 */
};

#define disopt_SWInames		1	/* use names, not &nnnn */
#define disopt_CommaSpace	2	/* put spaces after commas */
#define disopt_FIXS		4	/* bogus FIX syntax for ObjAsm */
#define disopt_ReverseBytes	8	/* byte-reverse words first */

defstruct(DisOptions) {
  word flags;
  char * * regnames;	/* pointer to 16 |char *|s: register names */
};

extern pInstruction instr_disassemble(word, address, pDisOptions);

#endif
