;
; ���ᮢ��� ��אַ㣮�쭨�
; x1,y1 ---------|
; |              |
; |              |
; |-------------x2,y2
;
  macro rect x1, y1, x2, y2, color
   {
     pushad

     ;mov edx, color
     ; ------------
     ;mov eax, 38
     mov ebx, x1
     shl ebx, 16
     add ebx, x2
     mov ecx, y1
     shl ecx, 16
     add ecx, y1
     ;int 40h
     mcall 38, ebx, ecx, color

     ; ------------
     ;mov eax, 38
     mov ebx, x1
     shl ebx, 16
     add ebx, x2
     mov ecx, y2
     shl ecx, 16
     add ecx, y2
     ;int 40h
     mcall 38, ebx, ecx, color
     ; |
     ; |
     ; |
     ;mov eax, 38
     mov ebx, x1
     shl ebx, 16
     add ebx, x1
     mov ecx, y1
     shl ecx, 16
     add ecx, y2
     ;int 40h
     mcall 38, ebx, ecx, color
     ; |
     ; |
     ; |
     ;mov eax, 38
     mov ebx, x2
     shl ebx, 16
     add ebx, x2
     mov ecx, y1
     shl ecx, 16
     add ecx, y2
     ;int 40h
     mcall 38, ebx, ecx, color
     
     popad
   }

;
;   �뢮� �� ��࠭ ���� � ��ப���
;

scbuff db 80*41 dup 0
; 60 - ����� ��ப�
; 41 - ������⢮ ��ப
;
ind db 0   ; ����騩 ������
;
;
x_s dw 15   ;  ���न���� ������ ���孥�� 㣫�  FIXIT
y_s dw 38  ;

; ���� ��ப� 
SH = 10
              
xlen dw 80   ; ����� ��ப�
;ylen dw 128   ; ������⢮ ��ப
ylen dw 40

;��᫥���� 4 ���� � ��ப� ��������� 梥�

  printbuff:
    pushad

    ;
    ; ����㥬 ���� ��אַ㣮�쭨�
    ;
    ;mov eax, 13
    ;mov ebx, 15*65536+480
    ;mov ecx, 31*65536+418
    ;mov edx, 0FFFFFFh
    ;int 40h
    mcall 13, (15 * 65536 + 480), (31 * 65536 + 418), 0x00FFFFFF



    xor ecx, ecx ; ���稪
    ;xor eax, eax
    xor ebx, ebx
    ;xor edx, edx

  pb_loop:
    xor edx, edx
    xor eax, eax
    mov dl, [ind]     ;  ������
    mov ax, [ylen]
    ;mul edx
    cmp ecx, eax
    ja pr_end
    ;
    ;
    add edx, ecx    ;�ਡ���� ���稪
    xor eax, eax
    mov ax, [ylen]
    cmp edx, eax    ;������ ����� ������⢠ ��ப
    jna @f           ;<=
    sub edx, eax    ;�᫨ �����, ��⠥� � ��砫�
    dec edx
  @@:
    ;
    mov bx, [x_s]    ; ���न��� X
    shl ebx, 16      ;
    push ecx
    mov eax, SH       ; ����塞 ���⮯�������� ��ப�
    imul eax, ecx     ;
    ;mov ecx, eax
    ;xor eax, eax

    ;mov ax , [x_s]
    xor ecx, ecx
    mov cx, [y_s]
    add ecx, eax
    add ebx, ecx    ; ���न��� Y
    ;
    xor eax, eax
    mov ax, [xlen]  ;����� ��ப�
    imul edx, eax         ;�������� ���稪 �� ����� ��ப�, ����砥� ᬥ饭�� �⭮�⥫쭮 ��砫� ����
    ;mov edx, eax
    add edx, scbuff

    xor eax, eax
    mov ax, [xlen]
    sub eax, 4
    xor ecx, ecx
    mov ecx, dword [edx+eax]  ; ��᫥���� 4 ���� � 梥⮬
    or ecx, 0x80000000        ; �뢮���� ASCIIZ
    ;mov eax, 4
   ;mov esi, -1               ; For Menuet
    ;int 40h
    mcall 4, ebx, ecx, edx
    pop ecx
    inc ecx
    jmp pb_loop


  pr_end:
    popad
  ret

