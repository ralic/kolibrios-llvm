;   SQ_game for MenuetOS
;   Author:    Alexei Ershov aka ealex
;   E-mail:    e-al[at]yandex[dot]ru
;   Fidonet:   2:469/335.38
; slightly optimized by diamond
; english translation by diamond (two strings in end of source)


	BtnSize     equ 32	; ࠧ��� ������
	BtnSpace    equ 10	; �஬���⮪ ����� ��������
	NofBtn	    equ 4   ; ࠧ��� ���� (3-15)
	Color1	    equ 0xcc0000
	Color2	    equ 0x00cc00

	FieldSize   = (BtnSize+BtnSpace)*NofBtn + BtnSpace*3
	center	    = FieldSize / 2


  use32 	     ; ������� 32-���� ०�� ��ᥬ����
  org	 0x0	     ; ������ � ���

  db	 'MENUET01'  ; 8-����� �����䨪��� MenuetOS
  dd	 0x01	     ; ����� ��������� (�ᥣ�� 1)
  dd	 START	     ; ���� ��ࢮ� �������
  dd	 I_END	     ; ࠧ��� �ணࠬ��
  dd	 0x1000      ; ������⢮ �����
  dd	 0x1000      ; ���� ���設� �����
  dd	 0x0	     ; ���� ���� ��� ��ࠬ��஢ (�� �ᯮ������)
  dd	 0x0	     ; ��१�ࢨ஢���

include 'lang.inc'
include 'MACROS.INC' ; ������ �������� ����� ��ᥬ����騪��!


START:
    call Game_Init
red:
    call Draw_Window

still:
    mcall 10		; �㭪�� 10 - ����� ᮡ���
	dec	eax
	jz	red
	dec	eax
	jnz	button

 key:		       ; ����� ������ �� ���������
	mov	al, 2
	int	0x40
	jmp	still
;---------------------------------------------------------------------

button:
	mov	al, 17	; 17 - ������� �����䨪��� ����⮩ ������
	int	0x40
    cmp   ah, 1 	; �᫨ ����� ������ � ����஬ 1, ��室��
    je	  .exit

    cmp   ah, 2 	; �᫨ ����� ������ 2, ��稭��� ����� ����
    je	  START

      ; ���� ������ �� ����
	inc	dword [moves]	  ; 㢥��稢��� �᫮ 室��

	shr	eax, 8		  ; � eax �����䨪��� ����⮩ ������

	mov	dh, NofBtn
	div	dh	      ; al = ax(����� ������) / NofBtn
			      ; ah = ���⮪  (�. ��㭮�)
	mov	ebx, eax      ; ��࠭�� ��⭮� � ���⮪ � ebx

      ; al\ah 0   1    2    3
      ;     +-+  +-+  +-+  +-+
      ;  1  |4|  |5|  |6|  |7|
      ;     +-+  +-+  +-+  +-+

      ;     +-+  +-+  +-+  +-+
      ;  2  |8|  |9|  |10  |11
      ;     +-+  +-+  +-+  +-+

      ;     +-+  +-+  +-+  +-+
      ;  3  |12  |13  |14  |15
      ;     +-+  +-+  +-+  +-+

      ;     +-+  +-+  +-+  +-+
      ;  4  |16  |17  |18  |19
      ;     +-+  +-+  +-+  +-+

; ���塞 梥� �⮫����
	shr	eax, 8	      ; eax = ah
	mov	edi, field
	add	edi, eax    ; edi 㪠���� �� ����� ������ � �᪮��� �⫡�
	mov	ecx, NofBtn
@@:
	not	byte[edi]    ; ���塞 梥� �� �ᥬ �⮫����
	dec	ecx
	add	edi, NofBtn
	test	ecx, ecx
	jnz	@r

; ���塞 梥� ��ப�
	mov	eax, ebx      ; ����⠭�������� eax
	dec	eax
	mov	dl, NofBtn
	mul	dl	      ; ax = al * NofBtn
	mov	edi, field
	add	edi, eax     ; edi 㪠���� �� ����� ������ � �᪮��� ��ப�
	mov	ecx, NofBtn
@@:
	not	byte [edi]    ; ���塞 梥� �� �ᥩ ��ப�
	dec	ecx
	inc	edi
	test	ecx, ecx
	jnz	@r
