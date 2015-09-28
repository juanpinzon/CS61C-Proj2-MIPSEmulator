.data
passstring:     .asciiz "all tests passed!\n"
fail1string:    .asciiz "test #"
fail2string:    .asciiz " failed!\n"

.text

# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

#
#  The header ends here, and code goes below
#

# print a string.  (note the %lo syntax only works if the string is
# in the lower 32KB of memory, which it always will be for us)

        # test #3: subu
        li    $30, 3
        li    $3, 1234
        li    $5, 5678
        subu  $4, $5, $3
        li    $5, 4444
        bne   $4, $5, fail
        
        # test #4: or
        li    $30, 4
        li    $3, 1234
        li    $5, 5678
        or    $4, $3, $5
        li    $5, 5886
        bne   $4, $5, fail
        
        # test #5: xor
        li    $30, 5
        li    $3, 1234
        li    $5, 5678
        xor   $4, $3, $5
        li    $5, 4860
        bne   $4, $5, fail
        
        # test #6: nor
        li    $30, 6
        li    $3, 1234
        li    $5, 5678
        nor   $4, $3, $5
        li    $5, 4294961409
        bne   $4, $5, fail
        
        # test #7: slt
        li    $30, 7
        li    $3, 1234
        li    $5, -5678
        slt   $4, $3, $5
        li    $5, 0
        bne   $4, $5, fail
        
        # test #8: sltu
        li    $30, 8
        li    $3, 1234
        li    $5, 5678
        slt   $4, $3, $5
        li    $5, 1
        bne   $4, $5, fail
        
        # test #9: slti
        li    $30, 9
        li    $3, 1234
        slti  $4, $3, 5678
        li    $5, 1
        bne   $4, $5, fail
        
        # test #10: sltiu
        li    $30, 10
        li    $3, 1234
        slti  $4, $3, -5678
        li    $5, 0
        bne   $4, $5, fail
        
        # test #11: andi
        li    $30, 11
        li    $3, 1234
        andi  $4, $3, 5678
        li    $5, 1026
        bne   $4, $5, fail
        
        # test #12: xori
        li    $30, 12
        li    $3, 1234
        xori  $4, $3, 5678
        li    $5, 4860
        bne   $4, $5, fail
                
        # test #13: lw
        li    $30, 13
        la    $4, foo
        lw    $4, 0($4)
        li    $5, 911
        bne   $4, $5, fail        
        
        # test #13: lw
        li    $30, 13
        la    $4, foo
        lw    $4, 0($4)
        li    $5, 911
        bne   $4, $5, fail 
        
        # test #14: lb
        li    $30, 14
        la    $4, 1
        lb    $4, a($4)
        li    $5, 48
        bne   $4, $5, fail 
        
	# test #15: mflo  and mult    
	li    $30, 15
        ori   $4, $0, 12       # put x into $4
        ori   $5, $0,  5       # put 5 into $5
        mult  $5, $4           # lo = 5x
        mflo  $4               # $4 = 5x
        addiu $4, $4, -74       # $4 = 5x - 74
        li    $5, -14
        bne   $4, $5, fail 
        

                
                
	ori   $a0, %lo(str)
	ori   $v0, $zero, 4
	syscall
pass:
        la $a0, passstring
        li $v0, 4
        syscall
        b done
fail:
        ori $a0, $0, %lo(fail1string)
        ori $v0, $0, 4
        syscall

        ori  $a0, $30, 0
        ori  $v0, $0, 1
        syscall

        ori $a0, $0, %lo(fail2string)
        ori $v0, $0, 4
        syscall

done:
        ori $v0, $zero, 10
        syscall

.end _start

.data
str: .asciiz "Hello, world!\n"
foo: .word 911
a: .asciiz "2021"
