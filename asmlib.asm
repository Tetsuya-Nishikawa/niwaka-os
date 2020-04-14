bits 32
global hlt
global in_8
global out_8
global in_32
global out_32
global asm_inthandler21
;global asm_inthandler26
global asm_inthandler48
global sidt
global cpuid
global rdmsr
global sti
global cli
global task_switch
global load_tr

extern inthandler21
;extern inthandler26
extern inthandler48

section .text
hlt:
    HLT 
    RET

sti:
	STI
	RET

cli:
	CLI
	RET

in_8:		;void in_8(int port);
	MOV DX, [ESP+4]
	IN  AL, DX
	RET

out_8:		;void out_8(int port, char value);
	MOV AL, [ESP+8]
	MOV DX, [ESP+4]
	OUT	DX, AL
	RET 

in_32:		;void in_32(int port);
	MOV DX, [ESP+4]
	IN  EAX, DX
	RET

out_32:		;void out_32(int port, int value);
	MOV EAX, [ESP+8]
	MOV DX, [ESP+4]
	OUT DX, EAX
	RET

;IDTRの値をロードする。
sidt:  
    SIDT [ESP+4]
    RET

asm_inthandler21:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	inthandler21
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD

;apic timer用のハンドラ
asm_inthandler48:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	inthandler48
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD

;apic有効?
cpuid:
	MOV EAX, 1
	CPUID
	MOV DWORD[0x0], EDX
	RET

rdmsr:
	MOV ECX, 0x1b
	RDMSR
	MOV DWORD[0x7000], EDX
	MOV DWORD[0x7004], EAX
	MOV EAX, 0xffffffff
	MOV DWORD[0xfee00000], EAX
	RET

;プロセス状態を保存する。
proc_save:
	PUSH	ES
	PUSH	DS
	PUSHAD

load_tr:
	LTR [ESP+4]
	RET

task_switch:
	JMP FAR [ESP+4]
	RET

