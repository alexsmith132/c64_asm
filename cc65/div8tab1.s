_divu_8
        lda div_b
        cmp #2
        bcs + ; >= 2

        lda div_a
        rts

+       ldx #8

-       dex
        asl
        bcc -

        bne +

        lda div_a
-       lsr
        dex
        bne -
        rts

+       tay
        lda r0_table,y
        ldy div_a

        sta zp8_1
        sta zp8_2
        eor #$ff
        sta zp8_3
        sta zp8_4

        sec
        lda (zp8_1),y
        sbc (zp8_3),y
        lda (zp8_2),y
        sbc (zp8_4),y

        clc
        adc div_a

        ror
-       lsr
        dex
        bne -
        rts

div_a
        .byte $0
div_b
        .byte $0
r0_table
        .byte $01,$00,$fd,$00,$f9,$00,$f5,$00,$f1,$00,$ed,$00,$ea,$00,$e6,$00
        .byte $e2,$00,$df,$00,$db,$00,$d8,$00,$d5,$00,$d1,$00,$ce,$00,$cb,$00
        .byte $c8,$00,$c4,$00,$c1,$00,$be,$00,$bb,$00,$b8,$00,$b5,$00,$b3,$00
        .byte $b0,$00,$ad,$00,$aa,$00,$a7,$00,$a5,$00,$a2,$00,$9f,$00,$9d,$00
        .byte $9a,$00,$98,$00,$95,$00,$93,$00,$90,$00,$8e,$00,$8b,$00,$89,$00
        .byte $87,$00,$84,$00,$82,$00,$80,$00,$7e,$00,$7b,$00,$79,$00,$77,$00
        .byte $75,$00,$73,$00,$71,$00,$6f,$00,$6d,$00,$6b,$00,$69,$00,$67,$00
        .byte $65,$00,$63,$00,$61,$00,$5f,$00,$5d,$00,$5b,$00,$59,$00,$58,$00
        .byte $56,$00,$54,$00,$52,$00,$51,$00,$4f,$00,$4d,$00,$4b,$00,$4a,$00
        .byte $48,$00,$47,$00,$45,$00,$43,$00,$42,$00,$40,$00,$3f,$00,$3d,$00
        .byte $3c,$00,$3a,$00,$39,$00,$37,$00,$36,$00,$34,$00,$33,$00,$31,$00
        .byte $30,$00,$2f,$00,$2d,$00,$2c,$00,$2a,$00,$29,$00,$28,$00,$26,$00
        .byte $25,$00,$24,$00,$22,$00,$21,$00,$20,$00,$1f,$00,$1d,$00,$1c,$00
        .byte $1b,$00,$1a,$00,$19,$00,$17,$00,$16,$00,$15,$00,$14,$00,$13,$00
        .byte $12,$00,$10,$00,$0f,$00,$0e,$00,$0d,$00,$0c,$00,$0b,$00,$0a,$00
        .byte $09,$00,$08,$00,$07,$00,$06,$00,$05,$00,$04,$00,$03,$00,$02,$00
