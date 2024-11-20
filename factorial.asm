.text

# Initialize
	addi 	$a0, $0, 4		# set n!
	add 	$a1, $a0, $0		# a1 & a2 used for conting in the loops
	add	$a2, $a0, $0
	add	$a3, $a0, $0		
	
# Main factorial function
	beq	$a0, $0, Zero	# Guard for 0! 4! = 4 * 3 * 2 * 1 = 12*2*1 = 24*1= 24
	nop 
	add	$v0, $a0, $0   	# Value out is initially the argument
OuterLoop:
	addi	$a1, $a1, -1    # OuterLoop
	beq	$a1, 1, Stop
	add	$a2, $a1, $0
	add	$a3, $v0, $0	# Store sum from inner loop
	InnerLoop:
		add	$v0, $v0, $a3 # Store the total sum in value out and InnerLoop
		addi	$a2, $a2, -1
		
		beq	$a2, 1, OuterLoop
		nop 
		
		beq	$0, $0, InnerLoop # Force innerLoop unless previous branch did not execute
		nop 
		


# If Zero
Zero:
	addi	$v0, $zero, 1
	

# Stoploop
Stop:
	beq	$0, $0, Stop
	nop
