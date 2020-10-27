# RISC-V Assembler
將一段RISC-V組語的code轉換為mechine code
> 並沒有實作所有類型
## 如何運行
編譯後運行程式，輸入資料檔名
需要檔案 `opcode.txt`

## 資料型態
### 輸入
RISC-V 格式

#### 輸入範例
```javascript
    add x2,x2,x23
    addi x24,x24,2
    bne x24,x23,L2
    sd x27, 0(x10)
    beq x0,x0,L1
L2: sd x1, 0(x2)
L1: 
```

### 輸出
mechine code

#### 輸出範例

```
00000001011100010000000100110011
00000000001011000000110000010011
00000001011111000001000101100011
00000001101100000000000000100011
00000000000000000000000101100011
00000000000100000000000000100011
```

## reference
https://github.com/rv8-io/rv8/blob/master/doc/pdf/riscv-instructions.pdf
