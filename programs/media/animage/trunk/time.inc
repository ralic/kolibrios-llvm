;---------------------------------------------------------
;---------- get time in 1/100 secunds---------------------
;---------------------------------------------------------
Clock:
	  mov eax,26
	  mov ebx,9
	  mcall
	  mov [time],eax

	  ret