; ���� ����⮩ ������ ������ 2 ࠧ� (����� ��ࠡ��뢠�� ��ப� � �⮫���),
; �.�. ��⠫�� ��ࢮ��砫��
; ���塞 梥� ����⮩ ������
	shr	ebx, 8	      ; ebx = ���⮪ (ah �� ��㭪�)
	not	byte [field+eax+ebx]
	jmp	red



  .exit:
    mcall -1		; ���� ����� �ணࠬ��


;----------------------------------------------------------------------------
;               Draw_Window
;----------------------------------------------------------------------------
Draw_Window:

    mcall 12, 1
    mcall 0, 200*65536+FieldSize,\
	     200*65536+FieldSize+20+20+20,\
	     0x14AABBCC, 0x805080D0, header

    mcall 8, BtnSpace*2*65536 + 83,\
	     (FieldSize+BtnSpace+20)*65536+ 15, 2
    mcall 4, (BtnSpace*2+3)*65536+(BtnSize+BtnSpace)*NofBtn+BtnSpace*4+20+4,\
	     0x10DDEEFF, strNew, strNew_e - strNew
    mcall 4, (center - 25) * 65536 + 30, 0, strMovs, strMovs_e - strMovs
    mcall 47, 4*65536+1, moves, (center + 2 ) * 65536 + 30, 0



	mov	ecx, (20+20+BtnSpace-BtnSize)*65536+BtnSize
	mov	edx, NofBtn	      ; �����䨪��� ��ࢮ� ������ �� ����
				      ; � ��㭮�

buttons:			      ; ��㥬 �����窨
	mov	eax, edx
	mov	dh, NofBtn
	div	dh	     ; al = ax / dh, ah - ���⮪
	mov	dh, 0
	test	ah, ah	     ; �᫨ ah = 0, ��稭��� ���� ��
	jnz	@f
	add	ecx, (BtnSize+BtnSpace)*65536
	mov	ebx, BtnSpace*2*65536+BtnSize
@@:
	mov	esi, Color1		       ; ���� �� �����
	cmp	byte [field-NofBtn+edx], 0	; field-NofBtn+�����_������
	jz	@f				; ������ � �� 梥�
	mov	esi, Color2
@@:
	mcall 8,
	add	ebx, (BtnSize+BtnSpace)*65536
	inc	edx			   ; ���室�� � ᫥���饩 ������
	cmp	edx, NofBtn*(NofBtn+1)
	jb	buttons

    mcall 12, 2 		   ; �㭪�� 12: ᮮ���� �� �� ���ᮢ�� ����

ret

;----------------------------------------------------------------------------
;               Game_Init
;----------------------------------------------------------------------------
Game_Init:
	and	[moves], 0	       ; ���㫨� ������⢮ 室��

	call	random		       ; � eax ��砩��� �᫮
	mov	ecx, NofBtn * NofBtn   ; � ecx ���-�� ������ �� ����
	mov	bh, 0		       ; ��� ������ ������ �㤥� ����
				       ; 1 ��� �� eax, bh - ᪮�쪮
button_init:			       ; ��� 㦥 �ᯮ�짮����
	shr	eax, 1		 ; ����訩 ��� �������� � 䫠� CF
				 ; � ����ᨬ��� �� ��� ���祭��
				 ; � bl �����뢠���� -1 ��� 0
	sbb	bl, bl
	mov	[field + ecx - 1], bl

	inc	bh
	cmp	bh, 32		 ; �᫨ �ᯮ�짮���� �� 32 ��� �� eax
	jb	@f		 ; ��� ������� ����� ��砩��� �᫮
	call	random
	mov	bh, 0
@@:
	loop	button_init

ret

;----------------------------------------------------------------------------
;     random - ���� �� ����
;----------------------------------------------------------------------------
random: 	mov eax,[generator]
		add eax,-43ab45b5h
		ror eax,1
		xor eax,32c4324fh
		ror eax,1
		mov [generator],eax
		; --- IVAN ---
		mov eax,26
		mov ebx,9
		int 0x40
		xor eax,0xdeadbeef
		add eax,[generator]
		; --- IVAN ---
		ret

;============================================================================

field		db NofBtn * NofBtn dup(0)
moves		dd 0
generator	dd 0

if lang eq ru
header		db 'CQ game',0
header_e:
strMovs 	db '���'	; '���'
strMovs_e:
strNew		db '����� ���' ; '����� ���'
strNew_e:
else
header		db 'CQ game',0
header_e:
strMovs 	db 'MOVE'
strMovs_e:
strNew		db ' New game'
strNew_e:
end if



I_END:				   ; ��⪠ ���� �ணࠬ��
