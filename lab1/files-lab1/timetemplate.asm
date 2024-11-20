  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,2
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #
hexasc:
   andi $t0,$a0,0xf  # check least 4 significant values and ignore other bits.
   ble $t0,9,numbers
   nop
   ble $t0,15,letters
   nop
   
   
numbers:
   addi $v0,$t0,0x30
   jr $ra
   nop
letters:
   
   addi $v0,$t0,0x37
   jr $ra
   nop
        
time2string:
        PUSH    ($s0)
        PUSH    ($s1)
        PUSH    ($ra)       

        add     $s0,$0,$a0     
        add     $s1,$0,$a1      

        andi    $t0,$s1,0xf000    
        srl     $a0,$t0,12    
        jal     hexasc     
        nop
        sb      $v0, 0($s0)     

        andi    $t1,$s1,0x0f00  
        srl     $a0,$t1,8       
        jal     hexasc      
        nop
        sb      $v0,1($s0)     

        li      $t3,0x3A        
        sb      $t3,2($s0)


        andi    $t2,$s1,0x00f0
        srl     $a0,$t2,4
        jal     hexasc
        nop
        sb      $v0,3($s0)


        move    $a0,$s1
        jal     hexasc
        nop
        sb      $v0, 4($s0)

        #andi	$t5, $s1, 0x00ff #0x000f
        #beq	$t5, 0x02, branch
        andi	$t5, $s1, 0x0001
        beq	$t5, 0, branch0
        nop
        andi	$t5, $s1, 0x0001
        beq	$t5, 1, branch1
        nop
goback:
        li      $t4,0x00        
        sb      $t4,6($s0)



        POP     ($ra)
        POP     ($s1)
        POP     ($s0)

        jr       $ra
        nop
delay:
	move $t0,$a0
	addi $t2,$0,4711
while:
	ble $t0,$0,done
	nop
	addi $t0,$t0,-1 #sub $t0
	addi $t1,$0,0 #i 
for:
	slt $t3,$t1,$t2
	beq $t3,$0,done2
	nop
	addi $t1,$t1,1
	j for
	nop
done2:
	j while
	nop
done:
	jr $ra
	nop
branch:
	li	$t6,0x54
	sb	$t6,4($s0)
	li	$t7,0x57
	sb	$t7,5($s0)
	li	$t8,0x4F
	sb	$t8,6($s0)
	li	$t4,0x00
	sb	$t4,7($s0)
	j	goback
	nop
branch1:
	li	$t6,0x44
	sb	$t6, 5($s0)
	j	goback
	nop
branch0:
	li	$t6,0x45
	sb	$t6, 5($s0)
	j	goback
	nop
	
	