;
;  �⫠���� ᮮ�饭��
;
  macro write_debug str 
    {
     local ..string, ..label, ..end, ..loop, ..fin, ..n_inc
     jmp ..label

     ..string db str, 0

    ..label:
     pushad
     xor eax, eax
     xor ebx, ebx
     xor ecx, ecx
     xor edx, edx

     mov bl, [ind]
     mov ax, [xlen]
     imul ebx, eax 
     add ebx, scbuff

     ;
     ; ���� 梥� �뢮��
     ;
     mov edx, ebx
     lea edx, [edx+eax]
     sub edx, 4            ;4 ���� � 梥⮬
     mov dword [edx], dword 0
     xor edx, edx
     

   ..loop:
     mov dl, [..string+ecx]
     cmp dl, 0
     jz ..end
     mov [ebx+ecx], dl
     inc ecx
     jmp ..loop
     
   ..end:
     mov [ebx+ecx], dl
     xor ebx, ebx
     mov bl, [ind]
     cmp bx, [ylen]
     jz ..n_inc
     inc bl
     jmp ..fin
   ..n_inc:
    xor bl, bl

   ..fin:
     mov [ind], bl 
     call printbuff
     ;call draw_window
     popad
    }

;
;  ������
;
  macro draw_button x, y, xlen, ylen, id, str
   {
    pushad
    local ..string, ..label 
    jmp ..label
    ..string db str, 0

    ..label:
    
    mcall 8, (x*65536+xlen), (y*65536+ylen), id, 0x4466aa

    mcall 4, ((x+5)*65536+y+ylen/2-3), 0x80FFFFFF, ..string

    popad
   }

;
; �⫠���� �뢮� ������
;
  macro data_debug str, rg
    {
     pushad
     local ..string, ..end, ..loop, ..strend, ..fin, ..label, ..n_inc
     jmp ..label
     ..string db str, 20h, 0, 0, 0, 0, 0, 0, 0, 0, 0
     ..strend:

     ..label:
     ;xor eax, eax
     ;xor ebx, ebx
     xor ecx, ecx
     xor edx, edx

     mov eax, rg
     lea ebx, [..strend-9]
     call int2str

     xor eax, eax
     xor ebx, ebx

     mov bl, [ind]
     mov ax, [xlen]
     imul ebx, eax 
     add ebx, scbuff

     ;
     ; ���� 梥� �뢮��
     ;
     mov edx, ebx
     lea edx, [edx+eax]
     sub edx, 4            ;4 ���� � 梥⮬
     mov dword [edx], dword 0
     xor edx, edx

   ..loop:
     mov dl, [..string+ecx]
     cmp dl, 0
     jz ..end
     mov [ebx+ecx], dl
     inc ecx
     jmp ..loop
     
   ..end:
     mov [ebx+ecx], dl
     xor ebx, ebx
     mov bl, [ind]
     cmp bx, [ylen]
     jz ..n_inc
     inc bl
     jmp ..fin
    ..n_inc:
     xor bl, bl

    ..fin:
     mov [ind], bl



     call printbuff
     ;call draw_window

     popad
    }

; <--EAX
; -->[ebx]
;
  int2str:
    pushf
    ;push ebx
    push ecx
    push edx
    push esi

    ;xor ebx, ebx
    xor ecx, ecx

   i_loop:
    mov edx, eax
    push ecx
    shl ecx, 2
    mov esi, 28
    sub esi, ecx
    mov ecx, esi
    shr edx, cl
    pop ecx
    and dl, 0fh    ;�⤥���� ����訥 4 ���
    cmp dl, 0Ah    ;�㪢�
    jnc @f
    or dl, 0x30
    jmp i_n1

  @@:
    sub dl, 9
    or dl, 0x40
  i_n1:
    mov [ebx+ecx], dl
    inc ecx
    cmp ecx, 8
    jnz i_loop

    
    pop esi
    pop edx
    pop ecx
    ;pop ebx
    popf
  ret



