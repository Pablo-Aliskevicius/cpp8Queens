; Listing generated by Microsoft (R) Optimizing Compiler Version 19.38.33135.0 

include listing.inc

INCLUDELIB OLDNAMES

PUBLIC	??_C@_01LFCBOECM@?4@				; `string'
PUBLIC	??_C@_0CE@POKBMHDB@?5failures?5for?5half?5a?5board?5of?5s@ ; `string'
PUBLIC	??_C@_0BA@LLNMHFLI@?5solutions?5and?5@		; `string'
PUBLIC	??_C@_0BF@DOJLFCKL@?5microseconds?0?5with?5@	; `string'
PUBLIC	??_C@_0BA@DMBECPMG@Resolving?5took?5@		; `string'
PUBLIC	??_C@_0CA@MJLLAPHA@Size?5must?5be?5at?5most?516?0?5it?5is?5@ ; `string'
EXTRN	__imp_??6?$basic_ostream@DU?$char_traits@D@std@@@std@@QEAAAEAV01@N@Z:PROC
EXTRN	__imp_??6?$basic_ostream@DU?$char_traits@D@std@@@std@@QEAAAEAV01@I@Z:PROC
;	COMDAT ??_C@_0CA@MJLLAPHA@Size?5must?5be?5at?5most?516?0?5it?5is?5@
CONST	SEGMENT
??_C@_0CA@MJLLAPHA@Size?5must?5be?5at?5most?516?0?5it?5is?5@ DB 'Size mus'
	DB	't be at most 16, it is ', 00H		; `string'
CONST	ENDS
;	COMDAT ??_C@_0BA@DMBECPMG@Resolving?5took?5@
CONST	SEGMENT
??_C@_0BA@DMBECPMG@Resolving?5took?5@ DB 'Resolving took ', 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_0BF@DOJLFCKL@?5microseconds?0?5with?5@
CONST	SEGMENT
??_C@_0BF@DOJLFCKL@?5microseconds?0?5with?5@ DB ' microseconds, with ', 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_0BA@LLNMHFLI@?5solutions?5and?5@
CONST	SEGMENT
??_C@_0BA@LLNMHFLI@?5solutions?5and?5@ DB ' solutions and ', 00H ; `string'
CONST	ENDS
;	COMDAT ??_C@_0CE@POKBMHDB@?5failures?5for?5half?5a?5board?5of?5s@
CONST	SEGMENT
??_C@_0CE@POKBMHDB@?5failures?5for?5half?5a?5board?5of?5s@ DB ' failures '
	DB	'for half a board of size ', 00H		; `string'
CONST	ENDS
;	COMDAT ??_C@_01LFCBOECM@?4@
CONST	SEGMENT
??_C@_01LFCBOECM@?4@ DB '.', 00H			; `string'
PUBLIC	??$pow@HH$0A@@@YANHH@Z				; pow<int,int,0>
PUBLIC	?do_solve@qns16@@YAXAEBT__m256i@@AEAV?$vector@HV?$allocator@H@std@@@std@@H@Z ; qns16::do_solve
PUBLIC	?threaten@qns16@@YA?BT__m256i@@AEBT2@HH@Z	; qns16::threaten
PUBLIC	?is_totally_under_threat@qns16@@YA_NAEBT__m256i@@H@Z ; qns16::is_totally_under_threat
PUBLIC	??Uqns16@@YA?AT__m256i@@AEBT1@0@Z		; qns16::operator|
PUBLIC	??Iqns16@@YA?AT__m256i@@AEBT1@0@Z		; qns16::operator&
PUBLIC	??8qns16@@YA_NAEBT__m256i@@0@Z			; qns16::operator==
PUBLIC	?set_board_size@solver@qns16@@SAXH@Z		; qns16::solver::set_board_size
PUBLIC	?test@solver@qns16@@SAXXZ			; qns16::solver::test
PUBLIC	?solve@solver@qns16@@SANXZ			; qns16::solver::solve
?success_count@qns16@@3IA DD 01H DUP (?)		; qns16::success_count
?threats@qns16@@3QBT__m256i@@B DB 02000H DUP (?)	; qns16::threats
?failures_count@qns16@@3IA DD 01H DUP (?)		; qns16::failures_count
_BSS	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?do_solve@qns16@@YAXAEBT__m256i@@AEAV?$vector@HV?$allocator@H@std@@@std@@H@Z DD imagerel $LN83@do_solve
	DD	imagerel $LN83@do_solve+566
	DD	imagerel $unwind$?do_solve@qns16@@YAXAEBT__m256i@@AEAV?$vector@HV?$allocator@H@std@@@std@@H@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?threaten@qns16@@YA?BT__m256i@@AEBT2@HH@Z DD imagerel $LN6@threaten
	DD	imagerel $LN6@threaten+99
	DD	imagerel $unwind$?threaten@qns16@@YA?BT__m256i@@AEBT2@HH@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??__Ethreats@qns16@@YAXXZ DD imagerel ??__Ethreats@qns16@@YAXXZ
	DD	imagerel ??__Ethreats@qns16@@YAXXZ+61853
	DD	imagerel $unwind$??__Ethreats@qns16@@YAXXZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?is_totally_under_threat@qns16@@YA_NAEBT__m256i@@H@Z DD imagerel $LN10@is_totally
	DD	imagerel $LN10@is_totally+168
	DD	imagerel $unwind$?is_totally_under_threat@qns16@@YA_NAEBT__m256i@@H@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??Uqns16@@YA?AT__m256i@@AEBT1@0@Z DD imagerel $LN4@operator
	DD	imagerel $LN4@operator+72
	DD	imagerel $unwind$??Uqns16@@YA?AT__m256i@@AEBT1@0@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$??Iqns16@@YA?AT__m256i@@AEBT1@0@Z DD imagerel $LN4@operator
	DD	imagerel $LN4@operator+72
	DD	imagerel $unwind$??Iqns16@@YA?AT__m256i@@AEBT1@0@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?set_board_size@solver@qns16@@SAXH@Z DD imagerel $LN9@set_board_
	DD	imagerel $LN9@set_board_+89
	DD	imagerel $unwind$?set_board_size@solver@qns16@@SAXH@Z
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?test@solver@qns16@@SAXXZ DD imagerel $LN24@test
	DD	imagerel $LN24@test+83
	DD	imagerel $unwind$?test@solver@qns16@@SAXXZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?solve@solver@qns16@@SANXZ DD imagerel $LN244
	DD	imagerel $LN244+1018
	DD	imagerel $unwind$?solve@solver@qns16@@SANXZ
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?dtor$0@?0??solve@solver@qns16@@SANXZ@4HA DD imagerel ?dtor$0@?0??solve@solver@qns16@@SANXZ@4HA
	DD	imagerel ?dtor$0@?0??solve@solver@qns16@@SANXZ@4HA+39
	DD	imagerel $unwind$?dtor$0@?0??solve@solver@qns16@@SANXZ@4HA
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?dtor$1@?0??solve@solver@qns16@@SANXZ@4HA DD imagerel ?dtor$1@?0??solve@solver@qns16@@SANXZ@4HA
	DD	imagerel ?dtor$1@?0??solve@solver@qns16@@SANXZ@4HA+36
	DD	imagerel $unwind$?dtor$1@?0??solve@solver@qns16@@SANXZ@4HA
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$?dtor$2@?0??solve@solver@qns16@@SANXZ@4HA DD imagerel ?dtor$2@?0??solve@solver@qns16@@SANXZ@4HA
	DD	imagerel ?dtor$2@?0??solve@solver@qns16@@SANXZ@4HA+39
	DD	imagerel $unwind$?dtor$2@?0??solve@solver@qns16@@SANXZ@4HA
