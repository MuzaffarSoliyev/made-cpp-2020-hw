.text

.global count_det

count_det:
	ldrsb r1, [r0], #4
	ldrsb r2, [r0], #4
	ldrsb r3, [r0], #4
	ldrsb r4, [r0], #4
	ldrsb r5, [r0], #4
	ldrsb r6, [r0], #4
	ldrsb r7, [r0], #4
	ldrsb r8, [r0], #4
	ldrsb r9, [r0], #4
	mul r0, r1, r5
	mul r0, r9
	mul r10, r2, r6
	mul r10, r7
	add r0, r10
	mul r10, r4, r8
	mul r10, r3
	add r0, r10
	
	mul r10, r3, r5
	mul r10, r7
	mul r1, r6
	mul r1, r8
	mul r4, r2
	mul r4, r9
	add r1, r10
	add r1, r4
	sub r0, r1
	push {r4-r10}
	pop {r4-r10}
	bx lr
	
