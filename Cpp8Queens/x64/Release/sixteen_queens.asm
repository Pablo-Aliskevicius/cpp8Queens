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
$pdata$?solve@solver@qns16@@SANXZ DD imagerel $LN244@solve
	DD	imagerel $LN244@solve+1017
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
$unwind$?dtor$2@?0??solve@solver@qns16@@SANXZ@4HA DQ 0d004420800030801r ; -2.93214e+77
	DD	05002H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?dtor$1@?0??solve@solver@qns16@@SANXZ@4HA DQ 0d004420800030801r ; -2.93214e+77
	DD	05002H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$?dtor$0@?0??solve@solver@qns16@@SANXZ@4HA DQ 0d004420800030801r ; -2.93214e+77
	DD	05002H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$ip2state$?solve@solver@qns16@@SANXZ DQ 0bd0402990002250er ; -8.88629e-15
	DQ	0ad02c20402450603r		; -7.19407e-92
	DD	04700002H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$stateUnwindMap$?solve@solver@qns16@@SANXZ DW 0e06H
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
$unwind$?solve@solver@qns16@@SANXZ DQ 0000f882e00134819r ; 2.15998e-308
	DQ	00011682400107829r		; 2.42071e-308
	DQ	0002b641f002c741fr		; 7.6184e-308
	DQ	00024011f002a341fr		; 5.5639e-308
	DQ	0c012d014e016f018r		; -4.7032
	DD	05010H
	DD	imagerel __GSHandlerCheck_EH4
	DD	imagerel $cppxdata$?solve@solver@qns16@@SANXZ
	DQ	000000040000000aer		; 1.35808e-312
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
END