;
;  ��� �뢮�� ᮮ�饭��
;  [eax] <-- null-terminated string
;   ebx <-- color
;
  writemsg:
    pushad
     xor edi, edi

     
   wm_loop:
     xor esi, esi            ; �᫨ 1 - ���� �� ᨬ���� � ��ப�
     lea eax, [eax+edi]

     push ebx
     push eax

     xor eax, eax
     xor ebx, ebx


     mov bl, [ind]
     mov ax, [xlen]
     imul ebx, eax 
     add ebx, scbuff




     ;
     ; ��������� ��ப� ᮮ�饭�� �� ��᪮�쪮 ��ப �� xlen-4 (�.� � ���� ������� ᫮�� - 梥� ��ப�)
     ;


     pop eax
     mov edx, eax
     call strlen

     movzx ecx, [xlen]
     cmp eax, ecx
     jc @f            ;<

     movzx edi, [xlen]
     lea edi, [edi-4]

     mov ecx, eax
     inc esi


    @@:
     

     mov eax, edx
     call strcpy

     mov [ebx+ecx], byte 0x00

     xor eax, eax
     mov ax, [xlen]
     sub eax, 4
     pop ecx
     mov dword [ebx+eax], ecx  ; ��᫥���� 4 ���� � 梥⮬


     xor eax, eax
     mov al, [ind]
     cmp ax, [ylen]
     jz @f
     inc al
     jmp ..fin
   @@:
    xor al, al

   ..fin:
     mov [ind], al
     mov ebx, ecx   ; 梥�
     mov eax, edx   ; 㪠��⥫� �� ��ப�

     cmp esi, 0
     jnz wm_loop

                          
     call printbuff

    popad
  ret

;
;



; <--EAX       ��ॢ�� �� 16 � 10 ���
; -->[ebx]     ��ப�
; --> eax �����
  int2strd:
    pushf
    ;push ebx
    push ecx
    push edx
    push esi
    ;push edi

    ;xor ebx, ebx
    xor ecx, ecx
    mov esi, 10
    

   id_loop:
    xor edx, edx
    
    div esi

    and dl, 0fh    ;�⤥���� ����訥 4 ���
    or dl, 0x30
    
    mov [ebx+ecx], dl
    cmp eax, 10
    inc ecx
    jnc id_loop

    and al, 0fh    ;�⤥���� ����訥 4 ���
    or al, 0x30

    mov [ebx+ecx], al
    ;mov [ebx+ecx+1], byte 0
    inc ecx
    mov eax, ecx
    ; ��ॢ����� ����祭��� ��ப�
    ;
    ;xor edx, edx

    ;mov esi, 2
    ;div esi
    shr eax, 1

    xor edx, edx

   id_loop2:
    cmp eax, 0
    jz id_end


    mov dl, [ebx+eax-1]
    lea esi, [ebx+ecx]
    sub esi, eax
    mov dh, [esi]

    mov [ebx+eax-1], dh
    mov [esi], dl

    dec eax
    jmp id_loop2




    id_end:
    mov eax, ecx

    ;pop edi
    pop esi
    pop edx
    pop ecx
    ;pop ebx
    popf
  ret

;
;
;
x_bb dw 550                ;  ���न���� ������ ���孥�� 㣫� 
y_bb dw 30                ;
;
bb_width dw 100           ; ��ਭ� ������
bb_height dw 12          ; ���� ������
;
bb_dist  dw 6           ; �����ﭨ� ����� ��������

