.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main

.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

main:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp

  addi $sp, $sp, -64
  lw $t0, -4($fp)
  li $t0, 3
  sw $t0, -4($fp)
  
  lw $t0, -8($fp)
  lw $t1, -4($fp)
  move $t0, $t1
  sw $t0, -8($fp)
  
  lw $t0, -24($fp)
  li $t0, 0
  sw $t0, -24($fp)
  
  lw $t0, -28($fp)
  lw $t1, -24($fp)
  move $t0, $t1
  sw $t0, -28($fp)

label0:
  lw $t0, -28($fp)
  lw $t1, -8($fp)
  blt $t0, $t1 label1
  j label2
label1:
  lw $t0, -32($fp)
  addi $t1, $fp, -12
  move $t0, $t1
  sw $t0, -32($fp)

  lw $t0, -36($fp)
  lw $t1, -28($fp)
  move $t0, $t1
  sw $t0, -36($fp)

  lw $t0, -36($fp)
  li $t1, 4
  lw $t2, -36($fp)
  mul $t0, $t1, $t2
  sw $t0, -36($fp)
 
  lw $t0, -40($fp)
  lw $t1, -32($fp)
  lw $t2, -36($fp)
  add $t0, $t1, $t2
  sw $t0, -40($fp)
  
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal read
  lw $ra, 0($sp)
  addi $sp, $sp, 4
 
  lw $t0, -44($fp)
  move $t0, $v0
  sw $t0, -44($fp)
  
  lw $t0, -40($fp)
  lw $t1, -44($fp)
  sw $t1, 0($t0)
  sw $t0, -40($fp)
  
  lw $t0, -48($fp)
  lw $t1, -28($fp)
  move $t0, $t1
  sw $t0, -48($fp)

  lw $t0, -52($fp)
  li $t0, 1
  sw $t0, -52($fp)
  
  lw $t0, -56($fp)
  lw $t1, -48($fp)
  lw $t2, -52($fp)
  add $t0, $t1, $t2
  sw $t0, -56($fp)
  
  lw $t0, -28($fp)
  lw $t1, -56($fp)
  move $t0, $t1
  sw $t0, -28($fp)
  j label0
label2:
  lw $t0, -60($fp)
  li $t0, 0
  sw $t0, -60($fp)
  
  lw $t0, -60($fp)
  move $v0, $t0
  addi $sp, $sp, 64
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