;	COMDAT xdata
xdata	SEGMENT
$unwind$?dtor$2@?0??solve@solver@qns16@@SANXZ@4HA DD 030801H
	DD	0d0044208H
	DD	05002H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?dtor$1@?0??solve@solver@qns16@@SANXZ@4HA DD 030801H
	DD	0d0044208H
	DD	05002H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?dtor$0@?0??solve@solver@qns16@@SANXZ@4HA DD 030801H
	DD	0d0044208H
	DD	05002H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$ip2state$?solve@solver@qns16@@SANXZ DB 0eH
	DB	'%', 02H
	DB	00H
	DB	099H, 02H
	DB	04H
	DB	0bdH, 03H
	DB	06H
	DB	'E', 02H
	DB	04H
	DB	0c2H
	DB	02H
	DB	0b1H, 02H
	DB	00H
	DB	'p'
	DB	04H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$stateUnwindMap$?solve@solver@qns16@@SANXZ DB 06H
	DB	0eH
	DD	imagerel ?dtor$0@?0??solve@solver@qns16@@SANXZ@4HA
	DB	02eH
	DD	imagerel ?dtor$1@?0??solve@solver@qns16@@SANXZ@4HA
	DB	02eH
	DD	imagerel ?dtor$2@?0??solve@solver@qns16@@SANXZ@4HA
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$cppxdata$?solve@solver@qns16@@SANXZ DB 028H
	DD	imagerel $stateUnwindMap$?solve@solver@qns16@@SANXZ
	DD	imagerel $ip2state$?solve@solver@qns16@@SANXZ
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?solve@solver@qns16@@SANXZ DD 0134819H
	DD	0f882eH
	DD	0107829H
	DD	0116824H
	DD	02c741fH
	DD	02b641fH
	DD	02a341fH
	DD	024011fH
	DD	0e016f018H
	DD	0c012d014H
	DD	05010H
	DD	imagerel __GSHandlerCheck_EH4
	DD	imagerel $cppxdata$?solve@solver@qns16@@SANXZ
	DD	0aeH
	DD	040H
	DD	020H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?test@solver@qns16@@SAXXZ DQ 00000420400010401r ; 3.58617e-310
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?set_board_size@solver@qns16@@SAXH@Z DQ 03002320600020601r ; 1.96423e-77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??Iqns16@@YA?AT__m256i@@AEBT1@0@Z DQ 05002720600020f01r ; 2.66979e+77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??Uqns16@@YA?AT__m256i@@AEBT1@0@Z DQ 05002720600020f01r ; 2.66979e+77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?is_totally_under_threat@qns16@@YA_NAEBT__m256i@@H@Z DQ 05002720600020f01r ; 2.66979e+77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$??__Ethreats@qns16@@YAXXZ DQ 05002f20900021201r ; 2.74217e+77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?threaten@qns16@@YA?BT__m256i@@AEBT2@HH@Z DQ 05002720600020f01r ; 2.66979e+77
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?do_solve@qns16@@YAXAEBT__m256i@@AEAV?$vector@HV?$allocator@H@std@@@std@@H@Z DQ 00012011200092919r ; 2.50379e-308
	DQ	07005c007e009f00br		; 4.22093e+231
	DQ	00000500230036004r		; 4.34631e-310
	DD	imagerel __GSHandlerCheck
	DQ	00000004000000044r		; 1.35808e-312
	DD	020H