; �����䨪���� ������ ��稭�� � 100
;
;  ��� ��
;
;
  buttonbox:
    pushad
    pushf
 
    xor ecx, ecx
    xor eax, eax
    xor ebx, ebx

  bb_loop:

    ; �஢��塞 ���� ���� 㨭�, �᫨ 0, ������ �ᮢ��� �� ����
    ;mov ebx, NAME_LEN
    ;imul ebx, ecx

    ;mov al, [names+ebx]
    ;cmp al, 0
    ;jz bb_end

    mov ebx, UIN_LEN
    imul ebx, ecx

    mov al, [uins + ebx]
    cmp al, 0
    jz bb_end




    ; ��㥬 ������
    ; ���� ������ �� ����� UIN
    mov ebx, 4
    imul ebx, ecx
    mov eax, [stats+ebx]     ; � ���襬 ᫮�� �������⥫�� �����
    cmp ax, -1
    jz bb_offline
    cmp ax, 1
    jz bb_away
    cmp ax, 2
    jz bb_dnd
    cmp ax, 4
    jz bb_na
    cmp ax, 10h
    jz bb_bisy
    cmp ax, 20h
    jz bb_free4chat
    cmp ax, 100h
    jz bb_invis
    ; Online
    mov esi, 0x4466AA
    jmp bb_dr

  bb_offline:
    mov esi, 0xBEBEBE
    jmp bb_dr

  bb_away:
    mov esi, 0x14dcc3
    jmp bb_dr

  bb_dnd:
    mov esi, 0x14dcc3
    jmp bb_dr

  bb_na:
    mov esi, 0x14dcc3
    jmp bb_dr

  bb_bisy:
    mov esi, 0x14dcc3
    jmp bb_dr

  bb_free4chat:
    mov esi, 0x2233FF
    jmp bb_dr

  bb_invis:
    mov esi, 0xD0D0D0

  bb_dr:

    mov bx, [x_bb]
    shl ebx, 16
    mov bx, [bb_width]
    ;push ecx
    mov edx, ecx
    lea edx, [edx+100]      ; ID
    mov edi, ecx
    mov cx, [y_bb]
    movzx eax, [bb_height]
    add ax, [bb_dist]
    imul eax, edi
    add ecx, eax
    shl ecx, 16
    mov cx, [bb_height]
    mov eax, 8
    int 40h
     
    ;
    ; ������� �� ������
    ;
    add ebx, 50000h     ; ���饭�� �� ������ ���
    shr ecx, 16
    movzx eax, [bb_height]
    shr eax, 1          ;  /2
    sub eax, 3          ; ᬥ饭�� ����� �� 業��
    add ecx, eax        ; + �������� ࠧ��� ������
    mov bx, cx
    mov ecx, 0x80FFFFFF ; ����
    mov edx, names
    mov esi, NAME_LEN
    imul esi, edi
    add edx, esi
    ;mov esi, -1
    mov eax, 4
    int 40h

    mov ecx, edi
    inc ecx
    cmp ecx, UINS
    ja bb_end
    jmp bb_loop


  bb_end:
    popf
    popad
  ret
;
; ���ᨢ � UIN
;
UIN_LEN = 11            ; �����
UINS    = 22            ; ������⢮
;
uins db UIN_LEN*UINS dup 0
; 
; ���ᨢ � ����ᠬ�
;
stats dd UINS dup -1
;
; ���ᨢ � �������
;
NAME_LEN = 30

names db NAME_LEN*UINS dup 0

;
;
U1 db '405577261',0
U2 db '455395049',0
U3 db '488118046',0
;
; ����㧪� ���ᨢ� UIN
;
; FIXME �㤥� 㤠����
  loaduin:
    pushad
    mov eax, U1
    mov ebx, uins
    mov ecx, 9
    call strcpy

    mov ebx, names
    call strcpy

    mov eax, U2
    mov ebx, uins+UIN_LEN
    mov ecx, 9
    call strcpy

    mov ebx, names+NAME_LEN
    call strcpy

    mov eax, U3
    mov ebx, uins+UIN_LEN*2
    mov ecx, 9
    call strcpy

    mov ebx, names+NAME_LEN*2
    call strcpy


    popad
  ret

