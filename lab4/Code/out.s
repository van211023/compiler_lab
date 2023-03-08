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

fun:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -28
  lw $t0, -4($fp)
  lw $t1, 8($fp)
  move $t0, $t1
  sw $t0, -4($fp)
addi $sp, $sp, -16
sw $a0, 0($sp)
sw $a1, 4($sp)
sw $a2, 8($sp)
sw $a3, 12($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $t0, -4($fp)
  move $a0, $t0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
lw $a0, 0($sp)
lw $a1, 4($sp)
lw $a2, 8($sp)
lw $a3, 12($sp)
addi $sp, $sp, 16
  lw $t0, -8($fp)
  lw $t1, 12($fp)
  move $t0, $t1
  sw $t0, -8($fp)
addi $sp, $sp, -16
sw $a0, 0($sp)
sw $a1, 4($sp)
sw $a2, 8($sp)
sw $a3, 12($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $t0, -8($fp)
  move $a0, $t0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
lw $a0, 0($sp)
lw $a1, 4($sp)
lw $a2, 8($sp)
lw $a3, 12($sp)
addi $sp, $sp, 16
  lw $t0, -12($fp)
  move $t0, $a3
  sw $t0, -12($fp)
addi $sp, $sp, -16
sw $a0, 0($sp)
sw $a1, 4($sp)
sw $a2, 8($sp)
sw $a3, 12($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $t0, -12($fp)
  move $a0, $t0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
lw $a0, 0($sp)
lw $a1, 4($sp)
lw $a2, 8($sp)
lw $a3, 12($sp)
addi $sp, $sp, 16
  lw $t0, -16($fp)
  move $t0, $a2
  sw $t0, -16($fp)
addi $sp, $sp, -16
sw $a0, 0($sp)
sw $a1, 4($sp)
sw $a2, 8($sp)
sw $a3, 12($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $t0, -16($fp)
  move $a0, $t0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
lw $a0, 0($sp)
lw $a1, 4($sp)
lw $a2, 8($sp)
lw $a3, 12($sp)
addi $sp, $sp, 16
  lw $t0, -20($fp)
  move $t0, $a1
  sw $t0, -20($fp)
addi $sp, $sp, -16
sw $a0, 0($sp)
sw $a1, 4($sp)
sw $a2, 8($sp)
sw $a3, 12($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $t0, -20($fp)
  move $a0, $t0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
lw $a0, 0($sp)
lw $a1, 4($sp)
lw $a2, 8($sp)
lw $a3, 12($sp)
addi $sp, $sp, 16
  lw $t0, -24($fp)
  move $t0, $a0
  sw $t0, -24($fp)
addi $sp, $sp, -16
sw $a0, 0($sp)
sw $a1, 4($sp)
sw $a2, 8($sp)
sw $a3, 12($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  lw $t0, -24($fp)
  move $a0, $t0
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
lw $a0, 0($sp)
lw $a1, 4($sp)
lw $a2, 8($sp)
lw $a3, 12($sp)
addi $sp, $sp, 16
  addi $sp, $sp, 28
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
main:
  addi $sp, $sp, -4
  sw $fp, 0($sp)
  move $fp, $sp
  addi $sp, $sp, -228
  lw $t0, -28($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -28($fp)
  lw $t0, -32($fp)
  li $t0, 0
  sw $t0, -32($fp)
  lw $t0, -32($fp)
  li $t1, 4
  lw $t2, -32($fp)
  mul $t0, $t1, $t2
  sw $t0, -32($fp)
  lw $t0, -36($fp)
  lw $t1, -28($fp)
  lw $t2, -32($fp)
  add $t0, $t1, $t2
  sw $t0, -36($fp)
  lw $t0, -40($fp)
  li $t0, 1
  sw $t0, -40($fp)
  lw $t0, -36($fp)
  lw $t1, -40($fp)
  sw $t1, 0($t0)
  sw $t0, -36($fp)
  lw $t0, -44($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -44($fp)
  lw $t0, -48($fp)
  li $t0, 1
  sw $t0, -48($fp)
  lw $t0, -48($fp)
  li $t1, 4
  lw $t2, -48($fp)
  mul $t0, $t1, $t2
  sw $t0, -48($fp)
  lw $t0, -52($fp)
  lw $t1, -44($fp)
  lw $t2, -48($fp)
  add $t0, $t1, $t2
  sw $t0, -52($fp)
  lw $t0, -56($fp)
  li $t0, 2
  sw $t0, -56($fp)
  lw $t0, -52($fp)
  lw $t1, -56($fp)
  sw $t1, 0($t0)
  sw $t0, -52($fp)
  lw $t0, -60($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -60($fp)
  lw $t0, -64($fp)
  li $t0, 2
  sw $t0, -64($fp)
  lw $t0, -64($fp)
  li $t1, 4
  lw $t2, -64($fp)
  mul $t0, $t1, $t2
  sw $t0, -64($fp)
  lw $t0, -68($fp)
  lw $t1, -60($fp)
  lw $t2, -64($fp)
  add $t0, $t1, $t2
  sw $t0, -68($fp)
  lw $t0, -72($fp)
  li $t0, 3
  sw $t0, -72($fp)
  lw $t0, -68($fp)
  lw $t1, -72($fp)
  sw $t1, 0($t0)
  sw $t0, -68($fp)
  lw $t0, -76($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -76($fp)
  lw $t0, -80($fp)
  li $t0, 3
  sw $t0, -80($fp)
  lw $t0, -80($fp)
  li $t1, 4
  lw $t2, -80($fp)
  mul $t0, $t1, $t2
  sw $t0, -80($fp)
  lw $t0, -84($fp)
  lw $t1, -76($fp)
  lw $t2, -80($fp)
  add $t0, $t1, $t2
  sw $t0, -84($fp)
  lw $t0, -88($fp)
  li $t0, 4
  sw $t0, -88($fp)
  lw $t0, -84($fp)
  lw $t1, -88($fp)
  sw $t1, 0($t0)
  sw $t0, -84($fp)
  lw $t0, -92($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -92($fp)
  lw $t0, -96($fp)
  li $t0, 4
  sw $t0, -96($fp)
  lw $t0, -96($fp)
  li $t1, 4
  lw $t2, -96($fp)
  mul $t0, $t1, $t2
  sw $t0, -96($fp)
  lw $t0, -100($fp)
  lw $t1, -92($fp)
  lw $t2, -96($fp)
  add $t0, $t1, $t2
  sw $t0, -100($fp)
  lw $t0, -104($fp)
  li $t0, 5
  sw $t0, -104($fp)
  lw $t0, -100($fp)
  lw $t1, -104($fp)
  sw $t1, 0($t0)
  sw $t0, -100($fp)
  lw $t0, -108($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -108($fp)
  lw $t0, -112($fp)
  li $t0, 5
  sw $t0, -112($fp)
  lw $t0, -112($fp)
  li $t1, 4
  lw $t2, -112($fp)
  mul $t0, $t1, $t2
  sw $t0, -112($fp)
  lw $t0, -116($fp)
  lw $t1, -108($fp)
  lw $t2, -112($fp)
  add $t0, $t1, $t2
  sw $t0, -116($fp)
  lw $t0, -120($fp)
  li $t0, 6
  sw $t0, -120($fp)
  lw $t0, -116($fp)
  lw $t1, -120($fp)
  sw $t1, 0($t0)
  sw $t0, -116($fp)
  lw $t0, -124($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -124($fp)
  lw $t0, -128($fp)
  li $t0, 0
  sw $t0, -128($fp)
  lw $t0, -128($fp)
  li $t1, 4
  lw $t2, -128($fp)
  mul $t0, $t1, $t2
  sw $t0, -128($fp)
  lw $t0, -132($fp)
  lw $t1, -124($fp)
  lw $t2, -128($fp)
  add $t0, $t1, $t2
  sw $t0, -132($fp)
  lw $t0, -136($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -136($fp)
  lw $t0, -140($fp)
  li $t0, 1
  sw $t0, -140($fp)
  lw $t0, -140($fp)
  li $t1, 4
  lw $t2, -140($fp)
  mul $t0, $t1, $t2
  sw $t0, -140($fp)
  lw $t0, -144($fp)
  lw $t1, -136($fp)
  lw $t2, -140($fp)
  add $t0, $t1, $t2
  sw $t0, -144($fp)
  lw $t0, -148($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -148($fp)
  lw $t0, -152($fp)
  li $t0, 2
  sw $t0, -152($fp)
  lw $t0, -152($fp)
  li $t1, 4
  lw $t2, -152($fp)
  mul $t0, $t1, $t2
  sw $t0, -152($fp)
  lw $t0, -156($fp)
  lw $t1, -148($fp)
  lw $t2, -152($fp)
  add $t0, $t1, $t2
  sw $t0, -156($fp)
  lw $t0, -160($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -160($fp)
  lw $t0, -164($fp)
  li $t0, 3
  sw $t0, -164($fp)
  lw $t0, -164($fp)
  li $t1, 4
  lw $t2, -164($fp)
  mul $t0, $t1, $t2
  sw $t0, -164($fp)
  lw $t0, -168($fp)
  lw $t1, -160($fp)
  lw $t2, -164($fp)
  add $t0, $t1, $t2
  sw $t0, -168($fp)
  lw $t0, -172($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -172($fp)
  lw $t0, -176($fp)
  li $t0, 4
  sw $t0, -176($fp)
  lw $t0, -176($fp)
  li $t1, 4
  lw $t2, -176($fp)
  mul $t0, $t1, $t2
  sw $t0, -176($fp)
  lw $t0, -180($fp)
  lw $t1, -172($fp)
  lw $t2, -176($fp)
  add $t0, $t1, $t2
  sw $t0, -180($fp)
  lw $t0, -184($fp)
  addi $t1, $fp, -28
  move $t0, $t1
  sw $t0, -184($fp)
  lw $t0, -188($fp)
  li $t0, 5
  sw $t0, -188($fp)
  lw $t0, -188($fp)
  li $t1, 4
  lw $t2, -188($fp)
  mul $t0, $t1, $t2
  sw $t0, -188($fp)
  lw $t0, -192($fp)
  lw $t1, -184($fp)
  lw $t2, -188($fp)
  add $t0, $t1, $t2
  sw $t0, -192($fp)
  lw $t0, -196($fp)
  lw $t1, -192($fp)
  lw $t0, 0($t1)
  sw $t0, -196($fp)
addi $sp, $sp, -16
sw $a0, 0($sp)
sw $a1, 4($sp)
sw $a2, 8($sp)
sw $a3, 12($sp)
  lw $t0, -196($fp)
  move $a0, $t0
  lw $t0, -200($fp)
  lw $t1, -180($fp)
  lw $t0, 0($t1)
  sw $t0, -200($fp)
  lw $t0, -200($fp)
  move $a1, $t0
  lw $t0, -204($fp)
  lw $t1, -168($fp)
  lw $t0, 0($t1)
  sw $t0, -204($fp)
  lw $t0, -204($fp)
  move $a2, $t0
  lw $t0, -208($fp)
  lw $t1, -156($fp)
  lw $t0, 0($t1)
  sw $t0, -208($fp)
  lw $t0, -208($fp)
  move $a3, $t0
  lw $t0, -212($fp)
  lw $t1, -144($fp)
  lw $t0, 0($t1)
  sw $t0, -212($fp)
  lw $t0, -212($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  lw $t0, -216($fp)
  lw $t1, -132($fp)
  lw $t0, 0($t1)
  sw $t0, -216($fp)
  lw $t0, -216($fp)
  addi $sp, $sp, -4
  sw $t0, 0($sp)
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal fun
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  lw $t0, -220($fp)
  move $t0, $v0
  sw $t0, -220($fp)
lw $a0, 0($sp)
lw $a1, 4($sp)
lw $a2, 8($sp)
lw $a3, 12($sp)
addi $sp, $sp, 16
  lw $t0, -224($fp)
  li $t0, 0
  sw $t0, -224($fp)
  lw $t0, -224($fp)
  move $v0, $t0
  addi $sp, $sp, 228
  lw $fp, 0($sp)
  addi $sp, $sp, 4
  jr $ra
