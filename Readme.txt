# Input/output
    需要輸入要讀取的檔名，包括副檔名
# class and method
    * class riscv
        * public 
            * struct inform
                * string format
                * string opcode
                > struct for instruction to mechine code
                > * format : instruction parameter format
                > * opcode : reference : "https://github.com/rv8-io/rv8/blob/master/doc/pdf/riscv-instructions.pdf"
                >            instruction opcode format

            * void parseInsts(vector<string> text)
                > parse text to mechine codes
                > int the procss of this function will save the mechine code into riscv::mcode
	        * void print()
                > print mechine codes after parse;
	        * void setFormat(string inst, string format, string opcode)
                > * inst : instruction name
                > * format : format of inst
                > * opcode : opcode format of inst
                > to store instruction information into riscv::inst_format, including input format and mechine code setFormat
        * private 
            * string itob(int a)
                > * a : integer
                > parse integer a to binary type
            * int getValue(map<string, int>& label, string str, int p)
                > * label : key is name of the label, value is the position of the label
                > * str : which want to get real value
                > * p : current program index of the instruction
                > if the `str` could be find in `label`, return (label[str] - p), means the offset of the instruction and label
                > if the `str` is a register, return it's position
                > if the `str` is a number, return itself
                > all return type is integer  
            * vector<mechinecode> mcodes
                > store mechine codes
            * map<string, inform> inst_format
                > store information of instruction, about input format and opcode 
    * class mechinecode
        * public
            * mechinecode& operator=(const mechinecode&)
                > assign operator =
            * void clear()
                > clear mechinecode::code
            * bool set(string&)
                > just append parameter_1 behind mechinecode::code
        * private   
            * string code
                > for mechine code 
# algorithm
    seperate by two part
    ## 1. instruction format
        > it will store like this:
        > `beq rs1, rs2, offset = simm[12|10:5] rs2 rs1 funct3=000 simm[4:1|11] opcode=1100011`
        >
        > 'instuction name' 'input format' = 'instruction set to mechine code format'
        > the third part will be ordered according to the pdf of the website : "https://github.com/rv8-io/rv8/blob/master/doc/pdf/riscv-instructions.pdf"
    ## 2. parse instructions
        (1) deal with label and empty line
            > every label will be record, and then be removed
            > empty line will be removed
        (2) compare and match parameter
            > parameter will be matched with 'input format', find matched pair
            > example:
            > beq x5, x6, Loop
            > 
            > if Loop's position is 7, instruction at 9
            > it will become
            > beq x5, x6, -2
            >
            > and then matched `beq rs1, rs2, offset`
            > rs1 = 5
            > rs2 = 6
            > offset = -2
            > 
            > simm[..] and shamt[..] format will read value from `imm` or `offset`
            > finally, store into an array with binary code
# description
    in the main function, first thing is to call riscv::setFormat, set instruction format in the file 'opcode.txt'
    second is read program file with user input, and then call parseInsts().
    
    parseInsts(vector<string> text) will do some things:
    for each line
        remove tab(\tab)
        if find label
            record position
        endif
        if no instruction // means it will be a label without instruction or empty line
            remove line
        else //have instruction
            remove label if it has label in front of instruction
        endif
    endfor

    map<string, int> val // to store key and value
    
    mechinecode mc // temp for mechinecode
    for each instruction
        replace ',' by ' ', and store into `str1`
        read format from `inst_format`, put it in `str2`, and do the same things above

        set `stringstream ss1` be `str1`, `stringstream ss2` be `str2`
        for each parameter // use stringstream to seperate parameter due to replacing ','
            `str1` will be a value 
            `str2` will be a key
            if `str2` == "offset(rs1)"
                seperate by two part : offset and rs1
                also use stringstream
                set key and value into local value `val` // value will use getValue(,,) to parse
            else
                set key and value into local value `val` // value will use getValue(,,) to parse
            endif

        endfor
        for each parts in opcode of `inst_format`
            set each part into `str1`
            if `str1` have charactor '[' means it is imm[..]
                parse value or offset or imm into binary code
                seperate by '|'
                if has ':'
                    cut a part of binary value and put in mc, use set()
                else
                    put single value into `mc`, use set()
                endif 
            elseif `str1` have charactor '=' means it is ..=..
                put string after '=' into `mc`
            else
                put value into mc, using key match with `str1` and key in the `val`
            endif
            put `mc` into `mcode`
        endfor
    endfor


   