;
; �㭪�� ��� ����㧪� �� ������ � ����ᠬ�
; eax    <--   ����� 㨭� �� ���浪� � ���ᨢ� uins, ��稭�� � 0
; [ebx]  <--   㪠��⥫� �� null-terminated ��ப� � ���� ������
; ecx    <--   ���� �����
;                                  
   loadbb:
     pushad
     pushf
     ;
     ; �஢��塞 �����
     ;
     cmp eax, UINS
     jnc loadbb_end        ;>=


     ;
     ; ����塞 ������
     ;
     push ecx
     push ebx
     push eax

     lea edx, [eax+100]
     or edx, 0x80000000
     mov eax, 8
     int 40h

     ;
     ; ���࠭塞 ���� ����� � ���ᨢ� ����ᮢ
     ;
     pop eax
     mov edx, 4                 ; DWORD
     imul edx, eax
     mov [stats+edx], ecx
     ;
     ; ���࠭塞 ����� ���
     ;
     mov edi, eax               ; edi <-- Uin number
     pop eax                    ; <-- offset of string
     mov ebx, eax
     call strlen
     mov ecx, eax               ; Len


     mov eax, ebx               ;Source
     mov edx, NAME_LEN
     imul edx, edi
     lea ebx, [names+edx]       ; Dest
     call strcpy
     mov [names+edx+ecx], byte 0

     xchg edi, eax              ; eax - ��⨪, edi - 㪠��⥫� �� ��ப�
     
     pop ecx
     push edi
     ; ��㥬 ������
     ; ���� ������ �� ����� UIN
    
     cmp cx, -1
     jz l_offline
     cmp cx, 1
     jz l_away
     cmp cx, 2
     jz l_dnd
     cmp cx, 4
     jz l_na
     cmp cx, 10h
     jz l_bisy
     cmp cx, 20h
     jz l_free4chat
     cmp cx, 100h
     jz l_invis
     ; Online
     mov esi, 0x4466AA
     jmp l_dr

   l_offline:
     mov esi, 0xBEBEBE
     jmp l_dr

   l_away:
     mov esi, 0x14dcc3
     jmp l_dr

   l_dnd:
     mov esi, 0x14dcc3
     jmp l_dr

   l_na:
     mov esi, 0x14dcc3
     jmp l_dr

   l_bisy:
     mov esi, 0x14dcc3
     jmp l_dr

   l_free4chat:
     mov esi, 0x2233FF
     jmp l_dr

   l_invis:
     mov esi, 0xD0D0D0

   l_dr:

     mov bx, [x_bb]
     shl ebx, 16
     mov bx, [bb_width]
     ;push ecx
     mov edx, eax
     lea edx, [edx+100]      ; ID
     mov edi, eax
     mov cx, [y_bb]
     movzx eax, [bb_height]
     add ax, [bb_dist]
     imul eax, edi
     add ecx, eax
     shl ecx, 16
     mov cx, [bb_height]
     mov eax, 8
     int 40h
     
    ;
    ; ������� �� ������
    ;
    add ebx, 50000h     ; ���饭�� �� ������ ���
    shr ecx, 16
    movzx eax, [bb_height]
    shr eax, 1          ;  /2
    sub eax, 3          ; ᬥ饭�� ����� �� 業��
    add ecx, eax        ; + �������� ࠧ��� ������
    mov bx, cx
    mov ecx, 0x80FFFFFF ; ����
    pop edx
   mov esi, -1
    mov eax, 4
    int 40h



   loadbb_end:

     popf
     popad
   ret

;
;
; �⫠��筠� �㭪�� - �뢮��� ������� �����
; EAX - 㪠��⥫� �� �������
; EBX - ������⢮ ����
;

 print_mem:
   push eax
   push ebx
   push ecx
   push edx
   push esi

   xor ecx, ecx
   xor esi, esi


 pm_loop:
   cmp ecx, ebx
   jz pm_exit


   mov dl, [eax + ecx]
   shr dl, 4        ;�������� �� 4 ࠧ�鸞 ��ࠢ�

   cmp dl, 0x0A
   jnb pm_chars

   or dl, 0x30
   jmp pm_write

  pm_chars:
   add dl, 0x37

  pm_write:
   mov [membuff + esi], dl
   inc esi
   ;
   ;

   mov dl, [eax + ecx]
   and dl, 0x0F

   cmp dl, 0x0A
   jnb pm_chars2

   or dl, 0x30
   jmp pm_write2

  pm_chars2:
   add dl, 0x37

  pm_write2:
   mov [membuff + esi], dl
   inc esi


   mov [membuff + esi], 0x20
   inc esi

   cmp esi, MEMBUFF_SIZE - 2
   jb pm_nwrite
   ; 
   ; �뢥�� ����
   mov [membuff + esi], byte 0

   push eax
   push ebx

   mov eax, membuff
   xor ebx, ebx
   xor esi, esi

   call writemsg

   pop ebx
   pop eax

  pm_nwrite:

   inc ecx
   jmp pm_loop



  pm_exit:

   mov [membuff + esi], byte 0


   mov eax, membuff
   xor ebx, ebx
   xor esi, esi

   call writemsg

   pop esi
   pop edx
   pop ecx
   pop ebx
   pop eax
 ret
