declare i32 @printf(i8*, ...)
declare i32 @atoi(i8*)
@.fmt_int    = private unnamed_addr constant [3 x i8]  c"%d\00"
@.fmt_double = private unnamed_addr constant [6 x i8]  c"%.16e\00"
@.fmt_true   = private unnamed_addr constant [5 x i8]  c"true\00"
@.fmt_false  = private unnamed_addr constant [6 x i8]  c"false\00"
@.fmt_str    = private unnamed_addr constant [3 x i8]  c"%s\00"
@global_argc = global i32 0

@.global_str_0 = private unnamed_addr constant [30 x i8] c"\54\65\73\74\65\20\55\6E\61\72\69\6F\20\28\64\65\76\65\20\73\65\72\20\2D\31\30\29\3A\20\00"
@.global_str_1 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.global_str_2 = private unnamed_addr constant [32 x i8] c"\4E\75\6D\65\72\6F\20\64\65\20\61\72\67\75\6D\65\6E\74\6F\73\20\70\61\73\73\61\64\6F\73\3A\20\00"
@.global_str_3 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.global_str_4 = private unnamed_addr constant [25 x i8] c"\4F\20\70\72\69\6D\65\69\72\6F\20\61\72\67\75\6D\65\6E\74\6F\20\65\3A\20\00"
@.global_str_5 = private unnamed_addr constant [2 x i8] c"\0A\00"
@.global_str_6 = private unnamed_addr constant [65 x i8] c"\50\61\72\61\20\74\65\73\74\61\72\20\65\6D\20\70\6C\65\6E\6F\2C\20\63\6F\72\72\65\20\6F\20\65\78\65\63\75\74\61\76\65\6C\20\70\61\73\73\61\6E\64\6F\20\75\6D\20\61\72\67\75\6D\65\6E\74\6F\21\0A\00"
@.global_str_7 = private unnamed_addr constant [29 x i8] c"\45\78\65\6D\70\6C\6F\3A\20\6C\6C\69\20\70\72\6F\67\72\61\6D\61\2E\6C\6C\20\34\32\0A\00"
@.global_str_8 = private unnamed_addr constant [62 x i8] c"\4F\20\74\72\69\70\6C\6F\20\64\6F\20\61\72\67\75\6D\65\6E\74\6F\20\28\63\61\6C\63\75\6C\61\64\6F\20\76\69\61\20\66\6F\72\77\61\72\64\20\64\65\63\6C\61\72\61\74\69\6F\6E\29\20\65\3A\20\00"
@.global_str_9 = private unnamed_addr constant [2 x i8] c"\0A\00"

@globalVar = global i32 0

define void @_avaliaArgumento_i(i32 %val) {
.L0:
  %0 = alloca i32
  %logical_stack = alloca [100 x i1]
  %val.addr = alloca i32
  store i32 %val, i32* %val.addr
  %1 = getelementptr inbounds [62 x i8], [62 x i8]* @.global_str_0, i32 0, i32 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %1)
  %3 = load i32, i32* %val.addr
  %4 = call i32 @_calculaTriplo_i(i32 %3)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %4)
  %6 = getelementptr inbounds [2 x i8], [2 x i8]* @.global_str_1, i32 0, i32 0
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %6)
  br label %.L1
.L1:
  br label %return
return:
  ret void
}

define i32 @_calculaTriplo_i(i32 %n) {
.L0:
  %0 = alloca i32
  %logical_stack = alloca [100 x i1]
  %n.addr = alloca i32
  store i32 %n, i32* %n.addr
  %1 = load i32, i32* %n.addr
  %2 = add i32 3, 0
  %3 = mul i32 %1, %2
  store i32 %3, i32* %0
  br label %return
return:
  %4 = load i32, i32* %0
  ret i32 %4
}

define i32 @main(i32 %argc, i8** %args) {
.L0:
  %0 = alloca i32
  %logical_stack = alloca [100 x i1]
  %1 = sub i32 %argc, 1
  store i32 %1, i32* @global_argc
  %args.addr = alloca i8**
  store i8** %args, i8*** %args.addr
  %len = alloca i32
  store i32 0, i32* %len
  %firstArg = alloca i32
  store i32 0, i32* %firstArg
  %unaryTest = alloca i32
  store i32 0, i32* %unaryTest
  %2 = add i32 10, 0
  store i32 %2, i32* %unaryTest
  %3 = load i32, i32* %unaryTest
  %4 = sub i32 0, %3
  store i32 %4, i32* %unaryTest
  %5 = load i32, i32* %unaryTest
  store i32 %5, i32* @globalVar
  %6 = getelementptr inbounds [30 x i8], [30 x i8]* @.global_str_2, i32 0, i32 0
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %6)
  %8 = load i32, i32* @globalVar
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %8)
  %10 = getelementptr inbounds [2 x i8], [2 x i8]* @.global_str_3, i32 0, i32 0
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %10)
  %12 = load i32, i32* @global_argc
  store i32 %12, i32* %len
  %13 = getelementptr inbounds [32 x i8], [32 x i8]* @.global_str_4, i32 0, i32 0
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %13)
  %15 = load i32, i32* %len
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %15)
  %17 = getelementptr inbounds [2 x i8], [2 x i8]* @.global_str_5, i32 0, i32 0
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %17)
  %19 = load i32, i32* %len
  %20 = add i32 0, 0
  %21 = icmp sgt i32 %19, %20
  br i1 %21, label %.L1, label %.L2
.L1:
  %22 = load i8**, i8*** %args.addr
  %23 = add i32 0, 0
  %24 = add i32 %23, 1
  %25 = getelementptr i8*, i8** %22, i32 %24
  %26 = load i8*, i8** %25
  %27 = call i32 @atoi(i8* %26)
  store i32 %27, i32* %firstArg
  %28 = getelementptr inbounds [25 x i8], [25 x i8]* @.global_str_6, i32 0, i32 0
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %28)
  %30 = load i32, i32* %firstArg
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_int, i32 0, i32 0), i32 %30)
  %32 = getelementptr inbounds [2 x i8], [2 x i8]* @.global_str_7, i32 0, i32 0
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %32)
  %34 = load i32, i32* %firstArg
  call void @_avaliaArgumento_i(i32 %34)
  br label %.L3
.L2:
  %36 = getelementptr inbounds [65 x i8], [65 x i8]* @.global_str_8, i32 0, i32 0
  %37 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %36)
  %38 = getelementptr inbounds [29 x i8], [29 x i8]* @.global_str_9, i32 0, i32 0
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.fmt_str, i32 0, i32 0), i8* %38)
  br label %.L3
.L3:
  br label %.L4
.L4:
  br label %return
return:
  ret i32 0
}