?board_size@qns16@@3HA DD 010H				; qns16::board_size
?threats$initializer$@qns16@@3P6AXXZEA DQ FLAT:??__Ethreats@qns16@@YAXXZ ; qns16::threats$initializer$
; Function compile flags: /Ogtpy
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\xmemory
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cmath
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\limits
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\high_res_clock.cpp
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\high_res_clock.cpp
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\__msvc_chrono.hpp
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\high_res_clock.cpp
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\xutility
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\high_res_clock.cpp
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\xmemory
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\xmemory
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp
;	COMDAT ?solve@solver@qns16@@SANXZ
_TEXT	SEGMENT
median_time$ = 0
microseconds$43 = 0
starting_map$44 = 32
$T10 = 64
times_vec$ = 96
_Newcapacity$ = 120
end_$45 = 120
solution$ = 128
timer$46 = 152
__$ArrayPad$ = 168
?solve@solver@qns16@@SANXZ PROC				; qns16::solver::solve, COMDAT

; 152  : 	{

$LN244:
	mov	rax, rsp
	mov	QWORD PTR [rax+8], rbx
	mov	QWORD PTR [rax+16], rsi
	mov	QWORD PTR [rax+24], rdi
	push	rbp
	push	r12
	push	r13
	push	r14
	push	r15
	sub	rsp, 288				; 00000120H
	vmovaps	XMMWORD PTR [rax-56], xmm6
	vmovaps	XMMWORD PTR [rax-72], xmm7
	vmovaps	XMMWORD PTR [rax-88], xmm8
	lea	rbp, QWORD PTR [rsp+64]
	and	rbp, -32				; ffffffffffffffe0H
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rsp
	mov	QWORD PTR __$ArrayPad$[rbp], rax

; 153  : 		failures_count = 0ULL;

	xor	r15d, r15d
	mov	DWORD PTR ?failures_count@qns16@@3IA, r15d

; 154  : 		success_count = 0ULL;

	mov	DWORD PTR ?success_count@qns16@@3IA, r15d
	vpxor	xmm0, xmm0, xmm0

; 155  : 		std::vector<int> solution(board_size, -1);

	lea	esi, QWORD PTR [r15-1]
	movsxd	r10, DWORD PTR ?board_size@qns16@@3HA
	mov	rbx, r10
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector

; 396  :     _CONSTEXPR20 _Vector_val() noexcept : _Myfirst(), _Mylast(), _Myend() {}

	vmovdqu	XMMWORD PTR solution$[rbp], xmm0
	mov	QWORD PTR solution$[rbp+16], r15

; 2073 :         if (_Count != 0) {

	test	r10d, r10d
	je	SHORT $LN171@solve

; 2074 :             _Buy_nonzero(_Count);

	mov	rdx, rbx
	lea	rcx, QWORD PTR solution$[rbp]
	call	?_Buy_nonzero@?$vector@HV?$allocator@H@std@@@std@@AEAAX_K@Z ; std::vector<int,std::allocator<int> >::_Buy_nonzero

; 2080 :                 _My_data._Mylast = _Uninitialized_fill_n(_My_data._Myfirst, _Count, _Val..., _Al);

	mov	rdi, QWORD PTR solution$[rbp]
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\xmemory

; 1790 :     _CONSTEXPR20 _Uninitialized_backout_al(pointer _Dest, _Alloc& _Al_) : _First(_Dest), _Last(_Dest), _Al(_Al_) {}

	lea	rbx, QWORD PTR [rbx*4]
	mov	r8, rbx
	and	r8, -4
	mov	edx, esi
	mov	rcx, rdi
	call	memset
	lea	rax, QWORD PTR [rdi+rbx]
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector

; 2080 :                 _My_data._Mylast = _Uninitialized_fill_n(_My_data._Myfirst, _Count, _Val..., _Al);

	mov	QWORD PTR solution$[rbp+8], rax
	mov	r10d, DWORD PTR ?board_size@qns16@@3HA
$LN171@solve:
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp

; 156  : 		const int loops = int(pow(16 - board_size, 3)) + 1;

	mov	eax, 16
	sub	eax, r10d
	vxorps	xmm1, xmm1, xmm1
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cmath

; 673  : _GENERIC_MATH2(pow)

	vcvtsi2sd xmm1, xmm1, eax
	vmulsd	xmm0, xmm1, xmm1
	vmulsd	xmm1, xmm0, xmm1
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp

; 156  : 		const int loops = int(pow(16 - board_size, 3)) + 1;

	vcvttsd2si ebx, xmm1
	inc	ebx

; 157  : 
; 158  : 		const int starting_rows_to_test = (board_size / 2) + (board_size % 2);

	mov	eax, r10d
	cdq
	mov	ecx, 2
	idiv	ecx
	lea	esi, DWORD PTR [rax+rdx]
	vxorpd	xmm6, xmm6, xmm6
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\limits

; 907  :         return DBL_MAX;

	vmovsd	xmm7, QWORD PTR __real@7fefffffffffffff
	vpxor	xmm0, xmm0, xmm0
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector

; 396  :     _CONSTEXPR20 _Vector_val() noexcept : _Myfirst(), _Mylast(), _Myend() {}

	vmovdqu	XMMWORD PTR times_vec$[rbp], xmm0
	mov	QWORD PTR times_vec$[rbp+16], r15
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp

; 162  : 		times_vec.reserve(loops);

	movsxd	rax, ebx
	mov	QWORD PTR _Newcapacity$[rbp], rax
	test	ebx, ebx
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector

; 1687 :         if (_Newcapacity > capacity()) { // something to do (reserve() never shrinks)

	je	$LN236@solve

; 1688 :             if (_Newcapacity > max_size()) {

	mov	rcx, 2305843009213693951		; 1fffffffffffffffH
	cmp	rax, rcx
	ja	$LN242@solve

; 1690 :             }
; 1691 : 
; 1692 :             _Reallocate<_Reallocation_policy::_At_least>(_Newcapacity);

	lea	rdx, QWORD PTR _Newcapacity$[rbp]
	lea	rcx, QWORD PTR times_vec$[rbp]
	call	??$_Reallocate@$0A@@?$vector@NV?$allocator@N@std@@@std@@AEAAXAEA_K@Z ; std::vector<double,std::allocator<double> >::_Reallocate<0>
	mov	r10d, DWORD PTR ?board_size@qns16@@3HA
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp

; 164  : 		for (int loop = 0; loop < loops; ++loop)

	test	ebx, ebx
	jle	$LN236@solve
	mov	r14d, ebx
	lea	r13, OFFSET FLAT:?row_masks@qns16cmn@@3PAT__m256i@@A+16
	mov	r12, -4616189618054758400		; bff0000000000000H
	vmovsd	xmm8, QWORD PTR __real@3f50624dd2f1a9fc
	npad	1
$LL4@solve:

; 165  : 		{
; 166  : 			failures_count = 0;

	mov	DWORD PTR ?failures_count@qns16@@3IA, r15d

; 167  : 			success_count = 0;

	mov	DWORD PTR ?success_count@qns16@@3IA, r15d

; 168  : 
; 169  : 			map_t starting_map{ .m256i_u64 { 0ULL, 0ULL, 0ULL, 0ULL } };

	mov	rcx, r15
	mov	QWORD PTR starting_map$44[rbp], rcx
	mov	rdx, r15
	mov	QWORD PTR starting_map$44[rbp+8], rdx
	mov	r8, r15
	mov	QWORD PTR starting_map$44[rbp+16], r15
	mov	r9, r15
	mov	QWORD PTR starting_map$44[rbp+24], r15

; 170  : 			for (int i = board_size; i < maximum_allowed_board_size; ++i)

	movsxd	rax, r10d
	cmp	rax, 16
	jae	SHORT $LN235@solve
	shl	rax, 5
	add	rax, r13
	npad	6
$LL7@solve:

; 61   : 		return m256i{ .m256i_u64 = {

	or	rcx, QWORD PTR [rax-16]
	mov	QWORD PTR $T10[rbp], rcx
	or	rdx, QWORD PTR [rax-8]
	mov	QWORD PTR $T10[rbp+8], rdx
	or	r8, QWORD PTR [rax]
	mov	QWORD PTR $T10[rbp+16], r8
	or	r9, QWORD PTR [rax+8]
	mov	QWORD PTR $T10[rbp+24], r9

; 171  : 			{
; 172  : 				starting_map = starting_map | row_masks[i];

	vmovdqu	ymm0, YMMWORD PTR $T10[rbp]
	vmovdqu	YMMWORD PTR starting_map$44[rbp], ymm0

; 170  : 			for (int i = board_size; i < maximum_allowed_board_size; ++i)

	inc	r10d
	lea	rax, QWORD PTR [rax+32]
	cmp	r10d, 16
	jae	SHORT $LN235@solve
	mov	r9, QWORD PTR starting_map$44[rbp+24]
	mov	r8, QWORD PTR starting_map$44[rbp+16]
	mov	rdx, QWORD PTR starting_map$44[rbp+8]
	mov	rcx, QWORD PTR starting_map$44[rbp]
	jmp	SHORT $LL7@solve
$LN235@solve:
	vpxor	xmm0, xmm0, xmm0
	vmovups	XMMWORD PTR timer$46[rbp], xmm0
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\high_res_clock.cpp

; 79   : 	m_Data = new TimerData;

	mov	ecx, 16
	vzeroupper
	call	??2@YAPEAX_K@Z				; operator new
	mov	rdi, rax

; 57   : 	const Clock::time_point m_begin = Clock::now();

	mov	rcx, rax
	call	?now@steady_clock@chrono@std@@SA?AV?$time_point@Usteady_clock@chrono@std@@V?$duration@_JU?$ratio@$00$0DLJKMKAA@@std@@@23@@23@XZ ; std::chrono::steady_clock::now

; 58   : 	microsecs_t m_duration = microsecs_t(-1);

	mov	QWORD PTR [rdi+8], r12

; 79   : 	m_Data = new TimerData;

	mov	QWORD PTR timer$46[rbp], rdi

; 80   : 	running = true;

	mov	BYTE PTR timer$46[rbp+8], 1
	xor	bl, bl
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp

; 175  : 			for (int_fast8_t current_row = 0; current_row < starting_rows_to_test; ++current_row)

	test	esi, esi
	jle	SHORT $LN9@solve
	mov	ecx, r15d
	npad	9
$LL10@solve:

; 176  : 			{
; 177  : 				solution[0] = current_row;

	mov	rax, QWORD PTR solution$[rbp]
	mov	DWORD PTR [rax], ecx

; 178  : 				do_solve(starting_map, solution, 0);

	xor	r8d, r8d
	lea	rdx, QWORD PTR solution$[rbp]
	lea	rcx, QWORD PTR starting_map$44[rbp]
	call	?do_solve@qns16@@YAXAEBT__m256i@@AEAV?$vector@HV?$allocator@H@std@@@std@@H@Z ; qns16::do_solve

; 175  : 			for (int_fast8_t current_row = 0; current_row < starting_rows_to_test; ++current_row)

	inc	bl
	movsx	ecx, bl
	cmp	ecx, esi
	jl	SHORT $LL10@solve
$LN9@solve:
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\high_res_clock.cpp

; 63   : 		const auto end_ = Clock::now();

	lea	rcx, QWORD PTR end_$45[rbp]
	call	?now@steady_clock@chrono@std@@SA?AV?$time_point@Usteady_clock@chrono@std@@V?$duration@_JU?$ratio@$00$0DLJKMKAA@@std@@@23@@23@XZ ; std::chrono::steady_clock::now
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\__msvc_chrono.hpp

; 291  :         return _CD(_CD(_Left).count() - _CD(_Right).count());

	mov	rax, QWORD PTR end_$45[rbp]
	sub	rax, QWORD PTR [rdi]
	vxorps	xmm0, xmm0, xmm0
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\high_res_clock.cpp

; 67   : 		m_duration = microsecs_t(double(ns.count()) / 1000.0);

	vcvtsi2sd xmm0, xmm0, rax
	vmulsd	xmm1, xmm0, xmm8
	vmovsd	QWORD PTR [rdi+8], xmm1

; 91   : 	running = false;

	mov	BYTE PTR timer$46[rbp+8], 0
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp

; 181  : 			auto microseconds = timer.GetElapsedMicroseconds();

	vmovsd	QWORD PTR microseconds$43[rbp], xmm1
	vminsd	xmm7, xmm1, xmm7

; 182  : 			if (microseconds < min_time) min_time = microseconds;

	vmaxsd	xmm6, xmm1, xmm6
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector

; 779  :         if (_Mylast != _My_data._Myend) {

	mov	rdx, QWORD PTR times_vec$[rbp+8]
	cmp	rdx, QWORD PTR times_vec$[rbp+16]
	je	SHORT $LN61@solve
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\xutility

; 255  :         ::new (static_cast<void*>(_STD addressof(_Obj))) _Ty(_STD forward<_Types>(_Args)...);

	vmovsd	QWORD PTR [rdx], xmm1
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector

; 804  :         ++_Mylast;

	add	QWORD PTR times_vec$[rbp+8], 8

; 780  :             return _Emplace_back_with_unused_capacity(_STD forward<_Valty>(_Val)...);

	jmp	SHORT $LN60@solve
$LN61@solve:

; 781  :         }
; 782  : 
; 783  :         return *_Emplace_reallocate(_Mylast, _STD forward<_Valty>(_Val)...);

	lea	r8, QWORD PTR microseconds$43[rbp]
	lea	rcx, QWORD PTR times_vec$[rbp]
	call	??$_Emplace_reallocate@AEBN@?$vector@NV?$allocator@N@std@@@std@@AEAAPEANQEANAEBN@Z ; std::vector<double,std::allocator<double> >::_Emplace_reallocate<double const &>
	npad	1
$LN60@solve:
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\high_res_clock.cpp

; 85   : 	delete m_Data;

	mov	edx, 16
	mov	rcx, rdi
	call	??3@YAXPEAX_K@Z				; operator delete
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp

; 164  : 		for (int loop = 0; loop < loops; ++loop)

	sub	r14, 1
	je	SHORT $LN236@solve
	mov	r10d, DWORD PTR ?board_size@qns16@@3HA
	jmp	$LL4@solve
$LN236@solve:

; 183  : 			if (microseconds > max_time) max_time = microseconds;
; 184  : 			times_vec.push_back(microseconds);
; 185  : 		}
; 186  : 
; 187  : 		double median_time;
; 188  : 		utils::ComputeAndDisplayMedianSpeed(median_time, times_vec, min_time, max_time);

	vmovaps	xmm3, xmm6
	vmovaps	xmm2, xmm7
	lea	rdx, QWORD PTR times_vec$[rbp]
	lea	rcx, QWORD PTR median_time$[rbp]
	call	?ComputeAndDisplayMedianSpeed@utils@@YAXAEANAEAV?$vector@NV?$allocator@N@std@@@std@@NN@Z ; utils::ComputeAndDisplayMedianSpeed

; 189  : 		do_show_results(failures_count, success_count, solutions, board_size);

	mov	edx, DWORD PTR ?success_count@qns16@@3IA
	mov	ecx, DWORD PTR ?failures_count@qns16@@3IA
	mov	r9d, DWORD PTR ?board_size@qns16@@3HA
	lea	r8, OFFSET FLAT:?solutions@qns16cmn@@3V?$vector@V?$vector@HV?$allocator@H@std@@@std@@V?$allocator@V?$vector@HV?$allocator@H@std@@@std@@@2@@std@@A ; qns16cmn::solutions
	call	?do_show_results@@YAX_K0AEBV?$vector@V?$vector@HV?$allocator@H@std@@@std@@V?$allocator@V?$vector@HV?$allocator@H@std@@@std@@@2@@std@@H@Z ; do_show_results

; 190  : 		std::cout.flush();

	mov	rcx, QWORD PTR __imp_?cout@std@@3V?$basic_ostream@DU?$char_traits@D@std@@@1@A
	call	QWORD PTR __imp_?flush@?$basic_ostream@DU?$char_traits@D@std@@@std@@QEAAAEAV12@XZ
	npad	1
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector

; 2052 :         if (_Myfirst) { // destroy and deallocate old array

	mov	rcx, QWORD PTR times_vec$[rbp]
	test	rcx, rcx
	je	SHORT $LN86@solve

; 2053 :             _Destroy_range(_Myfirst, _Mylast, _Al);
; 2054 :             _ASAN_VECTOR_REMOVE;
; 2055 :             _Al.deallocate(_Myfirst, static_cast<size_type>(_Myend - _Myfirst));

	mov	rax, QWORD PTR times_vec$[rbp+16]
	sub	rax, rcx
	sar	rax, 3
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\xmemory

; 973  :         _Deallocate<_New_alignof<_Ty>>(_Ptr, sizeof(_Ty) * _Count);

	lea	rdx, QWORD PTR [rax*8]
	mov	rax, rcx

; 260  :         if (_Bytes >= _Big_allocation_threshold) { // boost the alignment of big allocations to help autovectorization

	cmp	rdx, 4096				; 00001000H
	jb	SHORT $LN104@solve

; 156  :     _Bytes += _Non_user_size;

	add	rdx, 39					; 00000027H

; 157  : 
; 158  :     const uintptr_t* const _Ptr_user = static_cast<uintptr_t*>(_Ptr);
; 159  :     const uintptr_t _Ptr_container   = _Ptr_user[-1];

	mov	rcx, QWORD PTR [rcx-8]
	sub	rax, rcx

; 160  : 
; 161  :     // If the following asserts, it likely means that we are performing
; 162  :     // an aligned delete on memory coming from an unaligned allocation.
; 163  :     _STL_ASSERT(_Ptr_user[-2] == _Big_allocation_sentinel, "invalid argument");
; 164  : 
; 165  :     // Extra paranoia on aligned allocation/deallocation; ensure _Ptr_container is
; 166  :     // in range [_Min_back_shift, _Non_user_size]
; 167  : #ifdef _DEBUG
; 168  :     constexpr uintptr_t _Min_back_shift = 2 * sizeof(void*);
; 169  : #else // ^^^ defined(_DEBUG) / !defined(_DEBUG) vvv
; 170  :     constexpr uintptr_t _Min_back_shift = sizeof(void*);
; 171  : #endif // ^^^ !defined(_DEBUG) ^^^
; 172  :     const uintptr_t _Back_shift = reinterpret_cast<uintptr_t>(_Ptr) - _Ptr_container;
; 173  :     _STL_VERIFY(_Back_shift >= _Min_back_shift && _Back_shift <= _Non_user_size, "invalid argument");

	add	rax, -8
	cmp	rax, 31
	jbe	SHORT $LN104@solve
	call	QWORD PTR __imp__invalid_parameter_noinfo_noreturn
	int	3
$LN104@solve:

; 264  :         ::operator delete(_Ptr, _Bytes);

	call	??3@YAXPEAX_K@Z				; operator delete
	vpxor	xmm0, xmm0, xmm0
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector

; 2057 :             _Myfirst = nullptr;

	vmovdqu	XMMWORD PTR times_vec$[rbp], xmm0

; 2058 :             _Mylast  = nullptr;
; 2059 :             _Myend   = nullptr;

	mov	QWORD PTR times_vec$[rbp+16], r15
$LN86@solve:

; 2052 :         if (_Myfirst) { // destroy and deallocate old array

	mov	rcx, QWORD PTR solution$[rbp]
	test	rcx, rcx
	je	SHORT $LN113@solve

; 2053 :             _Destroy_range(_Myfirst, _Mylast, _Al);
; 2054 :             _ASAN_VECTOR_REMOVE;
; 2055 :             _Al.deallocate(_Myfirst, static_cast<size_type>(_Myend - _Myfirst));

	mov	rdx, QWORD PTR solution$[rbp+16]
	sub	rdx, rcx
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\xmemory

; 973  :         _Deallocate<_New_alignof<_Ty>>(_Ptr, sizeof(_Ty) * _Count);

	and	rdx, -4
	mov	rax, rcx

; 260  :         if (_Bytes >= _Big_allocation_threshold) { // boost the alignment of big allocations to help autovectorization

	cmp	rdx, 4096				; 00001000H
	jb	SHORT $LN131@solve

; 156  :     _Bytes += _Non_user_size;

	add	rdx, 39					; 00000027H

; 157  : 
; 158  :     const uintptr_t* const _Ptr_user = static_cast<uintptr_t*>(_Ptr);
; 159  :     const uintptr_t _Ptr_container   = _Ptr_user[-1];

	mov	rcx, QWORD PTR [rcx-8]
	sub	rax, rcx

; 160  : 
; 161  :     // If the following asserts, it likely means that we are performing
; 162  :     // an aligned delete on memory coming from an unaligned allocation.
; 163  :     _STL_ASSERT(_Ptr_user[-2] == _Big_allocation_sentinel, "invalid argument");
; 164  : 
; 165  :     // Extra paranoia on aligned allocation/deallocation; ensure _Ptr_container is
; 166  :     // in range [_Min_back_shift, _Non_user_size]
; 167  : #ifdef _DEBUG
; 168  :     constexpr uintptr_t _Min_back_shift = 2 * sizeof(void*);
; 169  : #else // ^^^ defined(_DEBUG) / !defined(_DEBUG) vvv
; 170  :     constexpr uintptr_t _Min_back_shift = sizeof(void*);
; 171  : #endif // ^^^ !defined(_DEBUG) ^^^
; 172  :     const uintptr_t _Back_shift = reinterpret_cast<uintptr_t>(_Ptr) - _Ptr_container;
; 173  :     _STL_VERIFY(_Back_shift >= _Min_back_shift && _Back_shift <= _Non_user_size, "invalid argument");

	add	rax, -8
	cmp	rax, 31
	jbe	SHORT $LN131@solve
	call	QWORD PTR __imp__invalid_parameter_noinfo_noreturn
	int	3
$LN131@solve:

; 264  :         ::operator delete(_Ptr, _Bytes);

	call	??3@YAXPEAX_K@Z				; operator delete
$LN113@solve:
; File E:\Documents and Settings\Pablo\My Documents\My Sources\Cpp8Queens\cpp8Queens\Cpp8Queens\sixteen_queens.cpp

; 192  : 		return double(median_time);

	vmovsd	xmm0, QWORD PTR median_time$[rbp]

; 193  : 	}

	mov	rcx, QWORD PTR __$ArrayPad$[rbp]
	xor	rcx, rsp
	call	__security_check_cookie
	lea	r11, QWORD PTR [rsp+288]
	mov	rbx, QWORD PTR [r11+48]
	mov	rsi, QWORD PTR [r11+56]
	mov	rdi, QWORD PTR [r11+64]
	vmovaps	xmm6, XMMWORD PTR [r11-16]
	vmovaps	xmm7, XMMWORD PTR [r11-32]
	vmovaps	xmm8, XMMWORD PTR [r11-48]
	mov	rsp, r11
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	rbp
	ret	0
$LN242@solve:
; File d:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector

; 1689 :                 _Xlength();

	call	?_Xlength@?$vector@NV?$allocator@N@std@@@std@@CAXXZ ; std::vector<double,std::allocator<double> >::_Xlength
	npad	1
$LN239@solve:
?solve@solver@qns16@@SANXZ ENDP				; qns16::solver::solve
_TEXT	ENDS
; Function compile flags: /Ogtpy
;	COMDAT text$x
text$x	SEGMENT
median_time$ = 0
microseconds$43 = 0
starting_map$44 = 32
$T10 = 64
times_vec$ = 96
_Newcapacity$ = 120
end_$45 = 120
solution$ = 128
timer$46 = 152
__$ArrayPad$ = 168
?dtor$0@?0??solve@solver@qns16@@SANXZ@4HA PROC		; `qns16::solver::solve'::`1'::dtor$0
	push	rbp
	push	r13
	sub	rsp, 40					; 00000028H
	mov	r13, rdx
	lea	rbp, QWORD PTR [rdx+64]
	and	rbp, -32				; ffffffffffffffe0H
	lea	rcx, QWORD PTR solution$[rbp]
	call	??1?$vector@HV?$allocator@H@std@@@std@@QEAA@XZ ; std::vector<int,std::allocator<int> >::~vector<int,std::allocator<int> >
	add	rsp, 40					; 00000028H
	pop	r13
	pop	rbp
	ret	0
?dtor$0@?0??solve@solver@qns16@@SANXZ@4HA ENDP		; `qns16::solver::solve'::`1'::dtor$0
text$x	ENDS
; Function compile flags: /Ogtpy
;	COMDAT text$x
text$x	SEGMENT
median_time$ = 0
microseconds$43 = 0
starting_map$44 = 32
$T10 = 64
times_vec$ = 96
_Newcapacity$ = 120
end_$45 = 120
solution$ = 128
timer$46 = 152
__$ArrayPad$ = 168
?dtor$1@?0??solve@solver@qns16@@SANXZ@4HA PROC		; `qns16::solver::solve'::`1'::dtor$1
	push	rbp
	push	r13
	sub	rsp, 40					; 00000028H
	mov	r13, rdx
	lea	rbp, QWORD PTR [rdx+64]
	and	rbp, -32				; ffffffffffffffe0H
	lea	rcx, QWORD PTR times_vec$[rbp]
	call	??1?$vector@NV?$allocator@N@std@@@std@@QEAA@XZ ; std::vector<double,std::allocator<double> >::~vector<double,std::allocator<double> >
	add	rsp, 40					; 00000028H
	pop	r13
	pop	rbp
	ret	0
?dtor$1@?0??solve@solver@qns16@@SANXZ@4HA ENDP		; `qns16::solver::solve'::`1'::dtor$1
text$x	ENDS
; Function compile flags: /Ogtpy
;	COMDAT text$x
text$x	SEGMENT
median_time$ = 0
microseconds$43 = 0
starting_map$44 = 32
$T10 = 64
times_vec$ = 96
_Newcapacity$ = 120
end_$45 = 120
solution$ = 128
timer$46 = 152
__$ArrayPad$ = 168
?dtor$2@?0??solve@solver@qns16@@SANXZ@4HA PROC		; `qns16::solver::solve'::`1'::dtor$2
	push	rbp
	push	r13
	sub	rsp, 40					; 00000028H
	mov	r13, rdx
	lea	rbp, QWORD PTR [rdx+64]
	and	rbp, -32				; ffffffffffffffe0H
	lea	rcx, QWORD PTR timer$46[rbp]
	call	??1hi_res_timer@@QEAA@XZ		; hi_res_timer::~hi_res_timer
	add	rsp, 40					; 00000028H
	pop	r13
	pop	rbp
	ret	0
?dtor$2@?0??solve@solver@qns16@@SANXZ@4HA ENDP		; `qns16::solver::solve'::`1'::dtor$2
text$x	ENDS
END
