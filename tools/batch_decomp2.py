#!/usr/bin/env python3
import os, re

asm_dir = "asm"
converted_dir = os.path.join(asm_dir, "converted")
decomp_dir = os.path.join("src", "decomp")

candidates = []

def phex(s):
    if s.upper().startswith('0X'):
        return '0x%X' % int(s, 16)
    return s

for f in sorted(os.listdir(asm_dir)):
    if not f.endswith('.s') or not os.path.isfile(os.path.join(asm_dir, f)):
        continue
    path = os.path.join(asm_dir, f)
    with open(path) as fh:
        content = fh.read()
    m = re.search(r'glabel (\w+)', content)
    if not m:
        continue
    fn = m.group(1)
    asm_name = f.replace('.s', '')
    body = re.search(r'glabel \w+.*?\n((?:.*\n)*?)\.ltorg', content, re.MULTILINE)
    if not body:
        body = re.search(r'glabel \w+.*?\n((?:.*\n)*?)\.end', content, re.MULTILINE)
    if not body:
        continue
    ins = [i for i in re.findall(r'/\* [0-9A-Fa-f]+ \*/\s+(.+)', body.group(1))
           if not i.startswith('.') and not i.startswith('@')]
    n = len(ins)
    c_code = None
    h = '#include "global.h"\n'
    has_branch = any(re.match(r'B(NE|EQ|GE|LT|LS|HI|CC|CS|MI|PL|VS|AL)? ', i) for i in ins[:-1])
    has_sym_d = bool(re.search(r'= (D_\w+)', content))

    # n==1: BX LR
    if n == 1 and ins[0] == 'BX LR':
        c_code = h + 'void %s(void) {}\n' % fn

    # n==2: return const
    if n == 2 and ins[1] == 'BX LR' and not c_code:
        mv = re.match(r'MOVS R0, (#[\dXxa-fA-F]+)', ins[0])
        if mv:
            c_code = h + 'u32 %s(void) { return %s; }\n' % (fn, phex(mv.group(1)))

    # n==2: binary ops
    if n == 2 and ins[1] == 'BX LR' and not r_code:
        bin_ops = {'ADDS R0, R0, R1': ('u32','+'), 'ADDS R0, R1': ('u32','+'),
                   'SUBS R0, R0, R1': ('s32','-'), 'ANDS R0, R1': ('u32','&'),
                   'ORRS R0, R1': ('u32','|'), 'EORS R0, R1': ('u32','^')}
        if ins[0] in bin_ops:
            t, op = bin_ops[ins[0]]
            c_code = h + '%s %s(%s a0, %s a1) { return a0 %s a1; }\n' % (t, fn, t, t, op)
        elif ins[0] == 'MVNS R0, R0':
            c_code = h + 'u32 %s(u32 a0) { return ~a0; }\n' % fn
        elif ins[0] == 'RSBS R0, R0, #0':
            c_code = h + 's32 %s(s32 a0) { return -a0; }\n' % fn

    # n==2: shifts
    if n == 2 and ins[1] == 'BX LR' and not r_code:
        sh = re.match(r'(LSLS|LSRS) R0, R0, #([\dXxa-fA-F]+)', ins[0])
        if sh:
            op = '<<' if 'LSL' in sh.group(1) else '>>'
            c_code = h + 'u32 %s(u32 a0) { return a0 %s %s; }\n' % (fn, op, phex(sh.group(2)))

    # n==2: single field store R1 [R0, #off]
    if n == 2 and ins[1] == 'BX LR' and not c_code:
        st = re.match(r'(STRB|STRH|STR) R1, \[R0, #([\dXxa-fA-F]+)\]', ins[0])
        if st:
            vt = {'STRB':'u8','STRH':'u16','STR':'u32'}[st.group(1)]
            c_code = h + 'void %s(void *a0, %s a1) { *(%s *)((u8 *)a0 + %s) = a1; }\n' % (fn, vt, vt, phex(st.group(2)))

    # n==2: single field store R0 [R1, #off]
    if n == 2 and ins[1] == 'BX LR' and not c_code:
        st = re.match(r'(STRB|STRH|STR) R0, \[R1, #([\dXxa-fA-F]+)\]', ins[0])
        if st:
            vt = {'STRB':'u8','STRH':'u16','STR':'u32'}[st.group(1)]
            c_code = h + 'void %s(void *a1, %s a0) { *(%s *)((u8 *)a1 + %s) = a0; }\n' % (fn, vt, vt, phex(st.group(2)))

    # n==2: single field load R0 [R0, #off]
    if n == 2 and ins[1] == 'BX LR' and not c_code:
        ld = re.match(r'(LDRB|LDRH|LDR) R0, \[R0, #([\dXxa-fA-F]+)\]', ins[0])
        if ld:
            rt = {'LDRB':'u8','LDRH':'u16','LDR':'u32'}[ld.group(1)]
            c_code = h + '%s %s(void *a0) { return *(%s *)((u8 *)a0 + %s); }\n' % (rt, fn, rt, phex(ld.group(2)))

    # n==2: single field load R0 [R1, #off]
    if n == 2 and ins[1] == 'BX LR' and not c_code:
        ld = re.match(r'(LDRB|LDRH|LDR) R0, \[R1, #([\dXxa-fA-F]+)\]', ins[0])
        if ld:
            rt = {'LDRB':'u8','LDRH':'u16','LDR':'u32'}[ld.group(1)]
            c_code = h + '%s %s(void *a1) { return *(%s *)((u8 *)a1 + %s); }\n' % (rt, fn, rt, phex(ld.group(2)))

    # n==3: const op
    if n == 3 and ins[2] == 'BX LR' and not has_branch and not c_code:
        m1 = re.match(r'MOVS R1, #([\dXxa-fA-F]+)', ins[0])
        if m1:
            val = phex(m1.group(1))
            cmap = {'ADDS R0, R1':'+', 'SUBS R0, R0, R1':'-', 'ANDS R0, R1':'&', 'ORRS R0, R1':'|', 'EORS R0, R1':'^'}
            if ins[1] in cmap:
                c_code = h + 'u32 %s(u32 a0) { return a0 %s %s; }\n' % (fn, cmap[ins[1]], val)

    # n==3: D_ symbol getter/setter
    if n == 3 and ins[2] == 'BX LR' and not has_branch and has_sym_d and not c_code:
        sm = re.match(r'LDR R(\d+), =(\S+)', ins[0])
        if sm:
            reg, sym = sm.group(1), sm.group(2)
            if sym.startswith('D_'):
                st = re.match(r'(STRB|STRH|STR) R0, \[R' + reg + r'\]$', ins[1])
                if st:
                    vt = {'STRB':'u8','STRH':'u16','STR':'u32'}[st.group(1)]
                    try:
                        a = int(sym[2:], 16)
                        v = 'volatile ' if 0x02000000 <= a < 0x04000000 else ''
                        c_code = h + 'void %s(u32 arg0) { *(%s%s *)0x%X = arg0; }\n' % (fn, v, vt, a)
                    except: pass
                if reg == '0' and not c_code:
                    ld = re.match(r'(LDRB|LDRH|LDR) R0, \[R0\]$', ins[1])
                    if ld:
                        rt = {'LDRB':'u8','LDRH':'u16','LDR':'u32'}[ld.group(1)]
                        try:
                            a = int(sym[2:], 16)
                            v = 'volatile ' if 0x02000000 <= a < 0x04000000 else ''
                            c_code = h + '%s %s(void) { return *(%s%s *)0x%X; }\n' % (rt, fn, v, rt, a)
                        except: pass

    # n==3: two-field setter
    if n == 3 and ins[2] == 'BX LR' and not has_branch and not c_code:
        m1 = re.match(r'(STRB|STRH|STR) R1, \[R0, #([\dXxa-fA-F]+)\]', ins[0])
        m2 = re.match(r'(STRB|STRH|STR) R2, \[R0, #([\dXxa-fA-F]+)\]', ins[1])
        if m1 and m2:
            t1 = {'STRB':'u8','STRH':'u16','STR':'u32'}[m1.group(1)]
            t2 = {'STRB':'u8','STRH':'u16','STR':'u32'}[m2.group(1)]
            c_code = h + 'void %s(void *a0, %s a1, %s a2) { *(%s *)((u8 *)a0 + %s) = a1; *(%s *)((u8 *)a0 + %s) = a2; }\n' % (fn, t1, t2, t1, phex(m1.group(2)), t2, phex(m2.group(2)))

    # n==4: bitfield extract
    if n == 4 and ins[3] == 'BX LR' and not has_branch and not c_code:
        ld = re.match(r'(LDRB|LDRH) R0, \[R0\]$', ins[0])
        sl = re.match(r'LSLS R0, R0, #([\dXxa-fA-F]+)', ins[1])
        sr = re.match(r'LSRS R0, R0, #([\dXxa-fA-F]+)', ins[2])
        if ld and sl and sr:
            ct = 'u8' if ld.group(1)=='LDRB' else 'u16'
            c_code = h + 'u32 %s(%s *a0) { return (u32)((u32)(*a0) << %s) >> %s; }\n' % (fn, ct, phex(sl.group(1)), phex(sr.group(1)))

    # n==4: bitfield extract with offset
    if n == 4 and ins[3] == 'BX LR' and not has_branch and not c_code:
        ld = re.match(r'(LDRB|LDRH) R0, \[R0, #([\dXxa-fA-F]+)\]', ins[0])
        sl = re.match(r'LSLS R0, R0, #([\dXxa-fA-F]+)', ins[1])
        sr = re.match(r'LSRS R0, R0, #([\dXxa-fA-F]+)', ins[2])
        if ld and sl and sr:
            ct = 'u8' if ld.group(1)=='LDRB' else 'u16'
            c_code = h + 'u32 %s(void *a0) { return (u32)((u32)(*((%s *)((u8 *)a0 + %s))) << %s) >> %s; }\n' % (fn, ct, phex(ld.group(2)), phex(sl.group(1)), phex(sr.group(1)))

    # n==4: inc/dec
    if n == 4 and ins[3] == 'BX LR' and not has_branch and not c_code:
        for w, ldr, stype in [('u16','LDRH','STRH'), ('u32','LDR','STR')]:
            for op_s, c_op in [('SUBS R1, #1','--'), ('ADDS R1, #1','++')]:
                if ins[0]==ldr+' R1, [R0]' and ins[1]==op_s and ins[2]==stype+' R1, [R0]':
                    c_code = h + 'void %s(%s *a0) { (*a0)%s; }\n' % (fn, w, c_op)

    # n==4: void call wrapper (1 call)
    if n == 4 and ins[0]=='PUSH {LR}' and ins[2]=='POP {R0}' and ins[3]=='BX R0' and not c_code:
        if ins[1].startswith('BL '):
            c_code = h + 'void %s(void) { %s(); }\n' % (fn, ins[1][3:].strip())

    # n==5: void call wrapper (2 calls)
    if n == 5 and ins[0]=='PUSH {LR}' and ins[3]=='POP {R0}' and ins[4]=='BX R0' and not c_code:
        if ins[1].startswith('BL ') and ins[2].startswith('BL '):
            c_code = h + 'void %s(void) { %s(); %s(); }\n' % (fn, ins[1][3:].strip(), ins[2][3:].strip())

    # n==5: call with const arg
    if n == 5 and ins[0]=='PUSH {LR}' and ins[3]=='POP {R0}' and ins[4]=='BX R0' and not c_code:
        mv = re.match(r'MOVS R0, #([\dXxa-fA-F]+)', ins[1])
        if mv and ins[2].startswith('BL '):
            c_code = h + 'void %s(void) { %s(%s); }\n' % (fn, ins[2][3:].strip(), phex(mv.group(1)))

    # n==5: zero init struct
    if n == 5 and ins[4]=='BX LR' and not has_branch and not c_code:
        if ins[0]=='MOVS R1, #0' and ins[1]=='STR R1, [R0]':
            m2 = re.match(r'STRB R1, \[R0, #(\d+)\]', ins[2])
            m3 = re.match(r'STRB R1, \[R0, #(\d+)\]', ins[3])
            if m2 and m3:
                c_code = h + 'void %s(void *a0) { *(u32 *)a0 = 0; *(u8 *)((u8 *)a0 + %s) = 0; *(u8 *)((u8 *)a0 + %s) = 0; }\n' % (fn, m2.group(1), m3.group(1))

    # n==6: void call wrapper (3 calls)
    if n == 6 and ins[0]=='PUSH {LR}' and ins[4]=='POP {R0}' and ins[5]=='BX R0' and not c_code:
        if ins[1].startswith('BL ') and ins[2].startswith('BL ') and ins[3].startswith('BL '):
            c_code = h + 'void %s(void) { %s(); %s(); %s(); }\n' % (fn, ins[1][3:].strip(), ins[2][3:].strip(), ins[3][3:].strip())

    # n==6: call f; then call f2(const)
    if n == 6 and ins[0]=='PUSH {LR}' and ins[4]=='POP {R0}' and ins[5]=='BX R0' and not c_code:
        if ins[1].startswith('BL '):
            mv = re.match(r'MOVS R0, #([\dXxa-fA-F]+)', ins[2])
            if mv and ins[3].startswith('BL '):
                c_code = h + 'void %s(void) { %s(); %s(%s); }\n' % (fn, ins[1][3:].strip(), ins[3][3:].strip(), phex(mv.group(1)))

    # n==6: gSym load + call
    if n == 6 and ins[0]=='PUSH {LR}' and ins[4]=='POP {R0}' and ins[5]=='BX R0' and not c_code:
        if ins[1].startswith('LDR R0, =') and ins[2]=='LDR R0, [R0]' and ins[3].startswith('BL '):
            sym = ins[1].split('=')[1].strip()
            target = ins[3][3:].strip()
            if sym.startswith('g'):
